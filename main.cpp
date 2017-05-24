#include "const_tester.h"
#include "run_case.h"
#include "expr_generator_runner.h"
#include "expr_checker_runner.h"

static el::s_run_case g_cases[] = {
{
    10,
    1,
    "10.txt",
},
{
    30,
    1,
    "30.txt",
},
{
    100,
    1,
    "100.txt",
},
{
    1000,
    3,
    "1000.txt",
},
{
    10000,
    5,
    "10000.txt",
},
{
    20000,
    7,
    "20000.txt",
},
};

int main(int argc, char** argv) {
    //c_const_tester tester;
    //tester.test_expr_tree();
    //tester.test_dup_checker();

    el::c_expr_generator_runner gen;
    el::c_expr_checker_runner chk;

    for (auto& c : g_cases) {
        if (argc == 2 && memcmp(argv[1], "chk", 3) == 0)
            goto L_CHECK;
        auto ret = gen.run_case(&c);
        if (ret) {
            std::cout << "CAN NOT run your program properly, quit!" << std::endl;
            break;
        }
        L_CHECK:
        chk.check_case(&c);
        c.report2file();
        std::cout << std::endl;
    }

    std::cout << "Press [Enter] to exit ...";
    getchar();
}
