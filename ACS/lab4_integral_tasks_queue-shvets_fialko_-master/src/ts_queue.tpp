#include "ts_queue.h"

#include <mutex>

template<typename T>
ts_queue<T>::ts_queue() = default;
template<typename T>
ts_queue<T>::~ts_queue() = default;
template<typename T>
ts_queue<T>::ts_queue(const ts_queue<T>&) = default;

template<typename T>
void ts_queue<T>::push(const T& value) {
    {
        std::lock_guard<std::mutex> lck(data_mutex);
        queue.push_back(value);
    }
    data_cond_var.notify_one();
}

template<typename T>
T ts_queue<T>::pop() {
    std::unique_lock<std::mutex> lck(data_mutex);
    // wait until queue is not empty
    // m_cond.wait(lock,
    //      [this]() { return !m_queue.empty(); });
    while(queue.empty()) {
        data_cond_var.wait(lck);
    }
    auto res = queue.front();
    queue.pop_front();
    return res;
}

template<typename T>
bool ts_queue<T>::is_empty(){
    return queue.empty();
}
