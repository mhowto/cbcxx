#ifndef CBCXX_TOKEN_H
#define CBCXX_TOKEN_H

#include <string>
#include <vector>

namespace cbcxx { namespace scanner {
    class Token;
    class Scanner;

    struct SourceLocation {
        std::string fileName;
        const char* lineBegin;
        unsigned line;
        unsigned column;
    };

    class Token {
        friend class Scanner;
        public:
            enum {
                // punctutors
                LPAR = '(',
                RPAR = ')',
                LSQB = '[',
                RSQB = ']',
                LBRACE = '{',
                RBRACE = '}',

                COMMA = ',',
                SEMI = ';',

                ADD = '+',
                SUB = '-',
                MUL = '*',
                DIV = '/',

                OR = '|',
                AND = '&',
                XOR = '^',
                TILDE = '~',
                NOT = '!',
                COND = '?',

                LESS = '<',
                GREATER = '>',
                ASSIGN = '=',
                DOT = '.',
                MOD = '%',

                SHARP = '#',
                NEW_LINE = '\n',

                DSHARP = 128, // ##
                PTR,     // ->
                INC,     // ++
                DEC,     // --
                LEFT,    // >>
                RIGHT,   // <<
                LE,      // <=
                GE,      // >=
                EQ,      // ==
                NE,      // !=
                LOGICAL_AND,  // &&
                LOGICAL_OR,   // ||

                MUL_ASSIGN,   // *=
                DIV_ASSIGN,   // /=
                MOD_ASSIGN,   // %=
                ADD_ASSIGN,   // +=
                SUB_ASSIGN,   // -=
                LEFT_ASSIGN,   // >>=
                RIGHT_ASSIGN,   // <<=
                AND_ASSIGN,   // &=
                XOR_ASSIGN,   // ^=
                OR_ASSIGN,   // |=

                ELLIPSIS,  // ...

                /* keywords */
                // type qualifier
                CONST,
                RESTRICT,
                VOLATILE,
                ATOMIC,  // _Atomic

                // type specifier
                VOID,
                CHAR,
                SHORT,
                INT,
                LONG,
                FLOAT,
                DOUBLE,
                SIGNED,
                UNSIGNED,
                BOOL,      // _Bool, C99开始提供_Bool，并在stdbool.h中提供bool这个宏
                COMPLEX,   // _Complex, from C99
                STRUCT,
                UNION,
                ENUM,

                // function specifier
                INLINE,
                NORETURN, // _Noreturn

                // alignment specifier
                ALIGNAS, // _Alignas

                // static conversion
                STATIC_ASSERT, // _Static_assert

                // storage class specifier
                TYPEDEF,
                EXTERN,
                STATIC,
                THREAD,     // _Thread_local
                AUTO,
                REGISTER, 


                // control
                BREAK,
                CASE,
                CONTINUE,
                DEFAULT,
                DO,
                ELSE,
                FOR,
                GOTO,
                IF,
                RETURN,
                SWITCH,
                WHILE, 

                // other
                SIZEOF,  
                ALINOF,  // _Alignof
                GENERIC, // _Generic
                Imaginary, // _Imageinary

                IDENTIFIER,
                CONSTANT,
                I_CONSTANT,  // int lie
                C_CONSTANT,
                F_CONSTANT,
                LITERAL,

                // 二义性
                POSTFIX_INC,
                POSTFIX_DEC,
                PREFIX_INC,
                PREFIX_DEC,
                ADDR,   // &
                DEREF,  // *


                // For preprocessor

                // error
                IGNORE,
                INVALID,
                END,
                NOTOK = -1,
            };

            static Token* New(const Token& t);

            Token(int tag, const std::string& raw, const SourceLocation& loc, bool ws)
                : tag_(tag), raw_(raw), loc_(loc), ws_(ws) {}

            std::string Value();
            int TokenType();

            std::string raw_; // 存放原始的字符串
            SourceLocation loc_;
            int tag_;
            bool ws_; // 表示token前是否有空字符

        private:
            explicit Token(int tag) : raw_(nullptr), loc_(nullptr), tag_(tag) {}
    };

    // 存放Token流
    class TokenSequence {
    public:
        void InsertBack(Token* t);
    private:
        std::vector<Token> tokens_;
    };
} }

#endif