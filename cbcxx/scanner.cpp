#include "scanner.h"
#include <cassert>

namespace cbcxx { namespace scanner {
    void Scanner::Tokenize(TokenSequence& ts) {
        while (true) {
            Token* t = Scan();
            if (t->tag_ == Token::END)
            {
                break;
            }
            else
            {
                ts.InsertBack(t);
            }
        }
    }

    Token* Scanner::Scan(bool ws) {
        int c = Next();
        if ((c >= 'a' && c <= 't') || (c >= 'v' && c <= 'z') ||
            (c >= 'A' && c <= 'K') || (c >= 'M' && c <= 'T') || (c >= 'V' && c <= 'Z') ||
            (c >= 0x80 && c <= 0xfd)) {
            return SkipIdentifier();
        }
        switch (c) {
        case '(': case ')': case '[': case ']':
        case '{': case '}': case '?': case ',':
        case '~': case ';':
            return MakeToken(c);
        case ':':
            return (Try('=') ? MakeToken(']') : MakeToken(c));
        case '+': 
            if (Try('=')) return MakeToken(Token::ADD_ASSIGN);
            if (Try('+')) return MakeToken(Token::INC);
            return MakeToken(c);
        case '-': 
            if (Try('=')) return MakeToken(Token::SUB_ASSIGN);
            if (Try('>')) return MakeToken(Token::PTR);
            if (Try('-')) return MakeToken(Token::DEC);
            return MakeToken(c);
        case '*':
            return (Try('=') ? MakeToken(Token::MUL_ASSIGN) : MakeToken(c)); 
        case '/':
            if (Test('/') || Test('*')) {
                SkipComment();
                return Scan(true);
            }
            return (Try('=') ? MakeToken(Token::DIV_ASSIGN) : MakeToken(c));
        case '|':
            if (Try('|')) return MakeToken(Token::LOGICAL_OR);
            if (Try('=')) return MakeToken(Token::OR_ASSIGN);
            return MakeToken(c);
        case '&':
            if (Try('&')) return MakeToken(Token::LOGICAL_AND);
            if (Try('=')) return MakeToken(Token::AND_ASSIGN);
            return MakeToken(c);
        case '^':
            return (Try('=') ? MakeToken(Token::XOR_ASSIGN) : MakeToken(c));
        case '!':
            return (Try('=') ? MakeToken(Token::NE) : MakeToken(c));
        case '<':
            if (Try('=')) return MakeToken(Token::LE);
            if (Try('<')) return (Try('=') ? MakeToken(Token::LEFT_ASSIGN) : MakeToken(Token::LEFT));
            if (Try(':')) return MakeToken(Token::LSQB);
            if (Try('%')) return MakeToken(Token::LBRACE);
            return MakeToken(c);
        case '>':
            if (Try('=')) return MakeToken(Token::GE);
            if (Try('>')) return (Try('=') ? MakeToken(Token::RIGHT_ASSIGN) : MakeToken(Token::RIGHT));
            return MakeToken(c);
        case '=':
            return (Try('=') ? MakeToken(Token::EQ) : MakeToken(c));
        case '.':
            if (isdigit(Peek())) return SkipNumber();
            if (Try('.')) {
                if (Try('.')) return MakeToken(Token::ELLIPSIS);
                PutBack();
                return MakeToken('.');
            }
            return MakeToken(c);
        case '%':
            if (Try('>')) return MakeToken('}');
            if (Try('=')) return MakeToken(Token::MOD_ASSIGN);
            if (Try(':')) {
                if (Try('%')) {
                    if (Try(':')) return MakeToken(Token::DSHARP);
                    PutBack();
                }
                return MakeToken('#');
            }
            return MakeToken(c);
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return SkipNumber();
        case 'u': case 'U': case 'L': {
            ScanEncoding(c);
            if (Try('\'')) return SkipCharacter();
            if (Try('\"')) return SkipLiteral();
            return SkipIdentifier();
        }
        case '\\':
            if (Test('u') || Test('U'))
                return SkipIdentifer();
            return MakeToken(Token::INVALID);
        case '\0': return MakeToken(Token::END);
        default: return MakeToken(Token::INVALID);
        }
    }
    
    // 既拿数据，也改变状态
    int Scanner::Next() {
        int c = Peek();
        ++p_;
        if (c == '\n') {
            ++loc_.line;
            loc_.column = 1;
            loc_.lineBegin = p_;
        }
        else {
            ++loc_.column;
        }
        return c;
    }

    // 一般情况下，只拿数据不改变状态
    int Scanner::Peek() {
        int c = *p_;
        if (c == '\\' && p_[1] == '\n') {
            p_ += 2;
            ++loc_.line;
            loc_.column = 1;
            loc_.lineBegin = p_;
            return Peek();
        }
        return c;
    }

    bool Scanner::Try(int c) {

    }

