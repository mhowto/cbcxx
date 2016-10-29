#ifndef CBCXX_TOKEN_H
#define CBCXX_TOKEN_H

#include <string>
#include <list>
#include <ostream>
#include <set>

namespace cbcxx { namespace scanner {
    class Token;
    class Scanner;
    class Parser;

    typedef std::set<std::string> HideSet; // 用于记录tok的分析一路上经历的macro
    typedef std::list<const Token*> TokenList;

    struct SourceLocation {
        std::string fileName;
        const char* lineBegin;
        unsigned line;
        unsigned column;
    };

    class Token {
        friend class Scanner;
        friend std::ostream& operator<<(std::ostream& os, const Token& t);
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

                SINGLE_QUOTE = '\'',
                DOUBLE_QUOTE = '"',

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
                
                // 140
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
                // 151
                CONST,
                RESTRICT,
                VOLATILE,
                ATOMIC,  // _Atomic

                // type specifier
                // 155
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
                // 169
                INLINE,
                NORETURN, // _Noreturn

                // alignment specifier
                ALIGNAS, // _Alignas

                // static conversion
                STATIC_ASSERT, // _Static_assert

                // storage class specifier
                // 173
                TYPEDEF,
                EXTERN,
                STATIC,
                THREAD,     // _Thread_local
                AUTO,
                REGISTER, 

                // control
                // 179
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
                // 191
                SIZEOF,  
                ALIGNOF,  // _Alignof
                GENERIC, // _Generic
                IMAGINARY, // _Imaginary

                // 195
                IDENTIFIER,
                CONSTANT,
                I_CONSTANT,  // int lie
                C_CONSTANT,
                F_CONSTANT,
                LITERAL,

                // 二义性
                //201
                POSTFIX_INC,
                POSTFIX_DEC,
                PREFIX_INC,
                PREFIX_DEC,
                ADDR,   // &
                DEREF,  // *


                // For preprocessor

                // error
                // 207
                IGNORE,
                INVALID,
                END,
                NOTOK = -1,
            };

            static Token* New(const Token& t);
            static Token* New(int tag);
            static Token* New(int tag,
                const SourceLocation& loc,
                const std::string& str,
                bool ws = false);

            Token(int tag, const std::string& raw, const SourceLocation& loc, bool ws)
                : tag_(tag), raw_(raw), loc_(loc), ws_(ws) {}

            std::string Value() const;
            int TokenType();

            static bool IsKeyWord(int tag) {
                return CONST <= tag && tag < IDENTIFIER;
            }

            bool IsKeyWord() const;
            bool IsPunctuator() const;
            bool IsLiteral() const;
            bool IsConstant() const;
            bool IsIdentifier() const;
            bool IsEOF() const;
            bool IsTypeSpecQual() const;
            bool IsDecl() const;

            std::string raw_; // 存放原始的字符串
            SourceLocation loc_;
            int tag_;
            bool ws_; // 表示token前是否有空字符

            HideSet* hs_{ nullptr }; // 记录一路走来替换的macro

        private:
            explicit Token(int tag) : tag_(tag) {}
    };

    // 存放Token流.主要是作为Preprocessor的输入
    class TokenSequence {
        friend std::ostream& operator<<(std::ostream& os, const TokenSequence& ts);
        friend class Preprocessor;
    public:
        const TokenSequence& operator=(const TokenSequence& other);

        TokenSequence(): tokList_(new TokenList()),  begin_(tokList_->begin()), end_(tokList_->end()) {}

        explicit TokenSequence(Token* tok) {
            TokenSequence();
            InsertBack(tok);
        }

        explicit TokenSequence(TokenList* tokList): tokList_(tokList), begin_(tokList->begin()), end_(tokList->end()) {}
        TokenSequence(TokenList* tokList, TokenList::iterator begin, TokenList::iterator end): tokList_(tokList), begin_(begin), end_(end) {}
        TokenSequence(const TokenSequence& other) {
            *this = other;
        }
        const TokenSequence& operator=(const TokenSequence& other) {
            tokList_ = other.tokList_;
            begin_ = other.begin_;
            end_ = other.end_;
        }

        ~TokenSequence() {}

        // deep copy
        void Copy(const TokenSequence& other);

        // 更新Peek()位置上token
        void UpdateHeadLocation(const SourceLocation& loc);

        void FinalizeSubst(bool leadingWS, const HideSet& hs); // 给每个token的hs_加入hs

        // 符合则前进一个
        const Token* Expect(int expect);

        // 符合则前进一个
        bool Try(int tag);

        bool Test(int tag);

        const Token* Next(); // token流前移一个
        void PutBack(); // 撤销Next()、Try()等动作，即token流回退一个
        const Token* Peek();  // 拿当前的
        const Token* Peek2(); // 向前拿一个, token流会前移一个
        const Token* Back(); // 扫描队列结尾处拿一个,结尾回退一个
        void PopBack(); // 真正存储的tokList_去掉一个
        TokenList::iterator Mark();
        void ResetTo(TokenList::iterator mark);
        bool Empty();

        void InsertBack(const Token* t);
        void InsertBack(TokenSequence& ts);
        void InsertFront(TokenSequence& ts);
        void InsertFront(const Token* t);

        bool IsBeginOfLine() const;
        TokenSequence GetLine();
        void SetParser(Parser* parser);
        void Print() const;

    private:
        // Find an insert position with no preceding newline
        TokenList::iterator GetInsertFrontPos();

        TokenList* tokList_;
        TokenList::iterator begin_;
        TokenList::iterator end_;

        Parser* parser_{ nullptr }; // 不符合look ahead原则，可能需要反复穿插tokenize和parse过程
    };
} }

#endif