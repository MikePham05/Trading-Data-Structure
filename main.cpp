#include <iostream>
#include "concurrent data structures/ThreadSafeQueue.h"
#include <boost/lockfree/queue.hpp>
#include <thread>
#include <vector>
#include <future>

/*
    <future> header:
        async [function template]: one-off, non-blocking event
        packaged_task [class template]: wrap callable (function, lambda) to be run
        promise [class template]: provide mechanism to set future value before future.get() is called
        future [class template]: store asynchronus return value from classes above
        
    <thread> cond-var: multiple synchronization
        wait(lk, []{condition})
        notify_one()
        notify_all()

    <mutex> mutex: mutual exclusion
        lock_guard(): RAII lock
        unique_lock(): usually used with cond-var to lock release the lock again if check = false
*/

/*
        benchmark queue from Anthony Williams
        Create a queue
        1 producers thread
        Multiple consumer threads:
            -> read only -> benchmark avg time for certain calculations
        
        multiple producer threads
        multiple consumer threads
            -> pop queue -> bench mark how fine-grained lock is
                         -> avg time spent in consumer function
*/

void adding_queue(boost::lockfree::queue<int>& q, int pos) {
    for (int i = 0; i < 30; i++)
        q.push(pos + i);
}

void push_williams_q(ThreadSafeQueue<int>& q) {
    for (int i = 0; i < 1000000; i++)
        q.push(1);
}

void pop_williams_q(ThreadSafeQueue<int>& q, int& s) {
    int temp = 0, c = 0, cnt = 0;
    while (c < 1000000) {
        temp = 0;
        q.try_pop(temp);
        cnt += static_cast<int>(temp == 0);
        c += temp;
    }
    s += c;
    std::cout << "cnt: " << cnt << std::endl;
}

int main() {
    boost::lockfree::queue<int> q = boost::lockfree::queue<int>(100);
    std::vector<std::vector<int>> data = std::vector<std::vector<int>>(3, std::vector<int>(30, 0));
    std::thread t1(adding_queue, std::ref(q), 0);
    std::thread t2(adding_queue, std::ref(q), 30);
    std::thread t3(adding_queue, std::ref(q), 60);
    t1.join();
    t2.join();
    t3.join();

    int c = 0;
    while (q.pop(c)) std::cout << c << " "; 

    // simple williams q use case

    ThreadSafeQueue<int> williams_q = ThreadSafeQueue<int>();
    int s = 0;
    std::thread producer1(push_williams_q, std::ref(williams_q));
    std::thread producer2(push_williams_q, std::ref(williams_q));
    std::thread consumer1(pop_williams_q, std::ref(williams_q), std::ref(s));
    std::thread consumer2(pop_williams_q, std::ref(williams_q), std::ref(s));
    consumer1.join();
    consumer2.join();
    producer1.join();
    producer2.join();
    

    std::cout << "S value: " << s << std::endl;
    return 0;
}