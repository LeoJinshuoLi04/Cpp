
#include <utility>

template <typename T> class unique_ptr {
public:
  using element_type = T;

  constexpr unique_ptr() noexcept : unique_ptr(nullptr) {}
  explicit unique_ptr(T *ptr) : ptr_(ptr) {}

  unique_ptr(const unique_ptr &other) = delete;
  unique_ptr(unique_ptr &&other) noexcept
      : ptr_(std::exchange(other.ptr_, nullptr)) {}

  unique_ptr &operator=(const unique_ptr &other) noexcept = delete;
  unique_ptr &operator=(unique_ptr &&other) noexcept {
    if (this != &other) {
      reset(other.release());
    }
    return *this;
  }

  T *release() { return std::exchange(ptr_, nullptr); }

  void reset(T *ptr = nullptr) {
    T *old = std::exchange(ptr_, ptr);
    delete (old);
  }

  void swap(unique_ptr &other) noexcept { std::swap(other.ptr_, ptr_); }

  explicit operator bool() const noexcept { return ptr_; }

  T *get() const noexcept { return ptr_; }

  T &operator*() const { return *ptr_; }

  T *operator->() const noexcept { return ptr_; }

  ~unique_ptr() { delete ptr_; }

private:
  T *ptr_;
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args &&...args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
