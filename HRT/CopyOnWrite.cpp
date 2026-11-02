
#include <atomic>
#include <utility>
#include <cassert>
#include <iostream>


class CowString {
  private:
  
  struct Buffer {
     char* data = nullptr;
     std::atomic<size_t> ref = 0;
    
     Buffer(char* str) : data(str) { ref.fetch_add(1);};
  };
  
  Buffer* buffer;

  friend int main();

  struct CharProxy{
    CowString* owner;
    size_t index;

    CharProxy(CowString* o, size_t idx) : owner(o), index(idx) {};
    
    char& operator=(char c){
      char* newString = new char[strlen(owner->buffer->data)];
      strcpy(newString, owner->buffer->data);
    	auto* newBuffer = new Buffer(newString);

    	if(owner->buffer->ref.fetch_sub(1, std::memory_order_relaxed) == 0){
        delete(owner->buffer->data);
      }

      owner->buffer = newBuffer;
      newBuffer->data[index] = c;

      return newBuffer->data[index];
    }

    operator char(){
      return owner->buffer->data[index];
    }
  };
  
  CowString(Buffer* buffer_) : buffer(buffer_) {};
    
  public:
  	CowString() = delete;
  
  	CowString(char* str){
      buffer = new Buffer(str);
    }
  
  	CowString(const CowString& other){
      buffer = other.buffer;
      buffer->ref.fetch_add(1);
    }
  
  	CowString(CowString&& other) {
      buffer = std::exchange(other.buffer, nullptr);
    }
  
  	CowString& operator=(const CowString& other){
      if(this == &other) return *this;
      
      if(buffer->ref.fetch_sub(1, std::memory_order_relaxed) == 0){
        delete buffer->data;
      };
      
      other.buffer->ref.fetch_add(1, std::memory_order_relaxed);
      buffer = other.buffer;
      
      return *this;
    }
  
  	char operator[](size_t index) const {
      return buffer->data[index];
    }
  
  	CharProxy operator[](size_t index) {
      return CharProxy(this, index);
    }
  
  	~CowString(){
      if(buffer->ref.fetch_sub(1, std::memory_order_relaxed) == 0){
        delete buffer->data;
      }
    }
};

int main(){
  auto original = std::string{"hello"};
  CowString first{original.data()};
  auto copy = CowString{first};
  assert(copy.buffer->data == first.buffer->data);
  copy = first;
  assert(copy.buffer->data == first.buffer->data);
  copy[0] = 'y';
  std::cout << copy.buffer->data;

  return 0;
}