/*

Problem 4 — Out-of-Order Message Buffer
Ring buffers · Sequence numbers · Atomics · Low latency
A network feed delivers messages with monotonically increasing sequence numbers, but messages may arrive out of order.

1. Basic design: Design a buffer that accepts (sequence_number, message) and emits messages in sequence order. 
   How do you detect duplicates and gaps?
2. Bounded memory: You can only buffer the next N sequence numbers. How would you implement this with a 
   circular array and O(1) insertion and lookup?
3. Sequence wraparound: Sequence numbers are unsigned 32-bit integers. How would you correctly compare them 
   when they wrap around?
4. Missing messages: If sequence 102 never arrives, how can you avoid blocking messages 103 onward indefinitely? 
   How would you support explicit gap skipping?
5. Concurrency: Multiple network threads insert messages concurrently, while one consumer emits messages. 
   What ordering guarantees must hold before a slot is considered ready?
6. Memory safety: Suppose messages have variable sizes and are allocated separately. 
   When can their storage safely be reused?

Final challenge: Can you make the producer side lock-free while ensuring the consumer never reads a partially published message?

*/