#include <iostream>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <future>

using namespace std;

int accum(int *begin, int *end) {
    return accumulate(begin, end, 0);
}

int parallel_sum(vector<int>& a, int num_thread) {
    int s = 0;
    int batch_size = a.size() / num_thread;
    int *current_address = &a[0];

    vector<future<int>> futures(num_thread);

    for (int i = 0; i < num_thread - 1; i++) {
        futures[i] = async(launch::async, accum, current_address, current_address + batch_size);
        current_address = current_address + batch_size;
    }
    futures[num_thread - 1] = async(launch::async, accum, current_address, &a[0] + a.size());
    
    for (int i = 0; i < num_thread; i++)
        s = s + futures[i].get();

    return s;
}

int main() {
    
    // sequential add
    vector<int> a(100000000);
    int se = 0, s = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < a.size(); ++i) {
        a[i]++;
        se += a[i];
    }
    cout << se << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    
    // Compute the duration
    std::chrono::duration<double> sequential_duration = end - start;
    
    //---------------------------------------------
    //reset
    s = 0;
    int num_thread = 2;
    cout << "Select amount of thread you want to run: ";
    cin >> num_thread;
    
    //---------------------------------------------

    // divide workload into n thread
    start = std::chrono::high_resolution_clock::now();
    s = parallel_sum(a, num_thread);
    end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> multi_thread_duration = end - start;

    // Output the duration in seconds
    cout << "Sum: " << se << " Sequential Execution time: " << sequential_duration.count() << " seconds" << std::endl;
    cout << "Sum: " << s << " " << num_thread << "Threads Execution time: " << multi_thread_duration.count() << " seconds" << std::endl;
    
    return 0;
}

// try to do data processing within memory and not in disk
// more simpler is faster. more complex = more durability
// Im-mem databases uses transactional logging (logged operation in disk) for durability
/*
    OLAP for trading system: fast ingestion, quick aggregations, in-memory table (cached), 
    Column-oriented databases optimized for analytics
    OLTP:
    Learn memory management
*/