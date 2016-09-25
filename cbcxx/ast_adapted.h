#ifndef CBCXX_AST_ADAPTED_H
#define CBCXX_AST_ADAPTED_H

#include "ast.h"
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::assign_expr,
    (client::ast::term, term_),
    (client::ast::expression, expr_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::opassign_expr,
    (client::ast::term, term_),
    (client::ast::opassign_token, assign_token),
    (client::ast::expression, expr_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::cond_expr,
    (client::ast::exprN, condition),
    (client::ast::expression, true_expr),
    (client::ast::expr10, false_expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::_exprN,
    (client::ast::exprN, first),
    (std::list<client::ast::operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::operation,
    (client::ast::optoken, operator_),
    (client::ast::exprN, oprand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::expr1,
    (client::ast::term, first_),
    (std::list<client::ast::term_operation>, term_operation_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::term_operation,
    (client::ast::optoken, operator_),
    (client::ast::term, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::type_term,
    (client::ast::typeref, typeref_),
    (client::ast::term, term_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::prefix_operation,
    (client::ast::op_prefix_token, token), 
    (client::ast::unary_expr, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::unary_operation,
    (client::ast::op_prefix_token, token_), 
    (client::ast::term, term_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::deference_operation,
    (client::ast::term, term_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::address_operation,
    (client::ast::term, term_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::sizeof_type_expr,
    (client::ast::typeref, type_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::sizeof_unary_expr,
    (client::ast::unary_expr, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::postfix_expr,
    (client::ast::primary, primary_),
    (std::list<client::ast::postfix>, postfixs)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::int_literal,
    (int, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::char_literal,
    (char, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::string_literal,
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::identifier,
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::parenthesis_expr,
    (client::ast::expression, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::suffix_operation,
    (client::ast::op_suffix_token, token)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::ref_operation,
    (client::ast::expression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::member_operation,
    (client::ast::identifier, iden)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::pointer_operation,
    (client::ast::identifier, iden)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::call_operation,
    (client::ast::args, arguments)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::args,
    (std::list<client::ast::expression>, exprs)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::typeref,
    (client::ast::typeref_base, base),
    (std::list<client::ast::type_suffix>, suffixs)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::array_size_type,
    (std::list<client::ast::param_typerefs>, refs)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::fixed_param_typerefs,
    (client::ast::typeref, first),
    (std::list<client::ast::typeref>, ref)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::primitive_typeref_base,
    (std::list<client::ast::primitive_typeref_enum>, primitive_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::struct_typeref_base,
    (std::string, identifier)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::union_typeref_base,
    (std::string, identifier)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::identifier_typeref_base,
    (std::string, identifier)
)
#endif