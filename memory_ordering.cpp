#include <atomic>

std::atomic<int> x{}, y{};

void thread1() {
  x.fetch_add(1, std::memory_order_release);     // A
  auto yVal = y.load(std::memory_order_acquire); // B
}

void thread2() {
  y.fetch_add(1, std::memory_order_release);     // C
  auto xVal = x.load(std::memory_order_acquire); // D
}

/*
  In above, A can occur before B and D before C because they could
  occur on different cores and not propagate. Using seq_cst guarantees a
  global ordering of the instructions.
*/

void thread1() {
  x.fetch_add(1); // A
  x.fetch_add(1); // B
}

void thread2() { auto xVal = x.load(); } // C

/*
 In above, thread2 can observe 0, 1, or 2.
 thread1 guarantees A < B, but C could be :
 C < A < B,    A < C < B,    or A < B < C

 seq_cst is mainly for sync across many global vars
*/