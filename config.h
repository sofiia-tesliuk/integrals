//
// Created by Sofiia Tesliuk on 2019-05-19.
//

#ifndef INTEGRALS_CONFIG_H
#define INTEGRALS_CONFIG_H

struct configuration_t
{
    double abs_err, rel_err;
    int num_threads;
    double a, b, c;
    double x1, x2;
    double y1, y2;
    size_t initial_steps;
    size_t max_steps;
};

configuration_t read_conf(std::istream& cf);

#endif //INTEGRALS_CONFIG_H
