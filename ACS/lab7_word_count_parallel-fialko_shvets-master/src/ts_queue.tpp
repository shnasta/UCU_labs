// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ts_queue.h"

#include <mutex>

template<typename T>
ts_queue<T>::ts_queue(size_t max_size) : max_size(max_size) {};
template<typename T>
ts_queue<T>::~ts_queue() = default;
template<typename T>
ts_queue<T>::ts_queue(const ts_queue<T>&) = default;

template<typename T>
void ts_queue<T>::push(T&& value) {
    {
        std::unique_lock<std::mutex> lck(data_mutex);
        not_full_cond_var.wait(lck, [this] { return queue.size() < max_size; });
        queue.emplace_back(std::forward<T>(value));
    }
    not_empty_cond_var.notify_all();
}


template<typename T>
T ts_queue<T>::pop() {
    T item;
    {
        std::unique_lock<std::mutex> lck(data_mutex);
        not_empty_cond_var.wait(lck, [this] { return !queue.empty(); });
        item = std::move(queue.front());
        queue.pop_front();
    }
    not_full_cond_var.notify_all();
    return item;
}

template<typename T>
std::pair<T, T> ts_queue<T>::pop_pair() {
    std::pair<T, T> item;
    {
        std::unique_lock<std::mutex> lck(pair_pop_mutex);
        not_empty_cond_var.wait(lck, [this] { return queue.size() >= 2 || (active_merge_threads == 0 && active_index_threads == 0); });
        {
            std::unique_lock<std::mutex> lck1(data_mutex);
            if (!queue.empty()) {
                item.first = std::move(queue.front());
                queue.pop_front();
                if (!queue.empty()) {
                    item.second = std::move(queue.front());
                    queue.pop_front();
                } else {
                    item.second = T();
                }
            } else {
                item = {{},{}};
            }
        }
        not_full_cond_var.notify_all();
        not_empty_cond_var.notify_all();

    }
    return item;
}

template<typename T>
size_t ts_queue<T>::size() const {
    return queue.size();
}

template<typename T>
void ts_queue<T>::set_active_index_threads(size_t num_threads) {
    active_index_threads = num_threads;
}


template<typename T>
void ts_queue<T>::subtract_active_index_threads() {
    {
        std::lock_guard<std::mutex> lck(data_mutex);
        --active_index_threads;
    }
    not_empty_cond_var.notify_all();
}

template<typename T>
void ts_queue<T>::inc_active_merge_threads() {
    std::lock_guard<std::mutex> lck(data_mutex);
    ++active_merge_threads;
}

template<typename T>
void ts_queue<T>::subtract_active_merge_threads() {
    {
        std::lock_guard<std::mutex> lck(data_mutex);
        --active_merge_threads;
    }
    not_empty_cond_var.notify_all();
}

