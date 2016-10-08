#ifndef CBCXX_AST_H
#define CBCXX_AST_H

#include <list>
#include <string>

namespace cbcxx {
    namespace ast {
        struct expression;
        struct assign_expr;
        struct opassign_expr;
        struct expr10;
        struct exprN;
        struct _exprN;
        struct expr1;
        struct cond_expr;
        struct term_operation;
        struct operation;

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
        struct prefix_operation;
        struct unary_operation;
        struct dereference_operation;
        struct address_operation;

        struct primary;
        struct postfix;

        struct sizeof_type_expr;
        struct sizeof_unary_expr;

        struct suffix_operation;
        struct ref_operation;
        struct member_operation;
        struct pointer_operation;
        struct call_operation;
        struct args;

        struct int_literal;
        struct char_literal;
        struct string_literal;
        struct identifier;
        struct parenthesis_expr;

        struct typeref;
        struct typeref_base;
        struct array_empty_type;
        struct array_size_type;
        struct ptr_type;
        struct function_type;

        struct param_typerefs;
        struct void_param_typeref;
        struct fixed_param_typerefs;

        struct primitive_typeref_base;
        struct custom_typeref_base;

        struct struct_typeref_base;
        struct union_typeref_base;
        struct identifier_typeref_base;

        struct typedef_;

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

        enum opassign_token {
            op_add_equal,          // a += 1
            op_minus_equal,        // a -= 1
            op_multiply_equal,     // a *= 1
            op_divide_equal,       // a /= 1
            op_mod_equal,          // a %= 1
            op_and_equal,          // a &= 0x1
            op_or_equal,           // a |= 0x1
            op_xor_equal,          // a ^= 0x1
            op_shift_left_equal,   // a <<= 1
            op_shift_right_equal   // a >>= 1
        };

        enum op_prefix_token {
            op_prefix_increment,       // ++a
            op_prefix_decrement,       // --a
            op_positive,        // +a
            op_negative,        // -a
            op_not,             // !a
            op_bitwise_not,     // ~a
        };

        enum op_suffix_token {
            op_suffix_increment,       // a++
            op_suffix_decrement        // --a
        };

        /**********************************************************
        *******           Typeref Ast                      ********
        **********************************************************/
        enum primitive_typeref_enum {
            void_t_base,
            char_t_base,
            shor_t_base,
            int_t_base,
            long_t_base,
            unsigned_char_t_base,
            unsigned_short_t_base,
            unsigned_int_t_base,
            unsigned_long_t_base
        };

        struct typeref_base :
            x3::variant<
            x3::forward_ast<primitive_typeref_base>,
            x3::forward_ast<struct_typeref_base>,
            x3::forward_ast<union_typeref_base>,
            x3::forward_ast<identifier_typeref_base>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct type_suffix :
            x3::variant<
                x3::forward_ast<array_empty_type>,
                x3::forward_ast<array_size_type>,
                x3::forward_ast<ptr_type>,
                x3::forward_ast<function_type>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };
        struct typeref : x3::position_tagged {
            typeref_base base;
            std::list<type_suffix> suffixs;
        };

        struct param_typerefs :
            x3::variant<
            x3::forward_ast<void_param_typeref>,
            x3::forward_ast<fixed_param_typerefs>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct array_empty_type : x3::position_tagged {};
        struct array_size_type : x3::position_tagged {
            int size_;
        };
        struct ptr_type : x3::position_tagged {};
        struct function_type : x3::position_tagged {
            param_typerefs refs;
        };

        struct void_param_typeref : x3::position_tagged {};

        struct fixed_param_typerefs : x3::position_tagged {
            typeref first;
            std::list<typeref> rest;
        };


        struct primitive_typeref_base : x3::position_tagged {
            primitive_typeref_enum primitive_type;
        };

        struct struct_typeref_base : x3::position_tagged {
            std::string identifier;
        };

