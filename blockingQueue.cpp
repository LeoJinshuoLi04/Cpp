#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>

template <typename T> class BlockingQueue {
public:
  explicit BlockingQueue(std::size_t capacity)
      : capacity_{capacity}, isActive_(true) {}

  void push(T value) {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    producer_cv_.wait(
        lock, [this]() { return queue_.size() < capacity_ || !isActive_; });
    if (!isActive_)
      return;
    queue_.push_back(value);
    consumer_cv_.notify_one();
  }

  std::optional<T> pop() {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    consumer_cv_.wait(lock,
                      [this]() { return queue_.size() > 0 || !isActive_; });
    if (queue_.empty() && !isActive_) {
      return std::nullopt;
    }
    auto res = queue_.front();
    queue_.pop_front();
    if (isActive_) {
      producer_cv_.notify_one();
    }
    return res;
  }

  void close() {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    isActive_ = false;
    consumer_cv_.notify_all();
    producer_cv_.notify_all();
  }

private:
  std::mutex queue_mutex_{};
  std::condition_variable consumer_cv_{};
  std::condition_variable producer_cv_{};
  std::size_t capacity_;

  std::deque<T> queue_;

  bool isActive_;
};
int main() {
  BlockingQueue<int> q(2); // Small capacity to force blocking quickly

  std::atomic<bool> producer_finished{false};
  std::atomic<bool> consumer_finished{false};

  // Start a producer that will fill the queue and block on the 3rd push
  std::thread producer([&] {
    q.push(1);
    q.push(2);
    q.push(3); // Blocks here because capacity is 2
  });

  // Give the producer time to fill the queue and block
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  q.close();

  // Start a consumer that will drain the queue and then block when empty
  std::thread consumer([&] {
    while (auto val = q.pop()) {
      std::cout << "[Consumer] Popped: " << *val << "\n";
    }
  });

  producer.join();
  consumer.join();

  return 0;
}