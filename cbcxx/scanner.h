#ifndef CBCXX_SCANNER_H
#define CBCXX_SCANNER_H

#include "token.h"
#include "encoding.h"

namespace cbcxx {namespace scanner {
    class Scanner {
    public:
        //Scanner(std::string* text) : Scanner(text, "literal") {}
        explicit Scanner(
            std::string* text,
            std::string fileName="",
            unsigned line=1,
            unsigned column=1) : text_(text), tok_(Token::END)
        {
            p_ = &(*text_)[0];
            loc_ = {fileName, p_, line, column}; // struct ���б��ʼ�� list initialization
        }
        
        void Tokenize(TokenSequence& ts);   // �����д��ts��
        Token* Scan(bool ws=false);  // ��ȡ��һ��Token
        int Next();     // ��ȡ��һ���ַ�,�ı�scanner״̬
        int Peek();     // �õ���һ���ַ�,���ı�scanner״̬
        bool Try(int c); // �����ſ���һ���ַ��Ƿ����Ԥ��
        bool Test(int c); // ����ı�״̬
        bool Empty();
        void PutBack(); // ��Try���Ļ���һ��
        void Mark();

        void SkipWhiteSpace();
        void SkipComment();
        Token* SkipNumber(); // ץȡ���� 
        Token* SkipIdentifier();
        Token* SkipCharacter();
        Token* SkipLiteral();

        int ScanEscaped();
        int ScanUCN(int len);
        int ScanOctEscaped(int c);
        int ScanHexEscaped();
        Encoding ScanEncoding(int c);

        bool isUCN(int c) { return c == '\\' && (Test('u') || Test('U')); }
        bool isOctal(int c);
        int XDigit(int c);

        Token* MakeToken(int tag);

    private:
        std::string* text_;        // scanner���ν������ı������ı����ܴܺ���ָ��
        SourceLocation loc_;       // ��ǰɨ�赽�ַ���λ��
        Token tok_;                // �������ɨ��ĵ�ǰtoken
        const char* p_;            // ����ʱ���ݣ���ǰchar��ָ��
        const char* tokLineBegin_; // ����ʱ����,��ǰ�����ַ���ָ��
        unsigned tokColumn;        // ����ʱ����,��ǰ�ַ���������
    };
} }

#endif