#include "const_tester.h"
#include "run_case.h"
#include "expr_generator_runner.h"
#include "expr_checker_runner.h"

static el::s_run_case g_cases[] = {
{
    100,
    1,
    "1.txt",
},
{
    1000,
    2,
    "2.txt",
},
{
    10000,
    4,
    "3.txt",
},
};

int main() {
    //c_const_tester tester;
    //tester.test_expr_tree();
    //tester.test_dup_checker();
    el::c_expr_generator_runner gen;
    el::c_expr_checker_runner chk;

    for (auto& c : g_cases) {
        gen.run_case(&c);
        chk.check_case(&c);
        c.report2file();
        std::cout << std::endl;
    }
}
