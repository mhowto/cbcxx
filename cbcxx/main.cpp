#include <iostream>
#include <fstream>
#include <streambuf>

#include "token.h"
#include "scanner.h"
#include "parser.h"

using cbcxx::scanner::Scanner;
using cbcxx::scanner::TokenSequence;
using cbcxx::parser::Parser;

int main() {
    std::ifstream t("../cbcxx/test/aftercpp/helloworld.c");
    std::string inputStr(
        (std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

    Scanner scanner(&inputStr);
    TokenSequence ts;
    scanner.Tokenize(ts);

    std::cout << ts;
    std::cin.get();
}