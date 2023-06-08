//
// Created by nasta on 12.04.23.
//

#ifndef COUNTWORDS_PAR_PAR_TRACKER_HPP
#define COUNTWORDS_PAR_PAR_TRACKER_HPP

#include <mutex>

template<typename T>
class par_tracker_t {
    std::mutex mutex;
    T tracked_val;
public:
    void add_value(T time) {
        std::unique_lock<std::mutex> lck(mutex);
        tracked_val += time;
    }

    [[nodiscard]] T get_tracked() {
        std::unique_lock<std::mutex> lck(mutex);
        return tracked_val;
    }
};


#endif //COUNTWORDS_PAR_PAR_TRACKER_HPP
