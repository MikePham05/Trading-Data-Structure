#include <iostream>
#include <chrono>
#include <functional>
#include <vector>

/*
    problem of std:unordered_map: linked-list approach. In efficient when try to access value, in-line cache are not fully utilized.

    general arch:
        vector<(hash, ptr_to_value_list)>
        ptr_to_value_list: linkedList

    [comparing to unordered_map where buckets are random address in memory]
    Pros: fast iteration over map
    Cons: slow remove
    Use case: O(1) access, LOTS of iteration needed thru the map

*/

/* 

Benchmarking: 

the program is run on a Macbook pro
MacBook pro Architecture
    L1 Cache: 64 KB instruction and 64 KB data per core.
    L2 Cache: 256 KB per core.
    L3 Cache: 24 MB (M1 Pro) or 36 MB (M2 Pro) shared among all cores.

Cache line can hold 4 items at a time, reducing possible cache misses.
*/

// use a struct for storing pointer and hash value
struct CustomHashBucket {
    double key;
    int* value_ptr;
};

// custom unordered_map class
class ContiguousUnorderedMap {
private:
    std::vector<CustomHashBucket> v;
    std::hash<double> hash_fn;

public:

    /*
        Choice of initial size extremely important to achieve performance increase
    */
    ContiguousUnorderedMap(int initial_size) : v(initial_size){

    };

    /*
        Description: insert a key, value pair into the map
        Input: double key, double value
        Output: void
    */
    void insert() {
        
    };

    /*
        Description: Get value associated with the key
        Input: int key 
        Output: value associated with that key
    */
    double get() {

    }

    // remove an element in map. NOT PRACTICAL TO IMPLEMENT FOR THIS USE CASE
    void erase() {

    }

};

int main() {
    double a = 1;
    std::hash<double> doubleHash;
    auto v = doubleHash(a);
    std::cout << sizeof(v);

    // create contiguous hash object

    // adding data
    
    // iterate over it

    // deallocate all containers to prevent mem-leak
    
    return 0;
}

