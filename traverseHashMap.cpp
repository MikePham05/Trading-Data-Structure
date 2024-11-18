#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <memory>
#include <unordered_set>
#include <benchmark/benchmark.h>

using namespace std;

typedef long long ll;

std::random_device rd; // Obtain a random number from hardware
std::mt19937 generator(rd()); // Seed the generator

template <typename K, typename V>
struct HashItem {
    K key;
    V value;
    HashItem<K, V>* next_ptr;

    HashItem(K key, V value): key(key), value(value), next_ptr(nullptr) {};
    HashItem(): key(-1), value(-1), next_ptr(nullptr) {};
};

/*
    This map preserves distinct features of unordered_map:
        avg O(1) insert
        avg O(1) delete
        not preserving the order in which the storin
        no support for resize with max_load_factor yet
        
    Benefits: faster iteration due to clumping bucket together in an array, which increases chances of cacheline
    Drawback: linked-list structure in each bucket causing fragmented memory and under-utilization of cache line
    Use Case: frequent read, especially good when data pattern is repetitive
*/
template <typename K, typename V>
class ConsecutiveHashMap {
    private: 
    vector<HashItem<K, V>*> table; // array storing address to each buckets in sequential manner to increase traversal time
    size_t load_factor;

    public:
    // Default Constructor
    ConsecutiveHashMap() {
        load_factor = 100000;
        table = vector<HashItem<K, V>*>(100000, nullptr);
    }

    public:
    // Constructor with load factor
    ConsecutiveHashMap(size_t n) {
        load_factor = n;
        table = vector<HashItem<K, V>*>(n, nullptr);
    }

    private:
    /*
    get last item of a bucket
    return: null_ptr if there is no item in the bucket
            ptr pointing to the last item in bucket
    */ 
    HashItem<K, V>* getLastItemBucket(size_t index) {
        HashItem<K, V>* item = table[index];
        if (item == nullptr) return nullptr;
        while (item->next_ptr != nullptr) item = item->next_ptr;
        return item;
    }

    size_t getIndex(K key) {
        return hash<K>{}(key) % load_factor;
    }

    public:
    /*
        Insert an item into map
        Params: 
            K Key: key of object
            V Value: value of object
            HashItem<K, V>* address_holder: ptr to allocated space for that object
        Return:
            address_holder
    */
    HashItem<K, V>* insert(K key, V value, HashItem<K, V>* address_holder) {
        size_t index = hash<K>{}(key) % load_factor;
        HashItem<K, V> *last_item_ptr = getLastItemBucket(index);
        if (last_item_ptr == nullptr) {
            table[index] = address_holder;
        } else {
            last_item_ptr->next_ptr = address_holder;
        }
        address_holder->key = key;
        address_holder->value = value;
        return address_holder;
    }

    /*
        Get object in the map with key
        Return :
            V: item with key in the map
            Exception : if item not in map
    */
    public:
    V get(K key) {
        size_t index = hash<K>{}(key) % load_factor;
        HashItem<K, V>* item_ptr = table[index];
        while (item_ptr != nullptr and item_ptr->key != key) item_ptr = item_ptr->next_ptr;
        return item_ptr->value;
    }

    bool check(size_t index) {
        return table[index] != nullptr;
    }

    /*
        Remove object with key in the map.
        NOTE: for perf demo stration purpose, func will not yet be implemented
    */
    void erase(K key) {

    }
};

/*
    Assumption for this data structures:
        - hash function can decently uniformly distribute all items across buckets:
            - Iterator with iterate over the buckets and find all values

*/
string randomStringGenerator(int length) {
    const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string result;
    uniform_int_distribution<size_t> dist(0, characters.size() - 1);

    for (size_t i = 0; i < length; ++i) {
        result += characters[dist(generator)];
    }
    return result;
}


float randomFloatGenerator(float min_range, float max_range) {
    uniform_real_distribution<float> dist(min_range, max_range);
    return dist(generator);
}

static void standard_map(benchmark::State& state) {
    int n = state.range(0);
    unordered_map<long long, long long> d = unordered_map<long long, long long>();
    vector<ll> data = vector<ll>(n);
    uniform_real_distribution<> dis_ll(1e+10, 1e+12);
    for (int i = 0; i < n; i++) {
        long long a = dis_ll(generator);
        d.insert(pair<long long, long long>(a, a));
        data[i] = a;
    }
    for (auto _:state) {
        for (auto e: data)
            d[e]++;
    }
}
BENCHMARK(standard_map)->RangeMultiplier(4)->Range(2<<5, 2<<12);

static void Array_map(benchmark::State& state) {
    /*
        It's important to know before hand the precomputed hash value of your items. If
        you do, you can precompute the hashed_key of all your items, and feed them into 
        the data structures above in a increasing sequential manner (1, 2, 3,.. to size)
        of buckets. Doing this will have a chance to better utilize cacheline, which will
        in produce better performance.
    */
    
    int n = state.range(0);
    // sort data by its precomputed hash

    uniform_real_distribution<> dis_real(1e+10, 1e+12);
    vector<long long> data = vector<long long>(n);
    for (auto& e: data)
        e = dis_real(generator);
    sort(data.begin(), data.end(), [n](long long a, long long b){
        return (hash<long long>{}(a)) % n < hash<long long>{}(b) % n;
    }); 

    //--------------------------------------------------------------------

    // dynamically allocate memory for hashItem object 
    unique_ptr<HashItem<ll, ll>[]> items = make_unique<HashItem<ll, ll>[]>(n);
    HashItem<ll, ll>* item_ptr = items.get();

    //--------------------------------------------------------------------

    ConsecutiveHashMap<ll, ll> sample_map = ConsecutiveHashMap<ll, ll>(n);
    for (auto e: data) {
        sample_map.insert(e, e, item_ptr);
        item_ptr++;
    }
    for (auto _: state) {
        for (auto e: data) {
            ll a = sample_map.get(e);
            a++;
        }
    }
}
BENCHMARK(Array_map)->RangeMultiplier(4)->Range(2<<5, 2<<12);

BENCHMARK_MAIN();