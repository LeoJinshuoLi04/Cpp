#include <memory>

template <typename T, typename... Args> class function {
private:
  class callable_base {
    virtual T invoke(Args... args) = 0;
  }

  template <typename F>
  class callable : public callable_base {
    callable(F f) : f_(std::move(f)) {};

    T invoke(Args... args) override { return f_(std::forward<Args>(args)...); }

    F f_;
  };

  std::unique_ptr<callable_base> func_;

public:
  template <typename F>
  function(F f) : func_(std::make_unique<callable<F>>(std::move<F>(f))) {};

  R operator()(Args... args) {
    return func_->invoke(std::forward<Args>(args)...);
  }
};