/*

Problem 7 — Concurrent Append-Only Log with Truncation
Indexing · Segmented arrays · Atomics · Reclamation
Design an in-memory log that supports appending records, reading by logical sequence number, and deleting old records.

1. Basic design: Implement append(record) and get(sequence_number) in O(1), with monotonically increasing sequence numbers.
2. Front truncation: Records older than a given sequence number can be removed. How would you avoid 
   shifting all remaining records?
3. Storage growth: A single contiguous array becomes too expensive to resize. How would you use fixed-size memory 
   segments while maintaining O(1) random access?
4. Stable references: Clients may hold references to records even as older records are truncated. How would you 
   guarantee their validity without preventing all reclamation?
5. Concurrency: Multiple producers append concurrently, and readers access records without locking. How would you 
   distinguish reserved sequence numbers from fully published records?
6. Memory reclamation: When an entire segment is truncated, how can you determine whether it is safe to free or reuse,
   without stopping concurrent readers?

Final challenge: How would you design this log for a single writer and many readers, then change the design to support 
multiple writers? What performance and complexity trade-offs arise?

*/