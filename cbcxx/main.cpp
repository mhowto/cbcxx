#include <boost/lexical_cast.hpp>
#include <iostream>

int main() {
    std::cout << boost::lexical_cast<int>("123455") << std::endl;
    std::cin.get();
}