//
// Created by Sofiia Tesliuk on 2019-05-19.
//

#ifndef INTEGRALS_INTEGRAL_H
#define INTEGRALS_INTEGRAL_H


struct result_t
{
    double abs_err, rel_err, res_value;
};

struct func_params_t
{
    double a, b, c;
};

double function_to_integrate(const func_params_t& params, const double& x, const double& y);


double integrate(const func_params_t& params,
                 const double& x1, const double& x2, double y1, const double& y2, size_t steps);


result_t integrate_interval(const func_params_t& params, const double& x1, const double& x2, double y1, const double& y2, const double& config_rel_err,
                            const double& config_abs_err, size_t initial_steps, size_t max_steps);


void integrate_interval_with_mutex(std::vector<result_t>& res, const func_params_t& params, const double& x1,
const double& x2, double y1, const double& y2, const double& config_rel_err, const double& config_abs_err,
        size_t initial_steps, size_t max_steps);


#endif //INTEGRALS_INTEGRAL_H
