#include <deque>
#include <mutex>
#include <optional>
#include <memory>

/*
Design a data structure backed by a deque that supports push/pop from both ends and O(1) access by logical
index. How do you use an offset to maintain stable logical indexing as elements are removed from the front?

How would you implement this without stable iterators?

How would you handle memory reclamation as elements are popped?

How would concurrency affect the offset map and the deque together?

How would you support random deletion by logical index?
*/

template< typename T>
class MyQueue {
  private:

    std::deque<std::shared_ptr<T>> queue_;
    long long offset_ = 0;
    mutable std::mutex queue_mutex{};

  public:
    MyQueue() = default;
    
    void push_back(const T& val){
      std::scoped_lock lock{queue_mutex};
      queue_.push_back(std::make_shared<T>(val));
    }

    void push_back(T&& val){
      std::scoped_lock lock{queue_mutex};
      queue_.push_back(std::make_shared<T>(std::move(val)));
    }

    void push_front(const T& val){
      std::scoped_lock lock{queue_mutex};
      queue_.push_front(std::make_shared<T>(val));
      --offset_;
    }

    void push_front(T&& val){
      std::scoped_lock lock{queue_mutex};
      queue_.push_front(std::make_shared<T>(std::move(val)));
      --offset_;
    }

    void pop_back() {
      std::scoped_lock lock{queue_mutex};
      while(!queue_.empty() && !queue_.back()) queue_.pop_back();

      if(queue_.empty()) return;
      queue_.pop_back();

      while(!queue_.empty() && !queue_.back()) queue_.pop_back();
    }

    void pop_front() {
      std::scoped_lock lock{queue_mutex};
      while(!queue_.empty() && !queue_.front()) {
        queue_.pop_front();
        ++offset_;
      }

      if(queue_.empty()) return;
      queue_.pop_front();
      ++offset_;

      while(!queue_.empty() && !queue_.front()) {
        queue_.pop_front();
        ++offset_;
      }
    }

    void erase(long long index){
      std::scoped_lock lock{queue_mutex};
      if(index < offset_ || index >= offset_ + static_cast<long long>(queue_.size())) return;
      queue_[index - offset_].reset();
    }

    std::shared_ptr<T> operator[](long long index) {
      std::scoped_lock lock{queue_mutex};
      return queue_[index - offset_];
    }
};

// takeaways - use case for shared pointer yay
