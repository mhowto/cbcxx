#include "token.h"

namespace cbcxx { namespace scanner {
    Token* Token::New(const Token& t) {
        return new Token(t.tag_, t.raw_, t.loc_, t.ws_);
    }
} }