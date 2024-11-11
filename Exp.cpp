#include "Exp.h"

NumberExp::NumberExp(int v) : value(v){}
NumberExp::~NumberExp() noexcept {}

BinaryExp::BinaryExp(Exp *l, BinaryOp o, Exp *r) : left(l), op(o), right(r){}
BinaryExp::~BinaryExp() noexcept {
    delete left;
    delete right;
}

Program::Program(Exp *r) : result(r) {}
Program::~Program() {
    delete result;
}


string Exp::BinaryToChar(BinaryOp op) {

    string operacion;

    switch (op) {
        case PLUS_OP: operacion = "+"; break;
        case MINUS_OP: operacion = "-"; break;
        case MUL_OP: operacion = "*"; break;
        case DIV_OP : operacion = "/"; break;
        default: operacion = "$"; break;
    }

    return operacion;

}