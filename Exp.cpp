#include "Exp.h"

Exp::~Exp() noexcept = default;

NumberExp::NumberExp(int v) : value(v){}
NumberExp::~NumberExp() noexcept = default;

BinaryExp::BinaryExp(Exp *l, BinaryOp o, Exp *r) : left(l), op(o), right(r){}
BinaryExp::~BinaryExp() noexcept {
    delete left;
    delete right;
}

Program::Program(StmList* sl_) : sl(sl_) {}
Program::~Program() {
    delete sl;
}

IdentifierExp::IdentifierExp(std::string id_) : id(std::move(id_)){}
IdentifierExp::~IdentifierExp() noexcept = default;

Stm::~Stm() noexcept = default;

AssignStatement::AssignStatement(std::string id_, Exp *exp_) : id(std::move(id_)), exp(exp_){}
AssignStatement::~AssignStatement() noexcept {
    delete exp;
}

PrinteoStatement::PrinteoStatement(string TypePrint_, Exp *exp_) : TypePrint(std::move(TypePrint_)), exp(exp_){}
PrinteoStatement::~PrinteoStatement() noexcept {
    delete exp;

}

StmList::StmList() : stms() {}
StmList::~StmList() {
    for(Stm* stm : stms) delete stm;
}

void StmList::add(Stm *stm) {
    stms.push_back(stm);
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