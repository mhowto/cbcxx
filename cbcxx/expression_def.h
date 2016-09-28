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
        using x3::lit;
        using x3::repeat;
        using namespace x3::ascii;

        /*
        x3::symbols<char, client::ast::optoken> logical_and_op;
        x3::symbols<char, client::ast::optoken> logical_or_op;
        x3::symbols<char, ast::optoken> logical_or_op;
        */
        x3::symbols<ast::optoken> logical_and_op;
        x3::symbols<ast::optoken> logical_or_op;
        x3::symbols<ast::optoken> relational_op;
        x3::symbols<ast::optoken> bitwise_or_op;
        x3::symbols<ast::optoken> bitwise_xor_op;
        x3::symbols<ast::optoken> bitwise_and_op;
        x3::symbols<ast::optoken> shift_op;
        x3::symbols<ast::optoken> plus_op;
        x3::symbols<ast::optoken> mult_op;
        x3::symbols<ast::opassign_token> assign_op;

        x3::symbols<ast::op_prefix_token> prefix_op_1;
        x3::symbols<ast::op_prefix_token> prefix_op_2;
        x3::symbols<ast::op_suffix_token> suffix_op;

        void add_keywords() {
            logical_and_op.add
                ("&&", ast::op_logic_and)
                ;
            logical_or_op.add
                ("||", ast::op_logic_or)
                ;
            relational_op.add
                (">", ast::op_gt)
                ("<", ast::op_lt)
                ("<", ast::op_lt)
                (">=", ast::op_ge)
                ("<=", ast::op_le)
                ("==", ast::op_eq)
                ("!=", ast::op_ne)
                ;
            bitwise_or_op.add
                ("|", ast::op_bitwise_or)
                ;
            bitwise_xor_op.add
                ("^", ast::op_bitwise_xor)
                ;
            bitwise_and_op.add
                ("&", ast::op_bitwise_and)
                ;
            shift_op.add
                ("<<", ast::op_left_shift)
                (">>", ast::op_right_shift)
                ;
            plus_op.add
                ("+", ast::op_plus)
                ("-", ast::op_minus)
                ;
            mult_op.add
                ("*", ast::op_multiply)
                ("/", ast::op_divide)
                ("%", ast::op_mod)
                ;
            assign_op.add
                ("+=", ast::op_add_equal)
                ("-=", ast::op_minus_equal)
                ("*=", ast::op_multiply_equal)
                ("/=", ast::op_divide_equal)
                ("%=", ast::op_mod_equal)
                ("&=", ast::op_and_equal)
                ("|=", ast::op_or_equal)
                ("^=", ast::op_xor_equal)
                ("<<=", ast::op_shift_left_equal)
                (">>=", ast::op_shift_right_equal)
                ;
            prefix_op_1.add
                ("++", ast::op_prefix_increment)
                ("--", ast::op_prefix_decrement)
                ;
            prefix_op_2.add
                ("+", ast::op_positive)
                ("-", ast::op_negative)
                ("!", ast::op_not)
                ("~", ast::op_bitwise_not)
                ;
            suffix_op.add
                ("++", ast::op_suffix_increment)
                ("--", ast::op_suffix_decrement)
                ;
        }


        ///////////////////////////////////////////////////////
        // Expression grammar
        //////////////////////////////////////////////////////
        struct expression_class;
        struct assign_expr_class;
        struct opassign_expr_class;

        struct condition_expr_class;
        struct expr_10_class;
        struct expr_9_class;
        struct expr_8_class;
        struct expr_7_class;
        struct expr_6_class;
        struct expr_5_class;
        struct expr_4_class;
        struct expr_3_class;
        struct expr_2_class;
        struct expr_1_class;

        struct term_class;
        struct type_term_class;
        struct unary_expr_class;
        struct typeref_class;
        struct prefix_operation_class;
        struct unary_operation_class;
        struct dereference_operation_class;
        struct address_operation_class;
        struct sizeof_expr_class;
        struct sizeof_type_expr_class;
        struct sizeof_unary_expr_class;
        struct postfix_expr_class;

        struct primary_class;
        struct postfix_class;

        struct int_literal_class;
        struct char_literal_class;
        struct string_literal_class;
        struct identifier_class;
        struct parenthesis_expr_class;

        struct suffix_operation_class;
        struct ref_operation_class;
        struct member_operation_class;
        struct pointer_operation_class;
        struct call_operation_class;
        struct args_class;

        typedef x3::rule<expression_class, ast::expression> expression_type;
        typedef x3::rule < assign_expr_class, ast::assign_expr > assign_expr_type;
        typedef x3::rule < opassign_expr_class, ast::opassign_expr> opassign_expr_type;

        typedef x3::rule<condition_expr_class, ast::cond_expr> condition_expr_type;
        typedef x3::rule<expr_10_class, ast::expr10> expr_10_type;
        typedef x3::rule<expr_9_class, ast::exprN> expr_9_type;
        typedef x3::rule<expr_8_class, ast::exprN> expr_8_type;
        typedef x3::rule<expr_7_class, ast::exprN> expr_7_type;
        typedef x3::rule<expr_6_class, ast::exprN> expr_6_type;
        typedef x3::rule<expr_5_class, ast::exprN> expr_5_type;
        typedef x3::rule<expr_4_class, ast::exprN> expr_4_type;
        typedef x3::rule<expr_3_class, ast::exprN> expr_3_type;
        typedef x3::rule<expr_2_class, ast::exprN> expr_2_type;
        typedef x3::rule<expr_1_class, ast::exprN> expr_1_type;

        typedef x3::rule<term_class, ast::term> term_type;
        typedef x3::rule<type_term_class, ast::type_term> type_term_type;
        typedef x3::rule<unary_expr_class, ast::unary_expr> unary_expr_type;
        typedef x3::rule<typeref_class, ast::typeref> typeref_type;
        typedef x3::rule<prefix_operation_class, ast::prefix_operation> prefix_operation_type;
        typedef x3::rule<unary_operation_class, ast::unary_operation> unary_operation_type;
        typedef x3::rule<dereference_operation_class, ast::dereference_operation> dereference_operation_type;
        typedef x3::rule<address_operation_class, ast::address_operation> address_operation_type;
        typedef x3::rule<sizeof_expr_class, ast::sizeof_expr> sizeof_expr_type;
        typedef x3::rule<sizeof_type_expr_class, ast::sizeof_type_expr> sizeof_type_expr_type;
        typedef x3::rule<sizeof_unary_expr_class, ast::sizeof_unary_expr> sizeof_unary_expr_type;
        typedef x3::rule<postfix_expr_class, ast::postfix_expr> postfix_expr_type;

        typedef x3::rule<primary_class, ast::primary> primary_type;
        typedef x3::rule<postfix_class, ast::postfix> postfix_type;

        typedef x3::rule<int_literal_class, ast::int_literal> int_literal_type;
        typedef x3::rule<char_literal_class, ast::char_literal> char_literal_type;
        typedef x3::rule<string_literal_class, ast::string_literal> string_literal_type;
        typedef x3::rule<identifier_class, ast::identifier> identifier_type;
        typedef x3::rule<parenthesis_expr_class, ast::parenthesis_expr> parenthesis_expr_type;

        typedef x3::rule<suffix_operation_class, ast::suffix_operation> suffix_operation_type;
        typedef x3::rule<ref_operation_class, ast::ref_operation> ref_operation_type;
        typedef x3::rule<member_operation_class, ast::member_operation> member_operation_type;
        typedef x3::rule<pointer_operation_class, ast::pointer_operation> pointer_operation_type;
        typedef x3::rule<call_operation_class, ast::call_operation> call_operation_type;
        typedef x3::rule<args_class, ast::args> args_type;

        expression_type const expression = "expression";
        assign_expr_type const assign_expr = "assign_expr";
        opassign_expr_type const opassign_expr = "opassign_expr";
        
        condition_expr_type const condition_expr = "condition_expr";
        expr_10_type const expr_10 = "expr_10";
        expr_9_type const expr_9 = "expr_9";
        expr_8_type const expr_8 = "expr_8";
        expr_7_type const expr_7 = "expr_7";
        expr_6_type const expr_6 = "expr_6";
        expr_5_type const expr_5 = "expr_5";
        expr_4_type const expr_4 = "expr_4";
        expr_3_type const expr_3 = "expr_3";
        expr_2_type const expr_2 = "expr_2";
        expr_1_type const expr_1 = "expr_1";
        
        term_type const term = "term";
        type_term_type const type_term = "type_term";
        unary_expr_type const unary_expr = "unary_expr";
        typeref_type const typeref = "typeref";
        prefix_operation_type const prefix_operation = "prefix_operation";
        unary_operation_type const unary_operation = "unary_operation";
        dereference_operation_type const dereference_operation = "dereference_operation";
        address_operation_type const address_operation = "address_operation";
        sizeof_expr_type const sizeof_expr = "sizeof_expr";
        sizeof_type_expr_type const sizeof_type_expr = "sizeof_type_expr";
        sizeof_unary_expr_type const sizeof_unary_expr = "sizeof_unary_expr";
        postfix_expr_type const postfix_expr = "postfix_expr";

        primary_type const primary = "primary";
        postfix_type const postfix = "postfix";

        int_literal_type const int_literal = "int_literal";
        char_literal_type const char_literal = "char_literal";
        string_literal_type const string_literal = "string_literal";
        identifier_type const identifier = "identifier";
        parenthesis_expr_type const parenthesis_expr = "parenthesis_expr";

        suffix_operation_type const suffix_operation = "suffix_operation";
        ref_operation_type const ref_operation = "ref_operation";
        member_operation_type const member_operation = "member_operation";
        pointer_operation_type const pointer_operation = "pointer_operation";
        call_operation_type const call_operation = "call_operation";
        args_type const args = "args";

        auto const expression_def =
            assign_expr
            | opassign_expr
            | expr_10;

        auto assign_expr_def =
            term >> "=" >> expression
            ;
        auto opassign_expr_def =
            term >> assign_op >> expression
            ;
        auto const expr_10_def =
            expr_9
            | condition_expr
            ;
        auto const condition_expr_def =
            expr_9 >> "?" >> expression >> ":" >> expr_10
            ;
        auto const expr_9_def =
            expr_8
            >> *(logical_or_op >> expr_8)
            ;
        auto const expr_8_def =
            expr_7
            >> *(logical_and_op >> expr_7)
            ;
        auto const expr_7_def =
            expr_6
            >> *(relational_op >> expr_6)
            ;
        auto const expr_6_def =
            expr_5
            >> *(bitwise_or_op >> expr_5)
            ;
        auto const expr_5_def =
            expr_4
            >> *(bitwise_xor_op >> expr_4)
            ;
        auto const expr_4_def =
            expr_3
            >> *(bitwise_and_op >> expr_3)
            ;
        auto const expr_3_def =
            expr_2
            >> *(shift_op >> expr_2)
            ;
        auto const expr_2_def =
            expr_1
            >> *(plus_op >> expr_1)
            ;
        auto const expr_1_def =
            term
            >> *(mult_op >> term)
            ;
        auto const term_def =
            type_term
            >> unary_expr
            ;
        auto const type_term_def =
            "(" >> typeref >> ")" >> term
            ;
        auto const unary_expr_def =
            prefix_operation
            | unary_operation
            | dereference_operation
            | address_operation
            | sizeof_expr
            | postfix_expr
            ;
        auto const prefix_operation_def =
            prefix_op_1 >> unary_expr;
        auto const unary_operation_def =
            prefix_op_2 >> term;
        auto const dereference_operation_def =
            "*" >> term;
        auto const address_operation_def =
            "&" >> term;
        auto const sizeof_expr_def =
            sizeof_type_expr
            | sizeof_unary_expr;
        auto const sizeof_type_expr_def =
            lit("sizeof") >> "(" >> typeref >> ")"
            ;
        auto const sizeof_unary_expr_def =
            lit("sizeof") >> unary_expr;
        auto const postfix_expr_def =
            primary
            >> *postfix;
        auto const primay_def =
            int_literal
            | char_literal
            | string_literal
            | identifier
            | parenthesis_expr
            ;
        auto const postfix_def =
            suffix_operation
            | ref_operation
            | member_operation
            | pointer_operation
            | call_operation
            ;
        auto const int_literal_def =
            uint_;
            ;
        auto const char_literal_def =
            char_
            ;

        auto const append = [](auto& ctx) { _val(ctx) += _attr(ctx); };
        auto const string_literal_def =
            lexeme['"'
            > *(+(~char_("\"\\\n\r"))
             | ("\\" > repeat(3)[char_("0"-"7")])
             | ("\\" > char_) [append]
             )
         > '"']
            ;
        auto const identifier_def =
            lexeme[char_("a-zA-Z_") > *(char_("0-9a-zA-Z_"))]
            ;
        auto const parenthesis_expr_def =
            "(" >> expression >> ")"
            ;

        auto const suffix_operation_def =
            suffix_op
            ;
        auto const ref_operation_def =
            "[" >> expression >> "]"
            ;
        auto const member_operation_def =
            "." >> identifier
            ;
        auto const pointer_operation_def =
            "->" >> identifier
            ;
        auto const call_operation_def =
            "(" >> args >> ")"
            ;
        auto const args_def =
            -(expression >> *("," > expression))
            ;

        auto const typeref_def =
            "int"
            ;

        BOOST_SPIRIT_DEFINE(
            expression = expression_def,
            assign_expr = assign_expr_def,
            opassign_expr = opassign_expr_def,
            condition_expr = condition_expr_def,
            expr_10 = expr_10_def,
            expr_9 = expr_9_def,
            expr_8 = expr_8_def,
            expr_7 = expr_7_def,
            expr_6 = expr_6_def,
            expr_5 = expr_5_def,
            expr_4 = expr_4_def,
            expr_3 = expr_3_def,
            expr_2 = expr_2_def,
            expr_1 = expr_1_def,
            term = term_def,
            type_term = type_term_def,
            unary_expr = unary_expr_def,
            typeref = typeref_def,
            prefix_operation = prefix_operation_def,
            unary_operation = unary_operation_def,
            dereference_operation = dereference_operation_def,
            address_operation = address_operation_def,
            sizeof_expr = sizeof_expr_def,
            sizeof_type_expr = sizeof_type_expr_def,
            sizeof_unary_expr = sizeof_unary_expr_def,
            postfix_expr = postfix_expr_def,
            primary = primary_def,
            postfix = postfix_def,

            int_literal = int_literal_def,
            char_literal = char_literal_def,
            string_literal = string_literal_def,
            identifier = identifier_def,
            parenthesis_expr = parenthesis_expr_def,

            suffix_operation = suffix_operation_def,
            ref_operation = ref_operation_def,
            member_operation = member_operation_def,
            pointer_operation = pointer_operation_def,
            call_operation = call_operation_def,
            args = args_def
        );
        */
} }

namespace client
{
    parser::expression_type const& expression()
    {
        parser::add_keywords();
        return parser::expression;
    }
}

#endif