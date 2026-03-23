#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue {
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv_pop;
    std::condition_variable cv_push;
    bool done = false;
    size_t max_size;

public:
    // push new packet to queue and wake 1 thread
    SafeQueue(size_t max_size = 200) : max_size(max_size) {}

    void push(T item) {
    std::unique_lock<std::mutex> lock(mtx);
    
    cv_push.wait(lock, [this]() {return q.size() < max_size; });

    q.push(std::move(item));
    cv_pop.notify_one();
    }

    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait until there is data in queue or done = true
        cv_pop.wait(lock, [this]() { return !q.empty() || done; });

        if (q.empty() && done) {
            return false;
        }

        item = std::move(q.front());
        q.pop();

        cv_push.notify_one();

        return true;
    }

    void set_done() {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
        cv_pop.notify_all();
    }
};

