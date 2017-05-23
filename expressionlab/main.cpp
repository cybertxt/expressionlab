
#include <iostream>
#include "expression.h"
void test() {
    el::expression expr1;
    expr1.from_string("(3 + 5 - 8 * 2 / 9) * 6 / (2 + 7-1)");
    std::cout << expr1.to_string() << std::endl;
    std::vector<el::s_token> postfix;
    el::expression::infix2postfix(expr1.m_tokens, postfix);
    expr1.set_tokens(std::move(postfix));
    std::cout << "postfix: " << expr1.to_string() << std::endl;

    el::expr_tree et1;
    et1.construct_from_postfix(expr1.m_tokens);

    el::expression expr2;
    expr2.from_string("6 * (3 + 5 - 2 * 8 / 9)  / (((((7+2-1)))))");
    std::cout << expr2.to_string() << std::endl;
    el::expression::infix2postfix(expr2.m_tokens, postfix);
    expr2.set_tokens(std::move(postfix));
    std::cout << "postfix: " << expr2.to_string() << std::endl;

    el::expr_tree et2;
    et2.construct_from_postfix(expr2.m_tokens);

    std::cout << "is_isomorphic: " << et1.is_isomorphic(et2) << std::endl;
    
    getchar();
}

int main() {
    test();
}
