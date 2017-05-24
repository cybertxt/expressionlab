#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>

namespace el {
    struct s_run_case {
        // in
        int expr_num;
        int optr_num;
        std::string file;

        // out
        int status;

        // std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        std::chrono::time_point<std::chrono::high_resolution_clock>
            run_begin,
            run_end;

        std::chrono::time_point<std::chrono::high_resolution_clock>
            check_begin,
            check_end;

        int total_expr_num;
        int dup_expr_num;

        std::string details;

        void report2file() {
            std::string rfile("report_");
            rfile.append(file);
            std::cout << "Generating report into " << rfile << " ...";
            std::ofstream out(rfile);
            out << details;
            out << "\n";
            out << "Generation cost ";
            out << std::chrono::duration_cast<std::chrono::milliseconds>(run_end - run_begin).count();
            out << " ms.\n";
            out.close();
            std::cout << " DONE" << std::endl;
        }
    };
}