cmake_minimum_required(VERSION 3.26)
project(CompiladoresProyecto)

set(CMAKE_CXX_STANDARD 17)

add_executable(CompiladoresProyecto main.cpp
        Token.cpp
        Scanner.cpp
        Exp.cpp
        parser.cpp
        visitor.cpp
)
