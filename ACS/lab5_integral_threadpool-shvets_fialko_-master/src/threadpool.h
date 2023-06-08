#ifndef INTEGRATE_PARALLEL_QUEUE_THREADPOOL_H
#define INTEGRATE_PARALLEL_QUEUE_THREADPOOL_H

#include <thread>
#include <vector>
#include "ts_queue.h"
#include <mutex>
#include <condition_variable>
#include <future>


class threadpool {
    std::vector<std::thread> threads;
    ts_queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable cond_var;

    bool done;

    void worker();

public:
    explicit threadpool(int num_threads);
    ~threadpool();

    auto submit(const std::function<double()>& func) -> std::future<double>;
};

#endif //INTEGRATE_PARALLEL_QUEUE_THREADPOOL_H
