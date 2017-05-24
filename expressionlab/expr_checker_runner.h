#pragma once
#include "run_case.h"
#include "expr_dup_checker.h"
#include <iostream>
#include <fstream>

namespace el {
    class c_expr_checker_runner {
    public:
        void check_case(s_run_case* c) {
            c_expr_dup_checker checker;

            // load expressions
            std::cout << "now checking your result int " << c->file << " ...";
            std::ifstream infile(c->file);
            std::string line;

            c->check_begin = std::chrono::high_resolution_clock::now();

            int i = 0;
            while (std::getline(infile, line)) {
                checker.add_expr(line, ++i);
            }
            infile.close();

            c->check_end = std::chrono::high_resolution_clock::now();
            c->total_expr_num = checker.m_total_count;
            c->dup_expr_num = checker.m_dup_count;
            c->details = checker.dump_statistics();
            std::cout << " DONE" << std::endl;
        }
    };
}
