cmake_minimum_required(VERSION 3.6)
project(cbcxx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Enable V3 of Phoenix by default. V2 has some issues with C++11
# compilers.
add_definitions( -DBOOST_SPIRIT_USE_PHOENIX_V3=1 )

find_package( Boost COMPONENTS REQUIRED )

set(SOURCE_FILES
    cbcxx/ast.h
    cbcxx/expression.h
    cbcxx/main.cpp
    cbcxx/parser.h)

add_executable(cbcxx ${SOURCE_FILES})