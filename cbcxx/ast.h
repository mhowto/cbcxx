#ifndef CBCXX_AST_H
#define CBCXX_AST_H

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/optional/optional.hpp>
#include <list>

namespace client {
    namespace ast {
        namespace x3 = boost::spirit::x3;


        struct expression;
        struct assign_expr;
        struct opassign_expr;
        struct expr10;
        struct exprN;
        struct _exprN;
        struct expr1;
        /*
        struct expr8;
        struct expr7;
        struct expr6;
        struct expr5;
        struct expr4;
        struct expr3;
        struct expr2;
        struct expr1;
        */
        struct term;
        struct type_term;
        struct unary_expr;

        struct prefix_expr;
        struct sizeof_expr;
        struct postfix_expr;

        struct primary;

        struct expression :
            x3::variant<
                x3::forward_ast<assign_expr>,
                x3::forward_ast<opassign_expr>,
                x3::forward_ast<expr10>
            > {
            using base_type::base_type;
            using base_type::operator=;
        };

        enum optoken {
            // Expr 9
            op_logic_or,     // a || b

            // Expr 8
            op_logic_and,    // a && b

            // Expr 7
            op_gt,    // a > b
            op_lt,    // a < b
            op_ge,    // a >= b
            op_le,    // a <= b
            op_eq,    // a == b
            op_ne,    // a != b

            // Expr 6
            op_bitwise_or, // a | b

            // Expr 5
            op_bitwise_xor, // a ^ b

            // Expr 4
            op_bitwise_and, // a & b

            // Expr 3
            op_left_shift,  // a << b
            op_right_shift, // a >> b

            // Expr 2
            op_plus,   // a + b
            op_minus,  // a - b

            // Expr 1
            op_multiply,  // a * b
            op_divide,    // a / b
            op_mod        // a % b
        };

        struct expr10 : x3::position_tagged {
            exprN first;
            boost::optional<expression> true_epxr;
            boost::optional<expr10> false_epxr;
        };

        struct exprN :
            x3::variant<
                x3::forward_ast<_exprN>,
                x3::forward_ast<expr1>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct _exprN : x3::position_tagged {
            exprN first;
            std::list<operation> rest;
        };

        struct operation : x3::position_tagged {
            optoken operator_;
            exprN operand_;
        };

        struct expr1 : x3::position_tagged {
            term first;
            std::list<term_operation> term;
        };

        struct term_operation : x3::position_tagged {
            optoken operator_;
            term operand_;
        };

        struct term :
            x3::variant<
                x3::forward_ast<type_term>,
                x3::forward_ast<unary_expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct type_term : x3::position_tagged {
            type type_;
            term term_;
        };

        struct unary_expr :
            x3::variant<
                x3::forward_ast<prefix_expr>,
                x3::forward_ast<sizeof_expr>,
                x3::forward_ast<postfix_expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

    }
}

#endif
