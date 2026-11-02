/*

Problem 2 — Indexed Priority Scheduler
Heaps · Hash tables · Exception safety · Thread safety
Design a scheduler that executes tasks according to their deadline, but also allows tasks to be cancelled or rescheduled.

1. Basic design: Implement schedule(task, deadline) and execute_next() with O(log n) 
   insertion and removal.
2. Cancellation: Each scheduled task has a unique ID. How would you support 
   cancel(task_id) in O(log n)?
3. Rescheduling: How would you support changing the deadline of an existing task in O(log n), 
   without removing and reinserting it?
4. Memory management: Tasks may own resources and contain move-only callables. How do you 
   store them while minimizing allocations?
5. Concurrency: Multiple threads can add, cancel, and reschedule tasks while one thread executes them. 
   How would you synchronize the heap and the ID lookup structure?
6. Advanced design: Suppose task execution can itself schedule or cancel tasks. How would you 
   avoid deadlocks and iterator invalidation?

Final challenge: Optimize the scheduler for millions of short-lived tasks where most tasks are cancelled before execution.

*/