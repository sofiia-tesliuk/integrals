//
// Created by Sofiia Tesliuk on 2019-05-19.
//

#include <iostream>
#include <mutex>
#include <cmath>
#include <vector>

#include "integral.h"


std::mutex mutex;

double function_to_integrate(const func_params_t& params, const double& x, const double& y){
    return -params.a * exp(-params.b * sqrt(0.5 * (x*x + y*y))) - exp(0.5 * (cos(params.c*x) +
    cos(params.c*y))) + params.a + exp(1);
}


double integrate(const func_params_t& params,
                const double& x1, const double& x2, double y1, const double& y2, size_t steps)
{
    double res = 0;
    double delta_x = (x2 - x1)/steps;
    double delta_y = (y2 - y1)/steps;
    double cur_x1;
    while(y1 < y2) {
        cur_x1 = x1;
        while (cur_x1 < x2) {
            res += function_to_integrate(params, cur_x1, y1) * delta_x * delta_y;
            cur_x1 += delta_x;
        }
        y1 += delta_y;
    }
    return res;
}


result_t integrate_interval(const func_params_t& params, const double& x1, const double& x2, double y1, const double& y2, const double& config_rel_err,
                            const double& config_abs_err, size_t initial_steps, size_t max_steps){
    size_t steps = initial_steps;
    double cur_res = integrate(params, x1, x2, y1, y2, steps);
    double prev_res;
    bool to_continue = true;
    double abs_err = -1; // Just guard value
    double rel_err = -1; // Just guard value

    while( to_continue )
    {

        prev_res = cur_res;
        steps *= 2;
        cur_res = integrate(params, x1, x2, y1, y2, steps);
        abs_err = fabs(cur_res-prev_res);
        rel_err = fabs( (cur_res-prev_res)/cur_res );

        to_continue = ( abs_err > config_abs_err );
        to_continue = to_continue && ( rel_err > config_rel_err );
        to_continue = to_continue && (steps < max_steps);
    }

    result_t result;
    result.res_value = cur_res;
    result.abs_err = abs_err;
    result.rel_err = rel_err;

    return result;
}

void integrate_interval_with_mutex(std::vector<result_t>& res, const func_params_t& params, const double& x1,
        const double& x2, double y1, const double& y2, const double& config_rel_err, const double& config_abs_err,
        size_t initial_steps, size_t max_steps){
    result_t cur_res = integrate_interval(params, x1, x2, y1, y2, config_rel_err, config_abs_err, initial_steps, max_steps);

    mutex.lock();
    res.push_back(cur_res);
    mutex.unlock();
}

