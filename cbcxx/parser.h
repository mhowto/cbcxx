#ifndef CBCXX_PARSER_H
#define CBCXX_PARSER_H

#include "ast.h"
#include "token.h"

namespace cbcxx {
    namespace parser {

        using cbcxx::ast::ASTNode;
        using cbcxx::scanner::TokenSequence;

        class Parser {
        public:
            Parser(TokenSequence& ts) : ts_(ts) {}

            ASTNode* Parse();

        private:
            TokenSequence&  ts_;
        };
    }
}

#endif