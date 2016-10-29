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
            loc_ = {fileName, p_, line, column}; // struct 的列表初始化 list initialization
        }
        
        void Tokenize(TokenSequence& ts);   // 将结果写到ts中
        Token* Scan(bool ws=false);  // 获取下一个Token
        int Next();     // 获取下一个字符,改变scanner状态
        int Peek();     // 拿到下一个字符,不改变scanner状态
        bool Try(int c); // 尝试着看下一个字符是否符合预期
        bool Test(int c); // 不会改变状态
        bool Empty();
        void PutBack(); // 对Try做的回退一步
        void Mark();

        void SkipWhiteSpace();
        void SkipComment();
        Token* SkipNumber(); // 抓取数字 
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
        std::string* text_;        // scanner本次解析的文本流。文本可能很大，用指针
        SourceLocation loc_;       // 当前扫描到字符的位置
        Token tok_;                // 存放正在扫描的当前token
        const char* p_;            // 运行时数据，当前char的指针
        const char* tokLineBegin_; // 运行时数据,当前行首字符的指针
        unsigned tokColumn;        // 运行时数据,当前字符的列数字
    };
} }

#endif