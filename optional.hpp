#include <concepts>
#include <cstddef>
#include <memory>
#include <utility>

struct nullopt_t{
  explicit constexpr nullopt_t() = default;
}

inline constexpr nullopt = nullopt_t();

template<typename T>
concept small_type = sizeof(T) < 16;

template <typename T> 
class Optional {
  private:
    T* data_ = nullptr;
  public:
    Optional() = default;
    Optional(nullopt_t) noexcept : data_(nullptr) {};

    Optional(T& data) : data_(new T(data)) {};
    Optional(T&& data) : data_(new T(std::move(data))) {};

    Optional(const Optional& other) {
      if(other.data_){
        data_ = new T(other.data_);
      }
    }

    Optional(Optional&& other) noexcept {
      data_ = std::exchange(other.data_, nullptr);
    }

    Optional& operator=(const Optional& other){
      if (this == &other) return *this;

      T* replacement = other.data_
          ? new T(*other.data_)
          : nullptr;

      delete data_;
      data_ = replacement;

      return *this;
    }

    Optional& operator=(Optional&& other) noexcept {
      if(data_) delete data_;
      data_ = std::exchange(other.data_, data_);
    }

    operator bool() const noexcept {
      return data_;
    }

    ~Optional(){ delete data_; }
};

template <small_type T>
class Optional<T> {
  private:
    union{ T data_; };
    bool present = false;
  public:
    Optional() = default;

    Optional(T& data) : present(true) {
      std::construct_at(reinterpret_cast<T*>(&data_), data);
    };

    Optional(T&& data) : present(true) {
      std::construct_at(reinterpret_cast<T*>(&data_), std::move(data));
    };

    Optional(nullopt_t) noexcept : present(false) {};

    Optional(const Optional& other) : present(other.present){
      if(present){
        std::construct_at(data_, reinterpret_cast<T>(other.data_));
      }
    }

    Optional(Optional&& other) noexcept : present(other.present){
      if(data_) std::destory_at(reinterpret_cast<T*>(&data_));
      if(present) {
        std::construct_at(reinterpret_cast<T*>(&data), std::move(reinterpret_cast<T>(other.data_)));
      }
    }

    Optional& operator=(const Optional& other){
      if (this == &other) return *this;

      present = other.present;
      if(data_) std::destory_at(reinterpret_cast<T*>(&data_));
      if(present) std::construct_at(reinterpret_cast<T*>(&data), reinterpret_cast<T>(other.data_));

      return *this;
    }

    Optional& operator=(Optional&& other) noexcept {
      present = other.present;
      if(data_) std::destory_at(reinterpret_cast<T*>(&data_));
      if(present) std::construct_at<reinterpret_cast<T*>(data_, std::move(reinterpret_cast<T>(other.data_)));
      std::destory_at(reinterpret_cast<T*>(&other.data_));
    }

    operator bool() const noexcept {
      return present;
    }

    ~Optional(){ 
      if(present){
        std::destroy_at(reinterpret_cast<T*>(&data_));
      }
    }
};