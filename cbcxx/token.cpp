#include "token.h"
#include "cassert"
#include <iostream>
#include "parser.h"

namespace cbcxx {
    namespace scanner {
        Token* Token::New(const Token& t) {
            return new Token(t.tag_, t.raw_, t.loc_, t.ws_);
        }

        Token* Token::New(int tag) {
            return new Token(tag);
        }

        Token* Token::New(int tag, const SourceLocation& loc, const std::string& str, bool ws) {
            return new Token(tag, str, loc, ws);
        }

        std::string Token::Value() const
        {
            return this->raw_;
        }
        
        bool Token::IsKeyWord() const {
            return IsKeyWord(tag_);
        }

        bool Token::IsPunctuator() const {
            return 0 <= tag_ && tag_ <= ELLIPSIS;
        }

        bool Token::IsLiteral() const {
            return tag_ == LITERAL;
        }

        bool Token::IsConstant() const {
            return CONSTANT <= tag_ && tag_ <= F_CONSTANT;
        }

        bool Token::IsIdentifier() const {
            return tag_ == IDENTIFIER;
        }

        bool Token::IsEOF() const {
            return tag_ == EOF;
        }

        bool Token::IsTypeSpecQual() const {
            return CONST <= tag_ && tag_ <= ENUM;
        }

        bool Token::IsDecl() const {
            return CONST <= tag_ && tag_ <= REGISTER;

        }

        void TokenSequence::InsertBack(const Token* t) {
            auto pos_ = tokList_->insert(end_, t);
            if (begin_ == end_) {
                begin_ = pos_;
            }
        }

        void TokenSequence::Copy(const TokenSequence& other) {
            tokList_ = new TokenList(other.begin_, other.end_);
            begin_ = tokList_->begin();
            end_ = tokList_->end();
            
            // TokenList 的类型是list<const Token*>， 深拷贝要求每一项都拷贝
            for (auto iter = begin_; iter != end_; ++iter) {
                *iter = Token::New(**iter);
            }
        }

        void TokenSequence::UpdateHeadLocation(const SourceLocation& loc) {
            assert(!Empty());
            auto tok = const_cast<Token*>(Peek());
            tok->loc_ = loc;
        }

        void TokenSequence::FinalizeSubst(bool leadingWS, const HideSet& hs) {
            while (!this->Empty()) {
                auto tok = const_cast<Token*>(this->Next());
                if (!tok->hs_) {
                    tok->hs_ = new HideSet(hs);
                }
                else {
                    tok->hs_->insert(hs.begin(), hs.end());
                }
            }
            const_cast<Token*>(Peek())->ws_ = leadingWS;
        }

        const Token* TokenSequence::Expect(int expect) {
            auto tok = Peek();
            if (!Try(expect)) {
                std::cout << "expect " << expect << "error!";
                exit(0);
            }
            return tok;
        }

        bool TokenSequence::Try(int tag) {
            if (Peek()->tag_ == tag) {
                Next();
                return true;
            }
            return false;
        }

        bool TokenSequence::Test(int tag) {
            return Peek()->tag_ == tag;
        }

        const Token* TokenSequence::Next() {
            auto tok = Peek();
            if (!tok->IsEOF()) {
                ++begin_;
                // Peek();
            }
            return tok;
        }

        void TokenSequence::PutBack() {
            assert(begin_ != tokList_->begin());
            --begin_;
            if ((*begin_)->tag_ == Token::NEW_LINE) {
                PutBack();
            }
        }

        const Token* TokenSequence::Peek() {
            static auto eof = Token::New(Token::END);
            if (begin_ != end_ && (*begin_)->tag_ == Token::NEW_LINE) {
                ++begin_;
                return Peek();
            }
            else if (begin_ == end_) {
                if (end_ != tokList_->begin()) {
                    *eof = *Back();
                }
                eof->tag_ = Token::END;
                return eof;
            }
            else if (parser_ && (*begin_)->tag_ == Token::IDENTIFIER
                && (*begin_)->raw_ == "__func__") {
                // __func__: 参考 6.4.2.2, 替换成当前函数名
                auto funcName = Token::New(*(*begin_));
                funcName->tag_ = Token::LITERAL;
                //funcName->raw_ = "\"" + parser_->CurFunc()->Name() + "\"";
                *begin_ = funcName;
            }
            return *begin_;
        }

        const Token* TokenSequence::Back() {
            auto back = end_;
            return *(--back);
        }

        void TokenSequence::PopBack() {
            // 对存储内存而非指针做pop
            assert(!Empty());
            assert(end_ == tokList_->end());
            auto size_eq1 = tokList_->back() == *begin_;
            tokList_->pop_back();
            end_ = tokList_->end();
            if (size_eq1) {
                begin_ = end_;
            }
        }

        bool TokenSequence::Empty() {
            return Peek()->tag_ == Token::END;
        }

        const Token* TokenSequence::Peek2() {
            auto tok = Peek();
            if (tok->tag_ != Token::END) {
                Next();
                tok = Peek();
                PutBack(); 
            }
            return tok;
        }

