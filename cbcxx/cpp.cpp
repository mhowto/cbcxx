#include "cpp.h"
#include "scanner.h"
#include <ctime>
#include <cstdio>
#include <cassert>

//extern std::string inFileName;

namespace cbcxx {
    namespace preprocessor {
        using cbcxx::scanner::Scanner;
        static std::string* Date() {
            std::time_t result = std::time(nullptr);
            std::tm* tm = std::localtime(&result);
            char* buf = new char[14];
            std::strftime(buf, 14, "\"%a %M %Y\"", tm);
            auto ret = new std::string(buf);
            delete[] buf;
            return ret;
        }

        void Preprocessor::AddSearchPath(const std::string& path) {
            searchPathList_.push_back(path);
        }

        void Preprocessor::AddMacro(const std::string & name, std::string * text, bool preDef)
        {
            TokenSequence ts;
            Scanner scanner(text);
            scanner.Tokenize(ts);
            Macro macro(ts, preDef);
            AddMacro(name, macro);
            macroMap_.insert(MacroMap::value_type(name, macro));
        }

        void Preprocessor::AddMacro(const std::string& name, const Macro& macro)
        {
            auto res = macroMap_.find(name);
            if (res != macroMap_.end()) {
                macroMap_.erase(name);
            }
            macroMap_.insert(std::make_pair(name, macro));
        }

        void Preprocessor::Init()
        {
            AddSearchPath("/usr/local/cbcxx/include/");
            AddSearchPath("/usr/include/");
            AddSearchPath("/usr/include/linux/");
            AddSearchPath("/usr/include/x86_64-linux-gnu/");
            AddSearchPath("/usr/local/include/");

            AddMacro("__FILE__", Macro(TokenSequence(), true));
            AddMacro("__LINE__", Macro(TokenSequence(), true));

            AddMacro("__DATE__", Date(), true);
            AddMacro("__STDC__", new std::string("1"), true);
            AddMacro("__STDC_HOSTED__", new std::string("0"), true);
            AddMacro("__STDC_VERSION__", new std::string("201103L"), true);
        }


        /**
         * Entry function of preprocessing.
         * @param os output token sequence
         */
        void Preprocessor::Process(TokenSequence& os) {
            TokenSequence is;
            //IncludeFile(is, inFileName);
            
            auto defaultIncludeFile = SearchFile("cbcxx.h", true, false);
            assert(defaultIncludeFile != nullptr);
            Expand(os, is);
            Finalize(os);
        }

        /**
         * Expand marco in input token sequence.
         * @param os stores output token sequence
         * @param is stores input token sequence
         * @param inCond the flag of conditional syntax
         */
        void Preprocessor::Expand(TokenSequence& os, TokenSequence& is, bool inCond)
        {
            while (!is.Empty()) {
                auto tok = is.Peek();
            }
        }


        void Preprocessor::Finalize(TokenSequence& os)
        {
        }

        /**
         * SearchFile searches the path of the file.
         * @param fileName file's name
         * @param libHeader true for <fileName> and false for "fileName"
         * @param next true for #include_next semantic, false for #include semantic
         * @param curPath current path
         */
        const std::string* Preprocessor::SearchFile(
            const std::string& fileName, 
            bool libHeader, 
            bool next, 
            const std::string* curPath)
        {
            return nullptr;
        }
    }
}
