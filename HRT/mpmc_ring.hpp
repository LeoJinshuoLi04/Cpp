
#include <atomic>
#include <cstddef>
#include <optional>
#include <thread>
#include <tuple>
#include <vector>

template <typename T>
class MpmcRing {
private:
  size_t capacity_;
  std::vector<T> values_;
  std::atomic<size_t> push_idx = 0;
  std::atomic<size_t> pop_idx = 0;

  size_t advance(size_t idx){
    return (idx + 1) % (capacity_ + 1);
  }

public:
    explicit MpmcRing(std::size_t capacity) : capacity_(capacity) {
      values_.resize(capacity + 1);
    }

    bool try_push(const T& value) {
      auto push = push_idx.load(std::memory_order_relaxed);
      auto next = advance(push);
      if(next == pop_idx.load(std::memory_order_acquire)) return false;
      values_[push] = value;
      push_idx.store(next, std::memory_order_release);

      return true;
    }

    std::optional<T> try_pop() {
      auto pop = pop_idx.load(std::memory_order_relaxed);
      if(pop == push_idx.load(std::memory_order_acquire)) return std::nullopt;
      auto res = values_[pop];
      pop_idx.store(advance(pop), std::memory_order_release);
      return res;
    }
};