        std::ostream& operator<<(std::ostream& os, const Token& t) {
            switch (t.tag_) {
            case(Token::LPAR): os << "("; break;
            case(Token::RPAR): os << ")"; break;
            case(Token::LSQB): os << "["; break;
            case(Token::RSQB): os << "]"; break;
            case(Token::LBRACE): os << "{"; break;
            case(Token::RBRACE): os << "}"; break;
            case(Token::COMMA): os << ","; break;
            case(Token::SEMI): os << ";"; break;
            case(Token::ADD): os << "+"; break;
            case(Token::SUB): os << "-"; break;
            case(Token::MUL): os << "*"; break;
            case(Token::DIV): os << "/"; break;
            case(Token::OR): os << "|"; break;
            case(Token::AND): os << "&"; break;
            case(Token::XOR): os << "^"; break;
            case(Token::TILDE): os << "~"; break;
            case(Token::NOT): os << "!"; break;
            case(Token::COND): os << "?"; break;
            case(Token::LESS): os << "<"; break;
            case(Token::GREATER): os << ">"; break;
            case(Token::ASSIGN): os << "="; break;
            case(Token::DOT): os << "."; break;
            case(Token::MOD): os << "%"; break;
            case(Token::SHARP): os << "#"; break;
            case(Token::NEW_LINE): os << "\n"; break;
            case(Token::DSHARP): os << "##"; break;
            case(Token::PTR): os << "->"; break;
            case(Token::INC): os << "++"; break;
            case(Token::DEC): os << "--"; break;
            case(Token::LEFT): os << ">>"; break;
            case(Token::RIGHT): os << "<<"; break;
            case(Token::LE): os << "<="; break;
            case(Token::GE): os << ">="; break;
            case(Token::EQ): os << "=="; break;
            case(Token::NE): os << "!="; break;
            case(Token::LOGICAL_AND): os << "&&"; break;
            case(Token::LOGICAL_OR): os << "||"; break;
            case(Token::MUL_ASSIGN): os << "*="; break;
            case(Token::DIV_ASSIGN): os << "/="; break;
            case(Token::MOD_ASSIGN): os << "%="; break;
            case(Token::ADD_ASSIGN): os << "+="; break;
            case(Token::SUB_ASSIGN): os << "-="; break;
            case(Token::LEFT_ASSIGN): os << ">>="; break;
            case(Token::RIGHT_ASSIGN): os << "<<="; break;
            case(Token::AND_ASSIGN): os << "&="; break;
            case(Token::XOR_ASSIGN): os << "^="; break;
            case(Token::OR_ASSIGN): os << "|="; break;
            case(Token::ELLIPSIS): os << "..."; break;
            case(Token::CONST): os << "const"; break;
            case(Token::RESTRICT): os << "restrict"; break;
            case(Token::VOLATILE): os << "volatile"; break;
            case(Token::ATOMIC): os << "_Atomic"; break;
            case(Token::VOID): os << "void"; break;
            case(Token::CHAR): os << "char"; break;
            case(Token::SHORT): os << "short"; break;
            case(Token::INT): os << "int"; break;
            case(Token::LONG): os << "long"; break;
            case(Token::FLOAT): os << "float"; break;
            case(Token::DOUBLE): os << "double"; break;
            case(Token::SIGNED): os << "signed"; break;
            case(Token::UNSIGNED): os << "unsigned"; break;
            case(Token::BOOL): os << "_Bool"; break;
            case(Token::COMPLEX): os << "_Complex"; break;
            case(Token::STRUCT): os << "struct"; break;
            case(Token::UNION): os << "union"; break;
            case(Token::ENUM): os << "enum"; break;
            case(Token::INLINE): os << "inline"; break;
            case(Token::NORETURN): os << "noreturn"; break;
            case(Token::ALIGNAS): os << "_Alignas"; break;
            case(Token::STATIC_ASSERT): os << "_Static_assert"; break;
            case(Token::TYPEDEF): os << "typedef"; break;
            case(Token::EXTERN): os << "extern"; break;
            case(Token::STATIC): os << "static"; break;
            case(Token::THREAD): os << "_Thread_local"; break;
            case(Token::AUTO): os << "auto"; break;
            case(Token::REGISTER): os << "register"; break;
            case(Token::BREAK): os << "break"; break;
            case(Token::CASE): os << "case"; break;
            case(Token::CONTINUE): os << "continue"; break;
            case(Token::DEFAULT): os << "default"; break;
            case(Token::DO): os << "do"; break;
            case(Token::ELSE): os << "else"; break;
            case(Token::FOR): os << "for"; break;
            case(Token::GOTO): os << "goto"; break;
            case(Token::IF): os << "if"; break;
            case(Token::RETURN): os << "return"; break;
            case(Token::SWITCH): os << "switch"; break;
            case(Token::WHILE): os << "while"; break;
            case(Token::SIZEOF): os << "sizeof"; break;
            case(Token::ALIGNOF): os << "_Alignof"; break;
            case(Token::GENERIC): os << "_Generic"; break;
            case(Token::IMAGINARY): os << "_Imaginary"; break;
            case(Token::IGNORE): case(Token::INVALID): case(Token::END): case(Token::NOTOK): break;
            default: os << t.Value();
            }
            return os;
        }

        std::ostream& operator<<(std::ostream & os, const TokenSequence & ts) {
            for (auto& t : *ts.tokList_) {
                os << *t << " ";
            }

            return os;
        }
    }
}