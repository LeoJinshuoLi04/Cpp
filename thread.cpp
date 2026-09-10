#include <iostream>
#include <thread>

int main() {
  int counter{};

  auto increment_counter = [&counter]() {
    for (int i = 0; i < 1000000; ++i) {
      ++counter;
    }
  };

  std::thread t(increment_counter);
  increment_counter();

  t.join();

  std::cout << counter << '\n';
};

// basic race condition thingy