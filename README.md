- This repo contains implementations of different data structures that are relevant to building a trading systems.
These data structures are designed to be used in a specific task in a system. It is important to keep in mind that there
are no one-size-fit-all solution for every tasks in design software for any system. A data structure might be optimized to run
as fast as possible, while others strive for a balance between performance and memory usuage.

- Some important numbers:
    - Cacheline = 64 bytes (64-bits arch)
    - L1 cache: 64kb (1ns -> 2ns)
    - L2 cache
    - L3 cache


- List of concepts and demonstration:
    - Traverse HashMap: Optimized for traversal. 
    - Key Optimization point: Buckets are stores contiguously where each element contains
    Hashed key and address to the linkedlist of element
    - Use case:
        - Keys are not successive
        - Often needs traversing the structures (Ex: summing)

- Lock design:
    - Correctness
    - Fairness
    - Performance (resource management)
    
        - Time
        - Memory
- SEQ-lock: few producers, many consumers pattern (optimistic locking)
    - pattern:
        - producer:
            version v
            v++;
            write data;
            v++
        - consumer:
            if v % 2 == 1:
                return
            read data;
    - trade-off:
        - not time sensitive
        
    
