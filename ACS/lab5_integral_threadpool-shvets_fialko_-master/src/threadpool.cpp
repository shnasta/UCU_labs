#include "threadpool.h"


threadpool::threadpool(int num_threads): done(false) {
    try
    {
        for(int i = 0; i < num_threads; ++i)
        {
            threads.emplace_back(&threadpool::worker, this);
        }
    }
    catch(...)
    {
        done = true;
        throw;
    }
}

threadpool::~threadpool() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        done = true;
    }

    cond_var.notify_all();

    for (auto& thread : threads) {
        thread.join();
    }
    threads.clear();
}

void threadpool::worker() {
    while (!done) {
        std::function<void()> task;
        if(tasks.try_pop(task))
        {
            task();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

auto threadpool::submit(const std::function<double()>& func) -> std::future<double>{

    auto task_ptr = std::make_shared<std::packaged_task<double()>>(func);

    // wrap packaged task into void function
    std::function<void()> wrapper_func = [task_ptr]() {
        (*task_ptr)();
    };

    // push generic wrapper function
    tasks.push(wrapper_func);

    // wake up one thread if its waiting
    cond_var.notify_one();

    // return future from promise
    return task_ptr->get_future();
}
