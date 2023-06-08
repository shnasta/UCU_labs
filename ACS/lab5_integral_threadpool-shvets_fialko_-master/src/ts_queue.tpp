#include "ts_queue.h"

#include <mutex>
#include <future>

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
    while(queue.empty()) {
        data_cond_var.wait(lck);
    }
    auto res = queue.front();
    queue.pop_front();
    return res;
}

template<typename T>
bool ts_queue<T>::try_pop(T& value)
{
    {
        std::lock_guard<std::mutex> lck(data_mutex);
        if (queue.empty()) {
            return false;
        }
        value = std::move(queue.front());
        queue.pop_front();
    }
    return true;
}


template<typename T>
bool ts_queue<T>::is_empty(){
    return queue.empty();
}
