#pragma once

#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <ctype.h>
#include <assert.h>

namespace el // expressionlab
{
    enum e_token_type
    {
        TOKEN_TYPE_OPERAND,
        TOKEN_TYPE_OPERATOR,
    };

    struct s_token
    {
        e_token_type type;
        int value;

        bool is_operator() const {
            return type == TOKEN_TYPE_OPERATOR;
        }
        bool is_operand() const {
            return type == TOKEN_TYPE_OPERAND;
        }
        std::string to_string() const {
            if (is_operator()) {
                char s[2];
                s[0] = (char)value;
                s[1] = '\0';
                return s;
            }

            if (is_operand())
                return std::to_string(value);

            return "";
        }

        bool operator==(const s_token& other) {
            return type == other.type && value == other.value;
        }
        bool operator!=(const s_token& other) {
            return value != other.value || type != other.type;
        }
    };
    
    class expression {
    public:
        std::vector<s_token> m_tokens;

    public:
        static bool is_operator(char ch) {
            switch (ch)
            {
            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
                return true;
            default:
                return false;
            }
        }

        static int operator_prec(const s_token& token)
        {
            if (!token.is_operator())
                return -1;

            auto ch = token.value;
            switch (ch)
            {
            case '+':
            case '-':
                return 1;

            case '*':
            case '/':
                return 2;
            }
            return -1;
        }

        static bool infix2postfix(const std::vector<s_token>& infix, std::vector<s_token>& postfix) {
            if (infix.size() == 0)
                return false;

            std::stack<s_token> op_stack;
            for (const auto& in : infix) {
                if (in.is_operand()) {
                    postfix.push_back(in);
                    continue;
                }

                if (in.is_operator() && in.value == '(') {
                    op_stack.push(in);
                    continue;
                }

                if (in.is_operator() && in.value == ')') {
                    while (!op_stack.empty() && op_stack.top().value != '(') {
                        postfix.push_back(op_stack.top());
                        op_stack.pop();
                    }
                    if (!op_stack.empty() && op_stack.top().value != '(')
                        return false;
                    else
                        op_stack.pop();
                }
                else {
                    while (!op_stack.empty() && operator_prec(in) <= operator_prec(op_stack.top())) {
                        postfix.push_back(op_stack.top());
                        op_stack.pop();
                    }
                    op_stack.push(in);
                }
            }

            while (!op_stack.empty()) {
                postfix.push_back(op_stack.top());
                op_stack.pop();
            }

            return true;
        }

        void from_string(const char* exp) {
            int operand = 0;
            bool has_operand = false;
            s_token t;
            char c;

#define try_push_operand() do {                 \
                if (has_operand) {              \
                    if (operand < 1 || operand > 9) {\
                        std::cerr << "operand " << operand << " is NOT ALLOWED!" << std::endl; \
                        assert(operand >= 1);\
                        assert(operand <= 9);\
                    }\
                    t.type = TOKEN_TYPE_OPERAND;\
                    t.value = operand;          \
                    m_tokens.push_back(t);      \
                    operand = 0;                \
                    has_operand = false;        \
                }                               \
            } while(0)

            for (int i = 0; exp[i] != 0; ++i) {
                c = exp[i];

                if (isblank(c)) {
                    try_push_operand();
                    continue;
                }

                if (isdigit(c)) {
                    operand *= 10;
                    operand += c - '0';
                    has_operand = true;
                    continue;
                }

                if (is_operator(c)) {
                    try_push_operand();
                    t.type = TOKEN_TYPE_OPERATOR;
                    t.value = c;
                    m_tokens.push_back(t);
                    continue;
                }

                assert(false);
            }
            try_push_operand();
#undef try_push_operand
        }

        void swap_tokens(std::vector<el::s_token>& t) {
            m_tokens.swap(t);
        }

        std::string to_string() const {
            std::string result;
            for (const auto& t : m_tokens) {
                result.append(t.to_string());
                result.append(" ");
            }
            return result;
        }

    };
    
    class expr_tree {
        struct expr_tree_node;
        typedef std::shared_ptr<expr_tree_node> expr_tree_node_ptr;

        struct expr_tree_node {
            expr_tree_node(const s_token& t)
                : token(t)
                , left(nullptr)
                , right(nullptr)
            {}
            s_token token;
            expr_tree_node_ptr left;
            expr_tree_node_ptr right;
        };
    public:
        expr_tree() : m_root(nullptr) {}

        void construct_from_postfix(const std::vector<s_token>& postfix) {
            std::stack<expr_tree_node_ptr> node_stack;
            expr_tree_node_ptr n, n1, n2;
            for (const auto& p : postfix) {
                if (p.is_operand()) {
                    n = std::make_shared<expr_tree_node>(p);
                    node_stack.push(n);
                }
                else {
                    n1 = node_stack.top();
                    node_stack.pop();
                    n2 = node_stack.top();
                    node_stack.pop();

                    n = std::make_shared<expr_tree_node>(p);
                    n->left = n1;
                    n->right = n2;

                    node_stack.push(n);
                }
            }

            m_root = node_stack.top();
        }

        const expr_tree_node_ptr root() const {
            return m_root;
        }

        bool is_isomorphic(const expr_tree& other) const {
            auto other_root = other.root();
            return is_isomorphic(m_root, other_root);
        }

        bool is_isomorphic(const expr_tree_node_ptr& l
            , const expr_tree_node_ptr& r) const {
            if (l == nullptr && r == nullptr)
                return true;

            if (l == nullptr || r == nullptr)
                return false;

            if (l->token != r->token)
                return false;

            if (is_isomorphic(l->left, r->left) && is_isomorphic(l->right, r->right))
                return true;

            // only + or * should compare the fliped subtree
            if (l->token.is_operator() && (l->token.value == '+') || (l->token.value == '*'))
                if (is_isomorphic(l->left, r->right) && is_isomorphic(l->right, r->left))
                    return true;

            return false;
        }

    private:
        expr_tree_node_ptr m_root;
    };
}
