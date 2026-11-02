#include <atomic>
#include <cstddef>
#include <optional>
#include <thread>
#include <tuple>
#include <vector>

/*
Implement a generic, bounded, thread-safe SpscRing<T> supporting concurrent access by exactly two threads: one producer and one consumer.

The producer inserts elements into a fixed-capacity circular buffer, and the consumer removes them in FIFO order.

The queue must use std::atomic to synchronise the threads without mutexes or condition variables.

Implement the following operations:
- SpscRing(capacity): Initialise a ring buffer capable of holding exactly capacity elements.
- try_push(value): Attempt to enqueue an element. Return true on success and false if the buffer is full. Never wait for space.
- try_pop(): Attempt to dequeue the oldest element. Return it as std::optional<T>, or std::nullopt if the buffer is empty. Never wait for data.
Preserve FIFO ordering and ensure that every successfully pushed element is consumed exactly once.

The implementation must be safe when one thread calls try_push() and another simultaneously calls try_pop().
*/

template <typename T>
class SpscRing {
private:
  size_t capacity_;
  std::vector<T> values_;
  std::atomic<size_t> push_idx = 0;
  std::atomic<size_t> pop_idx = 0;

  size_t advance(size_t idx){
    return (idx + 1) % (capacity_ + 1);
  }

public:
    explicit SpscRing(std::size_t capacity) : capacity_(capacity) {
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
