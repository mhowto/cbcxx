#include <boost/lexical_cast.hpp>
#include <iostream>
#include "ast_adapted.h"
#include "expression_def.h"

int main() {
    std::cout << boost::lexical_cast<int>("123455") << std::endl;
    std::cin.get();
}