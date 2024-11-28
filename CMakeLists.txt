cmake_minimum_required(VERSION 3.26)
project(CompiladoresProyecto)

set(CMAKE_CXX_STANDARD 17)

add_executable(CompiladoresProyecto svm.cpp svm_parser.cpp svm_run.cpp)

#add_executable(CompiladoresProyecto main.cpp Token.cpp Scanner.cpp Exp.cpp parser.cpp visitor.cpp imp_value.h imp_value.cpp imp_value_visitor.h imp_interpreter.h imp_interpreter.cpp environment.h imp_codegen.cpp imp_type.cpp imp_type_checker.cpp SinComentarios.cpp)
