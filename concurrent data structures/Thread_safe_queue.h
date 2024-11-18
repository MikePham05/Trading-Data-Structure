#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>
#include <iostream>

template <typename T>
class Thread_safe_queue
{
private:
    mutable std::mutex mut;  
    std::queue<std::shared_ptr<T>> data_queue;  
    std::condition_variable data_cond;

public:
    Thread_safe_queue<T>() {
        
    };

    void push(T data_) {
        using namespace std;
        shared_ptr<T> data = make_shared<T>(move(data_));
        lock_guard<mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }

    T wait_and_pop() {
        using namespace std;
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk, [this]{return !data_queue.empty();});
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res.get();
    }
};


