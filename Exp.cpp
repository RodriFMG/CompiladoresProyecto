#include "Exp.h"

#include <utility>

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

IfStatement::IfStatement(list<Exp *> conditions_, list<StmList*> conditionBodies_) : conditions(std::move(conditions_)), conditionBodies(std::move(conditionBodies_)){}
IfStatement::~IfStatement() noexcept {
    for(Exp* condition : conditions) delete condition;
    for(StmList* condicional : conditionBodies) delete condicional;
}

ForStatement::ForStatement(std::string id_, string i_o_d , Exp *exp1_, Exp *exp2_, StmList* stms_) : id(std::move(id_)), increase_or_decrease(std::move(i_o_d)),
exp1(exp1_), exp2(exp2_), stms(stms_){}
ForStatement::~ForStatement() noexcept {
    delete exp1;
    delete exp2;
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
        case LE_OP: operacion = "<"; break;
        case LT_OP: operacion = "<="; break;
        case DE_OP: operacion = ">"; break;
        case DT_OP: operacion = ">="; break;
        case EQ_OP: operacion = "="; break;
        default: operacion = "$"; break;
    }

    return operacion;

}