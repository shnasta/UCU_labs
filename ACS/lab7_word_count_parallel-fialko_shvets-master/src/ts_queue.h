// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef INTEGRATE_PARALLEL_QUEUE_TS_QUEUE_H
#define INTEGRATE_PARALLEL_QUEUE_TS_QUEUE_H

#include <iostream>
#include <deque>
#include <mutex>
#include <condition_variable>

template<typename T>
class ts_queue {
    std::deque<T> queue;
    std::mutex data_mutex;
    std::mutex pair_pop_mutex;
    std::condition_variable not_empty_cond_var;
    std::condition_variable not_full_cond_var;

    size_t active_index_threads = 0;
    size_t active_merge_threads = 0;

    size_t max_size;
public:
    explicit ts_queue(size_t max_size);
     ~ts_queue();
    ts_queue(const ts_queue&);
    ts_queue& operator=(const ts_queue<T>&) = delete;

    void set_active_index_threads(size_t num_threads);

    void subtract_active_index_threads();
    void inc_active_merge_threads();
    void subtract_active_merge_threads();

    void push(T&& value);
    T pop();
    std::pair<T, T> pop_pair();
    [[nodiscard]] size_t size() const;
};


#include "ts_queue.tpp"
#endif //INTEGRATE_PARALLEL_QUEUE_TS_QUEUE_H
