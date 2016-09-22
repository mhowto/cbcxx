#ifndef CBCXX_AST_H
#define CBCXX_AST_H

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/optional>

namespace client {
    namespace ast {
        namespace x3 = boost::spirit::x3;


        struct expression;
        struct assign_expr;
        struct opassign_expr;
        struct expr10;
        struct exprN;
        struct primary;

        struct expression :
            x3::variant<
            x3::forward_ast<assign_expr>,
            x3::forward_ast<opassign_expr>,
            x3::forward_ast<expr10>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        enum optoken
        {
            op_plus
        };

        struct expr10 : x3::position_tagged
        {
            exprN first;
            boost::optional<expression> true_epxr;
            boost::optional<expr10> false_epxr;
        };

        struct exprN : x3::position_tagged
        {
            exprN first;
            std::list<operation> rest;
        }

        struct operation : x3::position_tagged
        {
        }

}

#endif