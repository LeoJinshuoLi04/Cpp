#include <algorithm>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>

template <typename T, std::size_t N = std::max<std::size_t>(512 / sizeof(T), 1)>
class Deque {
 public:
  static_assert(N > 0);

  Deque()
      : pointers_(new T* [initial_pointer_capacity_] {}),
        pointer_start_idx_(initial_pointer_capacity_ / 2),
        pointer_end_idx_(initial_pointer_capacity_ / 2),
        pointer_capacity_(initial_pointer_capacity_) {}

  Deque(const Deque&) = delete;
  Deque& operator=(const Deque&) = delete;

  T& push_back(const T& data) {
    ensure_block(pointer_end_idx_);

    T* result =
        std::construct_at(pointers_[pointer_end_idx_] + block_end_idx_, data);

    ++block_end_idx_;

    if (block_end_idx_ == N) {
      block_end_idx_ = 0;
      ++pointer_end_idx_;

      if (pointer_end_idx_ == pointer_capacity_) {
        expand();
      }
    }

    ++size_;
    return *result;
  }

  T& push_front(const T& data) {
    if (block_start_idx_ == 0) {
      if (pointer_start_idx_ == 0) {
        expand();
      }

      --pointer_start_idx_;
      block_start_idx_ = N - 1;
    } else {
      --block_start_idx_;
    }

    ensure_block(pointer_start_idx_);

    T* result = std::construct_at(
        pointers_[pointer_start_idx_] + block_start_idx_, data);

    ++size_;
    return *result;
  }

  void pop_back() {
    if (empty()) {
      return;
    }

    if (block_end_idx_ == 0) {
      --pointer_end_idx_;
      block_end_idx_ = N;
    }

    --block_end_idx_;

    std::destroy_at(pointers_[pointer_end_idx_] + block_end_idx_);

    --size_;
  }

  void pop_front() {
    if (empty()) {
      return;
    }

    std::destroy_at(pointers_[pointer_start_idx_] + block_start_idx_);

    ++block_start_idx_;

    if (block_start_idx_ == N) {
      block_start_idx_ = 0;
      ++pointer_start_idx_;
    }

    --size_;
  }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  [[nodiscard]] std::size_t size() const noexcept { return size_; }

  T& operator[](std::size_t index) {
    const std::size_t absolute_offset = block_start_idx_ + index;

    const std::size_t block = pointer_start_idx_ + absolute_offset / N;

    const std::size_t offset = absolute_offset % N;

    return pointers_[block][offset];
  }

  const T& operator[](std::size_t index) const {
    const std::size_t absolute_offset = block_start_idx_ + index;

    const std::size_t block = pointer_start_idx_ + absolute_offset / N;

    const std::size_t offset = absolute_offset % N;

    return pointers_[block][offset];
  }

  ~Deque() {
    std::size_t block = pointer_start_idx_;
    std::size_t offset = block_start_idx_;

    for (std::size_t i = 0; i < size_; ++i) {
      std::destroy_at(pointers_[block] + offset);

      ++offset;

      if (offset == N) {
        offset = 0;
        ++block;
      }
    }

    for (std::size_t i = 0; i < pointer_capacity_; ++i) {
      if (pointers_[i] != nullptr) {
        ::operator delete(pointers_[i]);
      }
    }

    delete[] pointers_;
  }

 private:
  static constexpr std::size_t initial_pointer_capacity_ = 8;

  T** pointers_;

  std::size_t pointer_start_idx_;
  std::size_t pointer_end_idx_;

  std::size_t block_start_idx_ = 0;
  std::size_t block_end_idx_ = 0;

  std::size_t pointer_capacity_;
  std::size_t size_ = 0;

  void ensure_block(std::size_t index) {
    if (pointers_[index] == nullptr) {
      pointers_[index] = static_cast<T*>(::operator new(sizeof(T) * N));
    }
  }

  void expand() {
    const std::size_t old_capacity = pointer_capacity_;
    const std::size_t new_capacity = old_capacity * 3;

    T** new_array = new T* [new_capacity] {};

    for (std::size_t i = 0; i < old_capacity; ++i) {
      new_array[i + old_capacity] = pointers_[i];
    }

    pointer_start_idx_ += old_capacity;
    pointer_end_idx_ += old_capacity;

    delete[] pointers_;

    pointers_ = new_array;
    pointer_capacity_ = new_capacity;
  }
};