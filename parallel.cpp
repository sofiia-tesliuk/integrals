//
// Created by Sofiia Tesliuk on 2019-05-20.
//

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <limits>
#include <thread>
#include <vector>

#include "time_measure.h"
#include "config.h"
#include "integral.h"


int main(int argc, char* argv[]) {

    std::string filename;

    if (argc >= 2){
        filename = argv[1];
    } else{
        std::cerr << "Too few arguments." << std::endl;
        return 1;
    }

    std::ifstream config_stream(filename);
    if(!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 2;
    }

    configuration_t config;
    try{
        config = read_conf(config_stream);
    }catch (std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 3;
    }


    double delta_x = (config.x2 - config.x1) / config.num_threads;
    double x_down, x_up;

    std::vector<std::thread> threads;
    std::vector<result_t> results_parts;

    func_params_t f_params = {config.a, config.b, config.c};

    double new_abs = config.abs_err/config.num_threads;
    size_t new_initial_steps = config.initial_steps / config.num_threads;
    size_t new_max_steps = config.max_steps / config.num_threads;

    for(int i = 0; i < config.num_threads; i++){
        x_down = config.x1 + i * delta_x;
        x_up = config.x1 + (i + 1) * delta_x;
        threads.emplace_back(integrate_interval_with_mutex, &results_parts, f_params, x_down, x_up,
                config.y1, config.y2, config.rel_err, new_abs, new_initial_steps,
                new_max_steps);
    }

    auto before = get_current_time_fenced();

    for (auto &current_thread: threads){
        current_thread.join();
    }

    auto time_to_calculate = get_current_time_fenced() - before;

    result_t final_result;
    final_result.abs_err = 0.0;
    final_result.rel_err = 0.0;
    final_result.res_value = 0.0;

    for (auto current_result: results_parts){
        final_result.res_value += current_result.res_value;
        final_result.abs_err += current_result.abs_err;
        final_result.rel_err += current_result.rel_err * current_result.res_value;
    }

    final_result.rel_err /= final_result.res_value;

    std::cout << "Result: " << final_result.res_value << std::endl;
    std::cout << "Abs err : rel err | " << final_result.abs_err << " : " << final_result.rel_err << std::endl;
    std::cout << "Time: " << to_us(time_to_calculate)  << std::endl;

    return 0;
}