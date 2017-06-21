#pragma once
#include "run_case.h"
#include <vector>
#include <iostream>
#include <cstdlib>

namespace el {
    class c_expr_generator_runner {
    public:
        int run_case(s_run_case* c) {
            c->run_begin = std::chrono::high_resolution_clock::now();
            std::string cmd("java ExpressionGenerator ");
            //std::string cmd("expr_vczh ");
            cmd.append(std::to_string(c->expr_num));
            cmd.append(" ");
            cmd.append(std::to_string(c->optr_num));
            cmd.append(" ");
            cmd.append(c->file);
            std::cout << "Executing command: \"" << cmd << "\" ...";
            auto ret = system(cmd.c_str());
            std::cout << " DONE with status: " << ret << std::endl;
            c->run_end = std::chrono::high_resolution_clock::now();
            c->status = ret;
            return ret;
        }
    };
}