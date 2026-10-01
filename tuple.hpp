template <typename... Ts>
class Tuple;

template<typename T>
class Tuple<T> {
  protected:
    T obj;
  
  public:

    Tuple(): size_(1) {};

    static constexpr std::size_t size() {
      return 1;
    }
};

template <typename T, typename... Args> 
class Tuple<T, Args...> : Tuple<Args...> {
  protected:
    T obj_;
  
  public:

    static constexpr std::size_t size() {
      return sizeof...(Args) + 1;
    }

    template<int N>
    decltype (auto) get<N>(){
      if constexpr (N == 0){
        return (obj_);
      }else {
        return Tuple<Args...>::template get<N - 1>();
      }
    }
};