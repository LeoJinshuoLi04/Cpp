/*

Problem 5 — Custom Flat Hash Map
Implement a hash map optimized for fast lookups and minimal pointer chasing, using an array with open addressing.

1. Basic design: Implement insertion, lookup, and deletion with expected O(1) complexity. 
   How would you handle collisions?
2. Deletion: Removing an entry must not break the probe chain for other keys. How would you
   implement deletion without tombstones?
3. Growth: How would you support rehashing when the load factor exceeds a threshold? What happens 
   to references and pointers during rehashing?
4. C++ object lifetime: How would you support keys and values with nontrivial constructors, 
   deleted copy constructors, or throwing move constructors?
5. Incremental rehashing: Rehashing the entire map causes latency spikes. How could you spread the
   work across subsequent operations?
6. Concurrency: How would you permit concurrent readers during insertions and rehashing? What synchronization
   and memory reclamation mechanisms are necessary?

Final challenge: Compare your implementation with std::unordered_map in terms of cache locality, 
pointer stability, memory overhead, and tail latency.
*/

