
#include <atomic>
#include <cstddef>
#include <optional>
#include <thread>
#include <tuple>
#include <vector>

template <typename T>
concept wontThrow = std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>;

template <wontThrow T>
class MpmcRing {
private:
  size_t capacity_;
  std::vector<T> values_;
  std::atomic<size_t> push_idx = 0;
  std::atomic<size_t> push_complete_idx = 0;
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
      if(next == push_idx.load(std::memory_order_acquire)) return false;

      while (!push_idx.compare_exchange_weak(push, next, std::relaxed, std::memory_order_relaxed)){
        next = advance(push);
        if(next == pop_idx.load(std::memory_order_acquire)) return false;
      }

      values_[push] = value;

      auto checkPush = push;
      while(!push_complete_idx.compare_exchange_weak(checkPush, next, std::memory_order_release, std::memory_order_relaxed)){
        checkPush = push;
      }

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
