
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
    std::condition_variable data_cond_var;
public:
    ts_queue();
    ~ts_queue();
    ts_queue(const ts_queue&);
    ts_queue& operator=(const ts_queue<T>&) = delete;

    void push(const T& value);
    T pop();
    bool is_empty();
};

#include "ts_queue.tpp"
#endif //INTEGRATE_PARALLEL_QUEUE_TS_QUEUE_H
