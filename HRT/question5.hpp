/*

Problem 3 — A Cache with O(1) Eviction

Design a fixed-capacity LRU cache supporting get(key) and put(key, value) in O(1) average time.
1. Basic design: How would you combine a hash table and a doubly linked list to achieve O(1) operations?
2. No allocations: After initialization, cache operations must not allocate memory. 
   How would you redesign the cache?
3. Intrusive structures: Implement an intrusive linked list where each cached object contains its own linkage. 
   What are the ownership and lifetime implications?
4. Exception safety: What happens if constructing a new value throws during put()? How do you preserve the cache's invariants?
5. Concurrency: Multiple readers may call get() simultaneously, but get() also changes the LRU ordering. 
   How would you support concurrent access efficiently?
6. Memory reclamation: Suppose get() returns a pointer to the cached object. 
   How can the object remain valid if another thread evicts it?
Final challenge: Design a sharded cache that minimizes contention while maintaining reasonable eviction behavior.

*/