#include <iostream>
#include <fstream>
#include <cmath>
#include <optional>
#include <functional>
#include <thread>
#include <utility>
#include <climits>

#include "time_measurements.hpp"
#include "integrate2d_conf.hpp"
#include "ts_queue.h"
#include "threadpool.h"

//#define PRINT_INTERMEDIATE_STEPS

double func_to_integrate1(const double& x, const double& y) {
    double result = 0;

    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            result += 1/( 5*(i+2) + j + 3 + std::pow(x - 16*j, 6) + std::pow(y - 16*i, 6) );
        }
    }

    return 1 / (0.002 + result);
}

double func_to_integrate2(const double& x, const double& y) {

    double trig_sum = std::cos(2 * M_PI * x) + std::cos(2 * M_PI * y);
    double var_sum = x * x + y * y;

    return -20 * std::exp(-0.2 * std::sqrt(0.5 * var_sum)) - std::exp(0.5 * trig_sum) + 20 + M_E;
}

double func_to_integrate3(const double& x, const double& y) {
    double result = 0;
    double a1[5] = {1, 2, 1, 1, 5};
    double a2[5] = {4, 5, 1, 2, 4};
    double c[5] = {2, 1, 4, 7, 2};

    for (int i = 0; i < 5; ++i) {
        double var_sum = std::pow(x - a1[i], 2) + std::pow(y - a2[i], 2);
        result -= c[i] * std::exp(-(1/M_PI) * var_sum) * std::cos(M_PI * var_sum);
    }

    return result;
}




