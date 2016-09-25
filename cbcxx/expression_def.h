#ifndef CBCXX_EXPRESSION_DEF_H
#define CBCXX_EXPRESSION_DEF_H

#include <boost/spirit/home/x3.hpp>
#include "expression.h"
#include "ast.h"
#include "ast_adapted.h"

namespace client {
    namespace parser {
        using x3::uint_;
        using x3::char_;
        using x3::bool_;
        using x3::raw;
        using x3::lexeme;
        using namespace x3::ascii;

        /*
        x3::symbols<char, client::ast::optoken> logical_and_op;
        x3::symbols<char, client::ast::optoken> logical_or_op;
        x3::symbols<char, ast::optoken> logical_or_op;
        */
        x3::symbols<ast::optoken> logical_and_op;
        x3::symbols<ast::optoken> logical_or_op;

        void add_keywords() {
            logical_and_op
                .add('&&', ast::op_logic_and);

            logical_or_op
                .add('||', ast::op_logic_or);
        }


        ///////////////////////////////////////////////////////
        // Expression grammar
        //////////////////////////////////////////////////////
        struct expression_class;
        struct expr_9_class;
        struct expr_8_class;
        struct expr_7_class;
        struct expr_6_class;
        struct expr_5_class;
        struct expr_4_class;
        struct expr_3_class;
        struct expr_2_class;
        struct expr_1_class;

        typedef x3::rule<expression_class, ast::expression> expression_type;
        typedef x3::rule<expr_9_class, ast::exprN> expr_9_type;
        typedef x3::rule<expr_8_class, ast::exprN> expr_8_type;
        typedef x3::rule<expr_7_class, ast::exprN> expr_7_type;
        typedef x3::rule<expr_6_class, ast::exprN> expr_6_type;
        typedef x3::rule<expr_5_class, ast::exprN> expr_5_type;
        typedef x3::rule<expr_4_class, ast::exprN> expr_4_type;
        typedef x3::rule<expr_3_class, ast::exprN> expr_3_type;
        typedef x3::rule<expr_2_class, ast::exprN> expr_2_type;
        typedef x3::rule<expr_1_class, ast::exprN> expr_1_type;

        expression_type const expression = "expression";
        expr_9_type const expr_9 = "expr_9";
        expr_8_type const expr_8 = "expr_8";
        expr_7_type const expr_7 = "expr_7";
        expr_6_type const expr_6 = "expr_6";
        expr_5_type const expr_5 = "expr_5";
        expr_4_type const expr_4 = "expr_4";
        expr_3_type const expr_3 = "expr_3";
        expr_2_type const expr_2 = "expr_2";
        expr_1_type const expr_1 = "expr_1";

        auto const expr_10_def =
                expr_9
            >> -('?' > expression > ':' > expr_10)
            ;

        auto const expr_9_def =
                expr_8
            >> *("||" > expr_8)
            ;

        auto const expr_8_def =
                expr_7
            >> *("&&" > expr_7)
            ;

} }

#endif