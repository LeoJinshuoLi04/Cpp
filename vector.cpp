#include <cstddef>
#include <new>
#include <utility>

template <typename T> class vector {
public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = T *;

  vector() : data_(nullptr), size_(0), capacity_(0) {}

  ~vector() {
    clear();
    ::operator delete(data_);
  }

  vector(const vector &other) = delete;
  vector &operator=(const vector &other) = delete;

  vector(vector &&other) noexcept {
    data_ = std::exchange(other.data_, nullptr);
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
  }

  vector &operator=(vector &&other) noexcept {
    if (this != &other) {
      clear();
      ::operator delete(data_);

      data_ = std::exchange(other.data_, nullptr);
      size_ = std::exchange(other.size_, 0);
      capacity_ = std::exchange(other.capacity_, 0);
    }
    return *this;
  }

  T &operator[](size_type pos) { return data_[pos]; }

  [[nodiscard]] size_type size() const noexcept { return size_; }
  [[nodiscard]] size_type capacity() const noexcept { return capacity_; }
  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  void reserve(size_type new_cap) {
    if (new_cap <= capacity_)
      return;

    T *new_data = static_cast<T *>(::operator new(new_cap * sizeof(T)));

    for (size_type i = 0; i < size_; ++i) {
      new (new_data + i) T(std::move_if_noexcept(data_[i]));
    }

    for (size_type i = 0; i < size_; ++i) {
      data_[i].~T();
    }

    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_cap;
  }

  void push_back(const T &value) {
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(value);
    ++size_;
  }

  void push_back(T &&value) {
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
  }

  void pop_back() {
    if (empty())
      return;
    --size_;
    data_[size_].~T();
  }

  void clear() {
    for (size_type i = 0; i < size_; ++i) {
      data_[i].~T();
    }
    size_ = 0;
  }

  [[nodiscard]] iterator begin() noexcept { return data_; }
  [[nodiscard]] iterator end() noexcept { return data_ + size_; }

private:
  T *data_;
  size_type size_;
  size_type capacity_;
};