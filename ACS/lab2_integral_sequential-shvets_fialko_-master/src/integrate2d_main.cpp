// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <fstream>
#include <cmath>
#include <memory> // For unique_ptr
#include <optional>
#include <functional>

#include "time_measurements.hpp"
#include "integrate2d_conf.hpp"


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


template<typename func_T>
double integrate_2d(func_T func, const double& x1, const double& x2, double y1, const double& y2,
                    size_t steps_x, size_t steps_y)
{
    double res = 0;
    double delta_x = (x2 - x1) / static_cast<double>(steps_x);
    double delta_y = (y2 - y1) / static_cast<double>(steps_y);

    while (y1 < y2) {
        auto func_y = [&func, &y1](double x) {
            return func(x, y1);
        };
        res += integrate_1d(func_y, x1, x2, delta_x);
        y1 += delta_y;
    }

    return res * delta_y;
}

template<typename func_T>
double integrate_2d_with_opt(func_T func, const double& x1, const double& x2, double y1, const double& y2,
                    size_t steps_x, size_t steps_y, double prev_res)
{
    double res = 0;
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

    return res * delta_y + prev_res / 4;
}



int main(int argc, char* argv[])
{
    std::function<double(const double&, const double&)> funcs[3] = {
            func_to_integrate1, func_to_integrate2, func_to_integrate3};
    std::function<double(const double&, const double&)>  func_to_integrate;

    std::string filename("func1.cfg");
    int func_num;
    if(argc == 3) {
        filename = argv[2];
        func_num = std::stoi(argv[1]);
        if (func_num > 3 || func_num < 0) {
            std::cerr << "Invalid function number." << std::endl;
            return 2;
        }
        func_to_integrate = funcs[func_num - 1];
    }
    if(argc > 3) {
        std::cerr << "Too many arguments. Usage: \n"
                "<program>\n"
                "or\n"
                "<program> <config-filename>\n" << std::endl;
        return 1;
    }

    std::ifstream config_stream(filename);
    if(!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 3;
    }

    std::unique_ptr<integrate1d_conf> config_ptr; // Вказівник -- щоб забезпечити потрібну оброку помилок (return 3 тут),
                                                  // але не загортати в try-catch все тіло програми нижче.
                                                  // А розумний -- щоб потім не звільняти.
    try {
        config_ptr = std::make_unique<integrate1d_conf>(config_stream);
    } catch (std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 5;
    }

    auto before = get_current_time_fenced();

    size_t steps_x = config_ptr->initial_steps_x;
    size_t steps_y = config_ptr->initial_steps_y;
    double cur_res = integrate_2d(func_to_integrate, config_ptr->x1, config_ptr->x2, config_ptr->y1, config_ptr->y2, steps_x, steps_y);
    double prev_res = cur_res;
    bool to_continue = true;
    double abs_err = -1; // Just guard value
    double rel_err = -1; // Just guard value
    while( to_continue )
    {
#ifdef PRINT_INTERMEDIATE_STEPS
        std::cout << cur_res << " : " << steps_x << " steps_x " << steps_y << " steps_y" << std::endl;
#endif
        prev_res = cur_res;
        steps_x *= 2;
        steps_y *= 2;
        cur_res = integrate_2d_with_opt(func_to_integrate, config_ptr->x1, config_ptr->x2, config_ptr->y1, config_ptr->y2, steps_x, steps_y, prev_res);
        abs_err = fabs(cur_res-prev_res);
        rel_err = fabs( (cur_res-prev_res)/cur_res );
#ifdef PRINT_INTERMEDIATE_STEPS
        std::cout << '\t' << "Abs err : rel err " << abs_err << " : " << rel_err << std::endl;
#endif

        to_continue = ( abs_err > config_ptr->abs_err );
        to_continue = to_continue || ( rel_err > config_ptr->rel_err );
        to_continue = to_continue && (steps_x < config_ptr->max_steps_x);
        to_continue = to_continue && (steps_y < config_ptr->max_steps_y);
    }

    auto time_to_calculate = get_current_time_fenced() - before;

#ifdef PRINT_INTERMEDIATE_STEPS
    std::cout << "Result = " << cur_res << std::endl;
    std::cout << "Abs err = " << abs_err << std::endl;
    std::cout << "rel err = " << rel_err << std::endl;
    std::cout << "Time: " << to_us(time_to_calculate)  << std::endl;
#else
    std::cout << cur_res << std::endl;
    std::cout << abs_err << std::endl;
    std::cout << rel_err << std::endl;
    std::cout << to_ms(time_to_calculate) << std::endl;
#endif
    if( ( abs_err > config_ptr->abs_err ) && ( rel_err > config_ptr->rel_err ) ){
        std::cerr << "Requested errors not reached within allowed iterations limit" << std::endl;
        return 16;
    }
    return 0;
}
