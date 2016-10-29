#ifndef CBCXX_CPP_H
#define CBCXX_CPP_H

#include "token.h"
#include "scanner.h"

#include <map>
#include <string>
#include <stack>
#include <list>
#include <set>


namespace cbcxx {
    namespace preprocessor {
        using cbcxx::scanner::TokenSequence;
        using cbcxx::scanner::HideSet;
        using cbcxx::scanner::Token;

        class Macro;
        class CondDirective;

        typedef std::map<std::string, Macro> MacroMap;
        typedef std::list<std::string> ParamList;
        typedef std::map<std::string, TokenSequence> ParamMap;
        typedef std::stack<CondDirective> PPCondStack;
        typedef std::list<std::string> PathList;

        class Macro {
        public:
            Macro(const TokenSequence& reqSeq, bool preDef=false)
                : funcLike_(false), variadic_(false),
                preDef_(preDef), reqSeq_(reqSeq) {}

            Macro(bool variadic, ParamList& params,
                TokenSequence& reqSeq, bool preDef=false)
                : funcLike_(true), variadic_(variadic), preDef_(preDef),
                params_(params), reqSeq_(reqSeq) {}

            bool FuncLike() {
                return variadic_;
            }

            bool Variadic() {
                return variadic_;
            }

            bool PreDef() {
                return preDef_;
            }

            ParamList& Params() {
                return params_;
            }

        private:
            bool funcLike_;
            bool variadic_;
            bool preDef_;
            ParamList params_;
            TokenSequence reqSeq_;
        };

        class Preprocessor {
        public:
            Preprocessor() {
                Init();
            }

            ~Preprocessor() {}

            void Process(TokenSequence& os);  // 处理指定的inFile,结果存入os

            void Expand(TokenSequence& os, TokenSequence& is, bool inCond = false); // 将is的token流扩展进os中 
            void Finalize(TokenSequence& os);  // 替换部分identifier为keyword

            void Subst(TokenSequence& os, TokenSequence& is,
                bool leadingWS, const HideSet& hs, ParamMap& params);  // 用在Expand中， 当Expand中的is发现某macro已定义，将这个macro用它的定义subst掉

            /* 处理##
             * #define FUNC_STAT () {return;}
             * #define GEN_FUNC(x) #void ## x ## FUNCSTAT
             * GEN_FUNC(foo)
             * #void ## 的空格得以保留
             */
            void Glue(TokenSequence& os, TokenSequence is);  //处理##

            void Glue(TokenSequence& os, const Token* tok);

            const Token* ParseActualParam(TokenSequence& is, Macro* macro, ParamMap& paramMap);
            int GetDirective(TokenSequence& is); // 判断is第一个元素是不是directive
            void ReplaceDefOp(TokenSequence& is); // 处理defined()
            void ReplaceIdent(TokenSequence& is); // 处理 #if #else 中不满足条件的那一堆代码
            void ParseDirective(TokenSequence& os, TokenSequence& is, int directive); // Preprocessor 的起始
            void ParseIf(TokenSequence ls);
            void ParseIfdef(TokenSequence ls);
            void ParseIfndef(TokenSequence ls);

            void IncludeFile(TokenSequence& is, const std::string& fileName);  // 从指定fileName的文件中读出token流存入is中

            const std::string* SearchFile(const std::string& fileName, bool , bool next, const std::string* curPath=nullptr);
            void AddSearchPath(const std::string&);
            void AddMacro(const std::string&, std::string* text, bool preDef = false);
            void AddMacro(const std::string&, const Macro&);
        private:
            void Init();
            
            MacroMap macroMap_;
            PathList searchPathList_;
        };
    }
}


#endif