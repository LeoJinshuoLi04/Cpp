#include <atomic>
#include <chrono>
#include <coroutine>
#include <future>
#include <iostream>
#include <thread>

static int counter = 0;

struct Task {
  struct promise_type {
    std::mutex m;
    std::condition_variable cv;
    bool done_processing = true;
    int value = 0;

    Task get_return_object() {
      return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_never initial_suspend() noexcept { return {}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    void return_value(int val) { value = val; }

    std::suspend_always yield_value(int val) {
      value = val;
      return {};
    }

    void unhandled_exception() {}
  };

  using handle_type = std::coroutine_handle<promise_type>;
  handle_type handle;

  int get() {
    int current = handle.promise().value;

    if (!handle.done()) handle.resume();

    return current;
  }

  struct Awaiter {
    bool await_ready() { return false; }

    int await_resume() { return counter; }

    void await_suspend(std::coroutine_handle<> handle) {
      std::thread([this, handle]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++counter;
      }).detach();
    }
  };
};

auto complex_operation() -> Task::Awaiter { return {}; }

auto coroutine() -> Task {
  auto next = complex_operation();
  for (int i = 0; i < 3; ++i) {
    auto current = co_await next;

    next = complex_operation();

    co_yield current;
  }
  co_return -1;
}

int main() {
  auto task = coroutine();
  for (int i = 0; i < 3; ++i) {
    std::cout << task.get() << '\n';
  }

  return 0;
}