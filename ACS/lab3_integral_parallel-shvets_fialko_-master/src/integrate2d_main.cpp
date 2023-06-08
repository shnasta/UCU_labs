// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <fstream>
#include <cmath>
#include <memory> // For unique_ptr
#include <optional>
#include <functional>
#include <thread>
#include <numeric>

#include "time_measurements.hpp"
#include "integrate2d_conf.hpp"

//#define PRINT_INTERMEDIATE_STEPS
//#define THREADS_DEBUG


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

template<typename func_T>
double integrate_1d(func_T func, double x1, const double& x2, double delta_x)
{
    double res = 0;
    while(x1<x2)
    {
        res += func(x1);
        x1 += delta_x;
    }
    return res * delta_x;
}


//template<typename func_T>
//double integrate_2d(func_T func, const double& x1, const double& x2, double y1, const double& y2,
//                    size_t steps_x, size_t steps_y)
//{
//    double res = 0;
//    double delta_x = (x2 - x1) / static_cast<double>(steps_x);
//    double delta_y = (y2 - y1) / static_cast<double>(steps_y);
//
//    while (y1 < y2) {
//        auto func_y = [&func, &y1](double x) {
//            return func(x, y1);
//        };
//        res += integrate_1d(func_y, x1, x2, delta_x);
//        y1 += delta_y;
//    }
//
//    return res * delta_y;
//}

template<typename func_T>
void integrate_2d(func_T func, const double x1, const double x2, double y1, const double y2,
                    size_t steps_x, size_t steps_y, double& res)
{
    res = 0;
    double delta_x = (x2 - x1) / static_cast<double>(steps_x);
    double delta_y = (y2 - y1) / static_cast<double>(steps_y);

    while (y1 < y2) {
        auto func_y = [&func, &y1](double x) {
            return func(x, y1);
        };
        res += integrate_1d(func_y, x1, x2, delta_x);
        y1 += delta_y;
    }

    res *= delta_y;
}

//template<typename func_T>
//double integrate_2d_with_opt(func_T func, const double& x1, const double& x2, double y1, const double& y2,
//                    size_t steps_x, size_t steps_y, double prev_res)
//{
//    double res = 0;
//    double delta_x = (x2 - x1) / static_cast<double>(steps_x);
//    double delta_y = (y2 - y1) / static_cast<double>(steps_y);
//
//    double prev_delta_x = delta_x * 2;
//
//
//    while (y1 < y2) {
//        auto func_y = [&func, &y1](double x) {
//            return func(x, y1);
//        };
//        res += integrate_1d(func_y, x1 + delta_x, x2, prev_delta_x) / 2;
//        y1 += delta_y;
//
//        res += integrate_1d(func_y, x1, x2, delta_x);
//        y1 += delta_y;
//    }
//
//    return res * delta_y + prev_res / 4;
//}

template<typename func_T>
void integrate_2d_with_opt(func_T func, const double& x1, const double& x2, double y1, const double& y2,
                             size_t steps_x, size_t steps_y, double prev_res, double& res)
{
    res = 0;
    double delta_x = (x2 - x1) / static_cast<double>(steps_x);
    double delta_y = (y2 - y1) / static_cast<double>(steps_y);

    double prev_delta_x = delta_x * 2;


    while (y1 < y2) {
        auto func_y = [&func, &y1](double x) {
            return func(x, y1);
        };
        res += integrate_1d(func_y, x1 + delta_x, x2, prev_delta_x) / 2;
        y1 += delta_y;

        res += integrate_1d(func_y, x1, x2, delta_x);
        y1 += delta_y;
    }

    res = res * delta_y + prev_res / 4;
}


double run_threads(std::vector<std::thread>& threads, std::vector<double>& res, std::function<double(const double&, const double&)> func_to_integrate,
                 const std::unique_ptr<integrate1d_conf>& config_ptr, size_t steps_x, size_t steps_y, int THREADS_NUM) {
    for (int i = 0; i < THREADS_NUM; i++) {
        threads.emplace_back(integrate_2d<std::function<double(const double&, const double&)>>, func_to_integrate,
                             config_ptr->x1 + i * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM,
                             config_ptr->x1 + (i + 1) * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM,
                             config_ptr->y1,
                             config_ptr->y2,
                             steps_x / THREADS_NUM, steps_y, std::ref(res[i]));

#ifdef THREADS_DEBUG
        std::cout << config_ptr->x1 + i * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM << " " <<
                  config_ptr->x1 + (i + 1) * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM << " " <<
                  config_ptr->y1 << " " <<
                  config_ptr->y2 << " " <<
                  steps_x / THREADS_NUM << " " << steps_y / THREADS_NUM << std::endl;
#endif
    }

//    double cur_res = integrate_2d(func_to_integrate, config_ptr->x1, config_ptr->x2, config_ptr->y1, config_ptr->y2, steps_x, steps_y);
    for (int i = 0; i < THREADS_NUM; i++) {
        threads[i].join();
    }
    threads.clear();

#ifdef THREADS_DEBUG
    for (int i = 0; i < THREADS_NUM; i++) {
        std::cout << res[i] << std::endl;
    }
#endif

    return std::reduce(res.begin(), res.end());
}