        struct union_typeref_base : x3::position_tagged {
            std::string identifier;
        };

        struct identifier_typeref_base : x3::position_tagged {
            std::string identifier;
        };
        /**********************************************************
        *******           Typeref Ast End                  ********
        **********************************************************/

        /**********************************************************
        *******           Expression Ast                   ********
        ***********************************************************/
        struct expression :
            x3::variant<
            x3::forward_ast<assign_expr>,
            x3::forward_ast<opassign_expr>,
            x3::forward_ast<expr10>
            > {
            using base_type::base_type;
            using base_type::operator=;
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

        struct term_operation : x3::position_tagged {
            optoken operator_;
            term operand_;
        };

        struct expr1 : x3::position_tagged {
            term first_;
            std::list<term_operation> term_operation_;
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

        struct assign_expr : x3::position_tagged {
            term term_;
            expression expr_;
        };

        struct opassign_expr : x3::position_tagged {
            term term_;
            opassign_token assign_token;
            expression expr_;
        };

        struct expr10 :
            x3::variant <
                x3::forward_ast<exprN>,
                x3::forward_ast<cond_expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct cond_expr : x3::position_tagged {
            exprN condition;
            expression true_expr;
            expr10 false_expr;
        };

        struct type_term : x3::position_tagged {
            typeref typeref_;
            term term_;
        };

        struct unary_expr :
            x3::variant<
                x3::forward_ast<prefix_operation>,
                x3::forward_ast<unary_operation>,
                x3::forward_ast<dereference_operation>,
                x3::forward_ast<address_operation>,
                x3::forward_ast<sizeof_expr>,
                x3::forward_ast<postfix_expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct prefix_operation : x3::position_tagged {
            op_prefix_token token;
            unary_expr expr;
        };

        struct unary_operation : x3::position_tagged {
            op_prefix_token token_;
            term term_;
        };

        struct deference_operation : x3::position_tagged {
            term term_;
        };

        struct address_operation : x3::position_tagged {
            term term_;
        };

        struct sizeof_expr :
            x3::variant<
                x3::forward_ast<sizeof_type_expr>,
                x3::forward_ast<sizeof_unary_expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct sizeof_type_expr : x3::position_tagged {
            typeref type_;
        };

        struct sizeof_unary_expr : x3::position_tagged {
            unary_expr expr;
        };

        struct primary :
            x3::variant<
                x3::forward_ast<int_literal>,
                x3::forward_ast<char_literal>,
                x3::forward_ast<string_literal>,
                x3::forward_ast<identifier>,
                x3::forward_ast<parenthesis_expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct postfix_expr : x3::position_tagged {
            primary primary_;
            std::list<postfix> postfixs;
        };

        struct int_literal : x3::position_tagged {
            int value;
        };

        struct char_literal : x3::position_tagged {
            char value;
        };

        struct string_literal : x3::position_tagged {
            std::string value;
        };

        struct identifier : x3::position_tagged {
            std::string value;
        };

        struct parenthesis_expr : x3::position_tagged {
            expression expr;
        };

        struct postfix :
            x3::variant<
                x3::forward_ast<suffix_operation>,
                x3::forward_ast<ref_operation>,
                x3::forward_ast<member_operation>,
                x3::forward_ast<pointer_operation>,
                x3::forward_ast<call_operation>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct suffix_operation : x3::position_tagged {
            op_suffix_token token;
        };

        struct ref_operation : x3::position_tagged {
            expression expr;
        };

        struct member_operation : x3::position_tagged {
            identifier iden;
        };

        struct pointer_operation : x3::position_tagged {
            identifier iden;
        };

        struct args : x3::position_tagged {
            std::list<expression> exprs;
        };

        struct call_operation : x3::position_tagged {
            args arguments;
        };
        /**********************************************************
        *******           Expression Ast  End              ********
        **********************************************************/
    }
}

#endif
