#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <thread>
#include <chrono>

using namespace std;

/*
    - THREAD: thread t(lambda, class method, or function);
    - JOINABILITY:
        all safe threads must JOIN or DETACH, else compiler fails
        JOIN: create a point where PARENT THREAD cannot execute UNTIL CHILD THREAD finished execution
        DETACH: PARENT THREAD and CHILD THREAD execute independently
    - PASS BY REFERENCE TO A THREAD
        NOTE: a deached thread might try to access a variable passed to it by reference after when that variable is cleaned up the moment the
        parent thread's destuctor is called and memory address of that variable is no longer valid.
    - MUTEX: 
        lock_guard<mutex> lock<mutex> = lock() + unlock() combo
        unique_lock<mutex> ulock(mutex, types_of_lock): 
            - useful when need to lock multiple lock in same order
            - prevent deadlock

        Dining Philosopher: lock both 2 fork at the same time (Solution = unique lock)
        LiveLock: Programs blocked, threads inactive
        DeadLock: Programs blocked, threads constant active
    - Concurrency vs Parallelism 
*/


void pass_to_thread_by_reference(int& x) {
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(1000));
        x++;
        printf("Current value of x: %d", x);
    }
}

int main1() {
    thread t0(f);
    t0.join();

    int x = 0;
    thread pass_by_reference_thread(pass_to_thread_by_reference, ref(x)); // passing a variable by reference

    cout << "hello from main \n";
    return 0;
}