double run_threads_with_opt(std::vector<std::thread>& threads, std::vector<double>& res, std::vector<double>& prev_res, std::function<double(const double&, const double&)> func_to_integrate,
                 const std::unique_ptr<integrate1d_conf>& config_ptr, size_t steps_x, size_t steps_y, int THREADS_NUM) {
    for (int i = 0; i < THREADS_NUM; i++) {
        threads.emplace_back(integrate_2d_with_opt<std::function<double(const double &, const double &)>>,
                             func_to_integrate,
                             config_ptr->x1 + i * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM,
                             config_ptr->x1 + (i + 1) * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM,
                             config_ptr->y1,
                             config_ptr->y2,
                             steps_x / THREADS_NUM, steps_y, prev_res[i], std::ref(res[i]));
#ifdef THREADS_DEBUG
    std::cout << config_ptr->x1 + i * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM << " " <<
              config_ptr->x1 + (i + 1) * (config_ptr->x2 - config_ptr->x1) / THREADS_NUM << " " <<
              config_ptr->y1 << " " <<
              config_ptr->y2 << " " <<
              steps_x / THREADS_NUM << " " << steps_y << std::endl;
#endif
    }

    for (int i = 0; i < THREADS_NUM; i++) {
        threads[i].join();
    }
    threads.clear();

#ifdef THREADS_DEBUG
    for (int i = 0; i < THREADS_NUM; i++) {
        std::cout << res[i] << std::endl;
    }
#endif

    return std::reduce(res.begin(), res.end());
}



int main(int argc, char* argv[])
{
    std::function<double(const double&, const double&)> funcs[3] = {
            func_to_integrate1, func_to_integrate2, func_to_integrate3};
    std::function<double(const double&, const double&)>  func_to_integrate;

    std::string filename("func1.cfg");
    int FUNC_NUM, THREADS_NUM;

    if(argc == 4) {
        filename = argv[2];
        FUNC_NUM = std::stoi(argv[1]);
        if (FUNC_NUM > 3 || FUNC_NUM < 0) {
            std::cerr << "Invalid function number." << std::endl;
            return 2;
        }
        THREADS_NUM = std::stoi(argv[3]);
        if (THREADS_NUM <= 0) {   // (THREADS_NUM > 8 || THREADS_NUM < 0) {
            std::cerr << "Invalid threads number." << std::endl;
            return 2;
        }
        func_to_integrate = funcs[FUNC_NUM - 1];
    } else {
        std::cerr << "Invalid number of arguments. Usage: \n"
                "<program> <function-number> <config-filename> <threads-number>\n" << std::endl;
        return 1;
    }

#ifdef PRINT_INTERMEDIATE_STEPS
    std::cout << "number_of_threads: " << THREADS_NUM << std::endl;
#endif

    std::ifstream config_stream(filename);
    if(!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 3;
    }

    std::unique_ptr<integrate1d_conf> config_ptr;
    try {
        config_ptr = std::make_unique<integrate1d_conf>(config_stream);
    } catch (std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 5;
    }


    auto before = get_current_time_fenced();

    std::vector<std::thread> threads;
    std::vector<double> prev_results;
    std::vector<double> res(THREADS_NUM);

    size_t steps_x = config_ptr->initial_steps_x;
    size_t steps_y = config_ptr->initial_steps_y;



    double cur_res = run_threads(threads, res, func_to_integrate, config_ptr, steps_x, steps_y, THREADS_NUM);

    double prev_res;
    bool to_continue = true;
    double abs_err = -1; // Just guard value
    double rel_err = -1; // Just guard value
    while( to_continue )
    {
#ifdef PRINT_INTERMEDIATE_STEPS
        std::cout << "Result = " << cur_res << std::endl;
        std::cout << "  steps_x = " << steps_x << ", steps_y: " << steps_y << std::endl;
        std::cout << "  Abs err = " << abs_err << std::endl;
        std::cout << "  rel err = " << rel_err << std::endl;
        std::cout << "  time = " << to_ms(get_current_time_fenced() - before) << std::endl;
#endif

        prev_res = cur_res;
        prev_results = res;
        steps_x *= 2;
        steps_y *= 2;

//        cur_res = run_threads(threads, res, func_to_integrate, config_ptr, steps_x, steps_y, THREADS_NUM);
        cur_res = run_threads_with_opt(threads, res, prev_results, func_to_integrate, config_ptr, steps_x, steps_y, THREADS_NUM);
        abs_err = fabs(cur_res-prev_res);
        rel_err = fabs( (cur_res-prev_res)/cur_res );

        to_continue = ( abs_err > config_ptr->abs_err );
        to_continue = to_continue || ( rel_err > config_ptr->rel_err );
        to_continue = to_continue && (steps_x < config_ptr->max_steps_x);
        to_continue = to_continue && (steps_y < config_ptr->max_steps_y);
    }

    auto time_to_calculate = get_current_time_fenced() - before;

    std::cout << cur_res << std::endl;
    std::cout << abs_err << std::endl;
    std::cout << rel_err << std::endl;
    std::cout << to_ms(time_to_calculate) << std::endl;

    if( ( abs_err > config_ptr->abs_err ) && ( rel_err > config_ptr->rel_err ) ){
        std::cerr << "Requested errors not reached within allowed iterations limit" << std::endl;
        return 16;
    }
    return 0;
}