    bool Scanner::Test(int c) {
        return Peek() == c;
    }

    bool Scanner::Empty() {
        return *p_ == 0;
    }

    Token* Scanner::MakeToken(int tag) {
        tok_.tag_ = tag;
        auto& str = tok_.raw_;
        str.resize(0);
        const char* p = tok_.loc_.lineBegin + tok_.loc_.column - 1;
        for (; p != p; p++) {
            if (p[0] == '\n' && p[-1] == '\\') {
                str.pop_back();
            }
            else {
                str.push_back(*p);
            }
        }
        
        return Token::New(tok_);
    }

    void Scanner::SkipComment() {
        if (Try('/')) {
            while (!Empty()) {
                if (Peek() == '\n')
                    return;
                Next();
            }
            return;
        }
        else if (Try('*')) {
            while (!Empty()) {
                int c = Next();
                if (c == '*' && Peek() == '/') {
                    Next();
                    return;
                }
            }
        }
        assert(false);
    }

    Token* Scanner::SkipNumber() {
        bool sawHexPrefix = false;
        int c = Next();
        int tag = Token::I_CONSTANT;
        while ((c == '.') || isdigit(c) || isUCN(c) || isalpha(c) || c == '_') {
            if (c == 'e' || c == 'E' || c == 'p' || c == 'P') {
                // P(C11 C++17)含义: 0x1.99p-3 = 0b1.10011001 *2 ^ -3 
                if (!Try('-')) Try('+');
                if (sawHexPrefix && (c == 'p' || c == 'P')) {
                    tag = Token::F_CONSTANT;
                }
            }
            else if (isUCN(c)) {
                ScanEscaped();
            }
            else if (c == '.') {
                tag = Token::F_CONSTANT;
            }
            else if (c == 'x' || c == 'X') {
                sawHexPrefix = true;
            }
            c = Next();
        }
        return Token::New(tok_);
    }

    int Scanner::ScanEscaped() {
        int c = Next();
        switch (c) {
        case '\\': case '\'': case '\"': case '\?':
            return c;
        case 'a': return '\a';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 'v': return '\v';
        // Non-standard GCC extention
        case 'e': return '\033';
        case 'x': return ScanHexEscaped();
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7': 
            return ScanOctEscaped(c);
        case 'u': return ScanUCN(4);
        case 'U': return ScanUCN(8);
        default: assert(false);
        }
        return c;
    }

    int Scanner::ScanUCN(int len) {
        assert(len == 4 || len == 8);
        int val = 0;
        for (int i = 0; i < len; i++) {
            int c = Next();
            if (!isxdigit(c)) 
                assert(false);
            val = (val << 4) + XDigit(c);
        }
        return val;
    }

    int Scanner::ScanHexEscaped() {
        int val = 0;
        int c = Peek();
        while (isxdigit(c)) {
            val = (val << 4) + XDigit(c);
            Next();
            c = Peek();
        }
        return val;
    }
    
    int Scanner::ScanOctEscaped(int c) {
        int val = XDigit(c);
        c = Peek();
        if (!isOctal(c)) {
            return val;
        }
        val = (val << 3) + XDigit(c);
        Next();

        c = Peek();
        if (!isOctal)
            return val;
        val = (val << 3) + XDigit(c);
        Next();
        return val;
    }

    int Scanner::XDigit(int c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }
        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        assert(false);
        return c;
    }

    int Scanner::isOctal(int c) {
        return (c >= '0' && c <= '7');
    }

    Encoding Scanner::ScanEncoding(int c) {
        switch (c) {
        case 'u': return (Try('8') ? Encoding::UTF8 : Encoding::CHAR16);
        case 'U': return Encoding::CHAR32;
        case 'L': return Encoding::WCHAR;
        default: assert(false); return Encoding::NONE;
        }
    }

    Token* Scanner::SkipCharacter() {
        int c = Next();
        while (c != '\'' && c != '\n' && c != '\0') {
            if (c == '\\') Next();
            c = Next();
        }
        if (c != '\'') {
            assert(false);
        }
        return MakeToken(Token::C_CONSTANT);
    }

    Token* Scanner::SkipLiteral() {
        int c = Next();
        while (c != '\"' && c != '\n' && c != '\0') {
            if (c == '\\') Next();
            c = Next();
        }
        if (c != '\"') {
            assert(false);
        }
        return MakeToken(Token::LITERAL);
    }

    Token* Scanner::SkipIdentifier() {
        PutBack();
        int c = Next();
        while (isalnum(c)
            || (c >= 0x80 && c <= 0xfd) // UTF8下多字节编码
            || c == '_'
            || c == '$'
            || isUCN(c)) {
            if (isUCN(c))
                c = ScanEscaped();
            c = Next();
        }
        PutBack();
        return MakeToken(Token::IDENTIFIER);
    }
} }