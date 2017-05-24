#pragma once
#include "expression.h"
#include <unordered_set>

namespace el {
    struct s_expr_info {
        expression expr;
        expr_tree tree;
        std::string origin_string;
        int line_in_file;
        std::vector<std::shared_ptr<s_expr_info>> dup_list;
    };

    class c_expr_dup_checker {
    public:
        c_expr_dup_checker()
            : m_expr_set()
            , m_total_count(0)
            , m_dup_count(0)
        {}

        ~c_expr_dup_checker() = default;

        bool add_expr(const std::string& expr, int line_num) {
            auto expr_info = std::make_shared<s_expr_info>();
            expr_info->origin_string = expr;
            expr_info->line_in_file = line_num;
            expr_info->expr.from_string(expr.c_str());
            auto& tokens = expr_info->expr.m_tokens;
            std::vector<s_token> postfix;
            auto b = expression::infix2postfix(tokens, postfix);
            if (!b)
                return b;

            expr_info->tree.construct_from_postfix(postfix);

            auto r = m_expr_set.insert(expr_info);
            if (!r.second) {
                (*r.first)->dup_list.push_back(expr_info);
                ++m_dup_count;
            }
            ++m_total_count;
            return true;
        }

        std::string dump_statistics() const {
            std::string out;
            for (const auto& e : m_expr_set) {
                if (e->dup_list.size() > 0) {
                    out.append("\n\n [");
                    out.append(e->origin_string);
                    out.append("] @ line ");
                    out.append(std::to_string(e->line_in_file));
                    out.append(" is repeated with:");
                    for (const auto& d : e->dup_list) {
                        out.append("\n    -> [");
                        out.append(d->origin_string);
                        out.append("] @ line ");
                        out.append(std::to_string(d->line_in_file));
                    }
                }
            }
            out.append("\n\n===============================");
            out.append("\ntotal expressions: "); out.append(std::to_string(m_total_count));
            out.append("\nduplicated expressions: "); out.append(std::to_string(m_dup_count));
            out.append("\n===============================");
            return out;
        }

    private:
        struct expr_hash {
            size_t operator()(const std::shared_ptr<s_expr_info>& l) const {
                // here may be linearly searching, just make it right now, effciency comes third.
                return 1;
            }
        };
        struct expr_equal {
            bool operator()(const std::shared_ptr<s_expr_info>& l
                , const std::shared_ptr<s_expr_info>& r) const {
                // tree hash is needed to avoid repeatedly traversing the tree.
                return l->tree.is_isomorphic(r->tree);
            }
        };
        std::unordered_set<std::shared_ptr<s_expr_info>, expr_hash, expr_equal> m_expr_set;
    public:
        int m_total_count;
        int m_dup_count;
    };
}
