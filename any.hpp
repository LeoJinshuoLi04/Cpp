template <typename T>
class AnyObject {
  void getObject(void* data){
    return reinterpret_cast<T*>(data);
  }

  template <typename O>
  bool typeCheck(){
    return typeid(O) == typeid(T);
  }

  template <typename... Args>
  void construct(void* data, Args args){
    std::construct_at(reinterpret_cast<T*>(data), ...args);
  }

  void destroy(void* data) {
    std::destroy_at(reinterpret_cast<T*>(data));
  }

};

class Any {
public:
    template<typename T>
    Any(T value);

    Any(const Any&);
    Any(Any&&);

    template<typename T>
    T& get();

    template<typename T>
    const T& get() const;

    void reset();
};