#pragma once
#include <chrono>
#include <string>

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
    };
}