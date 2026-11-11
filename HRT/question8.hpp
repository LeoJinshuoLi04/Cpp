/*

Problem 6 — Dynamic Order Book
Design a limit order book that supports insertion, cancellation, and execution of orders, preserving price-time priority.

1. Basic design: Support add(order), cancel(order_id), and match(). What data structures would you choose to efficiently 
   obtain the best bid and ask?
2. Fast cancellation: How can you cancel any order by ID in O(1) average time, excluding any ordered price-level maintenance?
3. Price updates: An order can change its price or quantity. How would you update the order without violating price-time priority?
4. Allocation overhead: Every order currently requires a heap allocation. How would you eliminate most allocations using 
   memory pools or intrusive structures?
5. Concurrency: Market data and client order requests arrive from multiple threads. Should you allow multiple threads 
   to modify the order book concurrently, or use a single matching thread with an input queue?
6. Snapshots: Another thread needs to read a consistent snapshot of the book without significantly delaying matching. 
   How would you support this?

Final challenge: Your implementation must process millions of updates per second with predictable tail latency. 
Which design decisions would you reconsider?
*/