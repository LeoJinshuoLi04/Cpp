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
public:
    explicit SpscRing(std::size_t capacity) {
        // ...
        // ...
        // ...
        (void)capacity;
    }

    bool try_push(const T& value) {
        // ...
        (void)value;
        return false;
    }

    std::optional<T> try_pop() {
        // ...
        return std::nullopt;
    }

private:
    // ...
    // ...
};