int main(int argc, char *argv[]) {
    std::function<double(const double &, const double &)> funcs[3] = {
            func_to_integrate1, func_to_integrate2, func_to_integrate3};
    std::function<double(const double &, const double &)> func_to_integrate;

    std::string filename("func1.cfg");
    int FUNC_NUM, THREADS_NUM, POINTS_PER_TASK;

    if (argc == 5) {
        filename = argv[2];
        FUNC_NUM = std::stoi(argv[1]);
        if (FUNC_NUM > 3 || FUNC_NUM < 0) {
            std::cerr << "Invalid function number." << std::endl;
            return 2;
        }
        THREADS_NUM = std::stoi(argv[3]);
        if (THREADS_NUM <= 0) {
            std::cerr << "Invalid threads number." << std::endl;
            return 64;
        }
        POINTS_PER_TASK = std::stoi(argv[4]);
        if (POINTS_PER_TASK <= 0) {
            std::cerr << "Invalid points per task number." << std::endl;
            return 65;
        }
        func_to_integrate = funcs[FUNC_NUM - 1];
    } else {
        std::cerr << "Invalid number of arguments. Usage: \n"
                     "<program> <function-number> <config-filename> <threads-number> <points-per-task>\n"
                  << std::endl;
        return 1;
    }

#ifdef PRINT_INTERMEDIATE_STEPS
        std::cout << "number_of_threads: " << THREADS_NUM << std::endl;
#endif

    std::ifstream config_stream(filename);
    if (!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 3;
    }

    std::unique_ptr<integrate2d_conf> config_ptr;
    try {
        config_ptr = std::make_unique<integrate2d_conf>(config_stream);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 5;
    }


    auto before = get_current_time_fenced();

    std::vector<std::future<double>> results;
    double cur_res = 0;

    size_t steps_x = config_ptr->initial_steps_x;
    size_t steps_y = config_ptr->initial_steps_y;
    double delta_x = (config_ptr->x2 - config_ptr->x1) / static_cast<double>(steps_x);
    double delta_y = (config_ptr->y2 - config_ptr->y1) / static_cast<double>(steps_y);
    double delta_xy = delta_x * delta_y;

    threadpool pool(THREADS_NUM);

    double x = config_ptr->x1;
    double y;
    std::vector<std::pair<double, double>> points_per_task;
    int count_points = 0;
    for (size_t i = 0; i < steps_x; ++i) {
        y = config_ptr->y1;
        for (size_t j = 0; j < steps_y; ++j) {
//           check if we have enough points to complete task,
//           if so, submit task to threadpool,
//           and clear points_per_task vector for the next task
            if (count_points == POINTS_PER_TASK) {
                results.emplace_back(pool.submit([func_to_integrate, points_per_task]() {
                    double result = 0;
                    for (auto &point : points_per_task) {
                        result += func_to_integrate(point.first, point.second);
                    }
                    return result;
                }));
                points_per_task.clear();
                count_points = 0;
            }
            points_per_task.emplace_back(x, y);
            ++count_points;
            y += delta_y;
        }
        x += delta_x;
    }
//   if we have some points left, submit task to threadpool
    if (count_points != 0) {
        results.emplace_back(pool.submit([func_to_integrate, points_per_task]() {
            double result = 0;
            for (auto &point : points_per_task) {
                result += func_to_integrate(point.first, point.second);
            }
            return result;
        }));
        points_per_task.clear();
        count_points = 0;
    }

//   wait for all tasks to complete and sum up results
    for (auto &result : results) {
        result.wait();
        cur_res += result.get();
    }
    cur_res *= delta_xy;
    results.clear();


    double prev_res;
    bool to_continue = true;
    double abs_err = -1;
    double rel_err = -1;
    while (to_continue) {
#ifdef PRINT_INTERMEDIATE_STEPS
        std::cout << "Result = " << cur_res << std::endl;
        std::cout << "  steps_x = " << steps_x << ", steps_y: " << steps_y << std::endl;
        std::cout << "  Abs err = " << abs_err << std::endl;
        std::cout << "  rel err = " << rel_err << std::endl;
        std::cout << "  time = " << to_ms(get_current_time_fenced() - before) << std::endl;
#endif

        prev_res = cur_res;
        steps_x *= 2;
        steps_y *= 2;
        delta_x /= 2;
        delta_y /= 2;
        delta_xy /= 4;
        cur_res = 0;

//      calculating with simple optimization
        x = config_ptr->x1;
        while (x < config_ptr->x2) {
            y = config_ptr->y1 + delta_y;
            while (y < config_ptr->y2) {
                if (count_points == POINTS_PER_TASK) {
                    results.emplace_back(pool.submit([func_to_integrate, points_per_task]() {
                        double result = 0;
                        for (auto &point : points_per_task) {
                            result += func_to_integrate(point.first, point.second);
                        }
                        return result;
                    }));
                    points_per_task.clear();
                    count_points = 0;
                }
                points_per_task.emplace_back(x, y);
                ++count_points;
                y += delta_y * 2;
            }
            x += delta_x;

            y = config_ptr->y1;
            while (y < config_ptr->y2) {
                if (count_points == POINTS_PER_TASK) {
                    results.emplace_back(pool.submit([func_to_integrate, points_per_task]() {
                        double result = 0;
                        for (auto &point : points_per_task) {
                            result += func_to_integrate(point.first, point.second);
                        }
                        return result;
                    }));
                    points_per_task.clear();
                    count_points = 0;
                }
                points_per_task.emplace_back(x, y);
                ++count_points;
                y += delta_y;
            }
            x += delta_x;
        }

//        including points without optimization (just like on the first calculation, before while loop)
//        x = config_ptr->x1;
//        points_per_task.clear();
//        count_points = 0;
//        for (size_t i = 0; i < steps_x; ++i) {
//            y = config_ptr->y1;
//            for (size_t j = 0; j < steps_y; ++j) {
//                if (count_points == POINTS_PER_TASK) {
//                    results.emplace_back(pool.submit([func_to_integrate, points_per_task]() {
//                        double result = 0;
//                        for (auto &point : points_per_task) {
//                            result += func_to_integrate(point.first, point.second);
//                        }
//                        return result;
//                    }));
//                    points_per_task.clear();
//                    count_points = 0;
//                }
//                points_per_task.emplace_back(x, y);
//                ++count_points;
//                y += delta_y;
//            }
//            x += delta_x;
//        }


//       if we have some points left, submit task to threadpool
        if (count_points != 0) {
            results.emplace_back(pool.submit([func_to_integrate, points_per_task]() {
                double result = 0;
                for (auto &point : points_per_task) {
                    result += func_to_integrate(point.first, point.second);
                }
                return result;
            }));
            points_per_task.clear();
            count_points = 0;
        }

//       wait for all tasks to complete and sum up results
        for (auto &result : results) {
            result.wait();
            cur_res += result.get();
        }
        cur_res *= delta_xy;
//       because we used optimization, we need to add points that we didn't count
        cur_res += prev_res/4;
        results.clear();


        abs_err = fabs(cur_res - prev_res);
        rel_err = fabs((cur_res - prev_res) / cur_res);

        to_continue = (abs_err > config_ptr->abs_err);
        to_continue = to_continue || (rel_err > config_ptr->rel_err);
        to_continue = to_continue && (steps_x < config_ptr->max_steps_x);
        to_continue = to_continue && (steps_y < config_ptr->max_steps_y);
    }
    auto time_to_calculate = get_current_time_fenced() - before;

    std::cout << cur_res << std::endl;
    std::cout << abs_err << std::endl;
    std::cout << rel_err << std::endl;
    std::cout << to_ms(time_to_calculate) << std::endl;

    if ((abs_err > config_ptr->abs_err) && (rel_err > config_ptr->rel_err)) {
        std::cerr << "Requested errors not reached within allowed iterations limit" << std::endl;
        return 16;
    }
    return 0;
}