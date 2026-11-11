/*

Design a memory pool that stores objects of type T and returns a handle when an object is allocated.
1. Basic design: How would you implement allocate(), get(handle), and release(handle) such that allocation
   and release are O(1)?
2. Stable handles: Objects can be released in any order. How would you ensure existing handles remain 
   valid even if the underlying storage grows?
3. Handle reuse: Suppose object A is released, and its slot is reused for object B. How do you prevent 
   an old handle to A from accidentally accessing B?
4. Object lifetime: How would you support non-default-constructible and non-copyable objects? How do placement 
   construction and destruction work?
5. Concurrency: How would you support multiple threads allocating and releasing objects concurrently? Can you 
   implement a lock-free free list?
6. Memory reclamation: If one thread is using an object while another releases it, how can you safely reclaim its
   memory? Would reference counting, hazard pointers, or epoch-based reclamation help?

Final challenge: Can you keep the entire pool cache-friendly while guaranteeing O(1) handle lookup?
*/

#include <vector>
#include <stack>
#include <optional>
#include <atomic>


template <typename T> 
class MemoryPool {
  private:
  size_t index;
  std::stack<size_t> free_;

  struct Data {
    size_t generation = 0;
    std::optional<T> object = std::nullopt;
    std::atomic<size_t> ref;
  };

  std::vector<Data> data_;

  public:
  struct Handle {
    size_t index = 0;
    size_t generation = 0;
  };

  Handle allocate(){
    if(!free_.empty());
    auto temp = free_.top();
    free_.pop();
    return {temp, data_[temp].generation};
  }

  std::optional<T>& get(handle handle){
    if(data_[handle.index].generation != handle.generation) return std::nullopt;
    return data_[handle].object;
  }

  void release(handle) {
    free_.push(handle);
    data_[handle.index].object = std::nullopt;
    ++data_[handle.index].generation;
  }

};
