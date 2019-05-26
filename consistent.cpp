#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <limits>

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

    func_params_t f_params = {config.a, config.b, config.c};

    auto before = get_current_time_fenced();

    result_t res = integrate_interval(f_params, config.x1, config.x2, config.y1, config.y2,
            config.rel_err, config.abs_err, config.initial_steps, config.max_steps);

    auto time_to_calculate = get_current_time_fenced() - before;

    std::cout << "Result: " << res.res_value << std::endl;
    std::cout << "Abs err : rel err " << res.abs_err << " : " << res.rel_err << std::endl;
    std::cout << "Time: " << to_us(time_to_calculate)  << std::endl;

    return 0;
}