// used built in mechanism from c++ to build concurrent structures
/*
    #include <mutex>
    (std::lock_guard) MOST RIGID to wrap atomic instructions with lock() - unlock() instruction
    (std:unique_lock) FLEXIBLE locking
*/