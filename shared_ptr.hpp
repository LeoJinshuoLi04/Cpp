
#include <atomic>
#include <cstddef>
#include <utility>

template <typename T> class shared_ptr;
template <typename T> class weak_ptr;

template <typename T> class control_block {
  friend class shared_ptr<T>;
  friend class weak_ptr<T>;

  control_block() = delete;

  void add_weak_reference() {
    weak_count_.fetch_add(1, std::memory_order_relaxed);
  }

  void remove_weak_reference() {
    weak_count_.fetch_sub(1, std::memory_order_acq_rel);
  }

  void add_reference() {
    reference_count_.fetch_add(1, std::memory_order_relaxed);
  }

  void remove_reference() {
    if (reference_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
      delete_data();
    };
  }

  virtual void delete_data() = 0;

  std::atomic<std::size_t> reference_count_{1};
  std::atomic<std::size_t> weak_count_{0};
};

template <typename T>
class control_block_pointer_impl : public control_block<T> {
public:
  control_block_pointer_impl(T *ptr) : data_(ptr) {}

  void delete_data() override { delete data_; }

private:
  T *data_;
};

template <typename T, typename... Args>
class control_block_owning_impl : public control_block<T> {
public:
  control_block_owning_impl(Args &&...args)
      : data_(std::forward<Args>(args)...) {}

  void delete_data() override { data_.~T(); }

private:
  friend class shared_ptr<T>;
  T data_;
};

template <typename T> class shared_ptr {
public:
  shared_ptr() noexcept : c_block_(nullptr) {};

  shared_ptr(T *ptr)
      : c_block_(new control_block_pointer_impl(ptr)), data_(ptr) {};

  shared_ptr(weak_ptr<T> weak) : c_block_(weak.c_block_) {
    if (!c_block_)
      return;
    auto current_count = c_block_->reference_count_.load();

    while (current_count > 0) {
      if (c_block_->reference_count_.compare_exchange_strong(
              current_count, current_count + 1, std::memory_order_acq_rel,
              std::memory_order_relaxed)) {
        return;
      }
    }

    c_block_ = nullptr;
  }

  shared_ptr(const shared_ptr &other) : c_block_(other.c_block_) {
    if (c_block_) {
      c_block_->add_reference();
    }
  }

  shared_ptr &operator=(shared_ptr other) {
    std::swap(c_block_, other.c_block_);
    return *this;
  }

  shared_ptr(shared_ptr &&other) noexcept : c_block_(other.c_block_) {
    other.c_block_ = nullptr;
  }

  shared_ptr &operator=(shared_ptr &&other) {
    std::swap(other.c_block_, c_block_);
    return *this;
  }

  T *get() { return data_; };

  size_t use_count() {
    if (!c_block_)
      return 0;
    return c_block_->reference_count_.load(std::memory_order_acquire);
  };

  void reset() {
    if (!c_block_)
      return;
    c_block_->remove_reference();
    if (!c_block_->weak_count_.load() && !c_block_->reference_count_.load()) {
      delete (c_block_);
    }
    c_block_ = nullptr;
  };

  ~shared_ptr() { reset(); }

private:
  template <typename... Args> friend shared_ptr<T> make_shared(Args &&...args);

  template <typename... Args> shared_ptr(Args &&...args) {
    auto *block =
        new control_block_owning_impl<T, Args...>(std::forward<Args>(args)...);
    c_block_ = block;
    data_ = &block->data_;
  }

  control_block<T> *c_block_;
  T *data_;
};

template <typename T> class weak_ptr {
public:
  weak_ptr(shared_ptr<T> &other) : c_block_(other.c_block_) {
    if (c_block_) {
      c_block_->add_weak_reference();
    }
  };

  weak_ptr(const weak_ptr &other) : c_block_(other.c_block_) {
    if (c_block_) {
      c_block_->add_weak_reference();
    }
  }

  weak_ptr &operator=(weak_ptr other) {
    std::swap(c_block_, other.c_block_);
    return *this;
  };

  bool expired() {
    if (!c_block_)
      return true;
    return !c_block_->reference_count_.load();
  };

  void reset() {
    if (!c_block_)
      return;
    c_block_->remove_weak_reference();
    if (!c_block_->weak_count_.load() && !c_block_->reference_count_.load()) {
      delete c_block_;
    }
    c_block_ = nullptr;
  }

  shared_ptr<T> lock() { return shared_ptr<T>(*this); }

  ~weak_ptr() { reset(); }

private:
  friend class shared_ptr<T>;
  control_block<T> *c_block_;
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args &&...args) {
  return shared_ptr<T>(std::forward<Args>(args)...);
}
