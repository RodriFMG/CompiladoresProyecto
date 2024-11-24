#include "Exp.h"

Exp::~Exp() noexcept = default;

NumberExp::NumberExp(int v) : value(v) {}
NumberExp::~NumberExp() noexcept = default;

BoolExp::BoolExp(int v) : value(v){}
BoolExp:: ~BoolExp() noexcept = default;

BinaryExp::BinaryExp(Exp *l, BinaryOp o, Exp *r) : left(l), op(o), right(r) {}
BinaryExp::~BinaryExp() noexcept {
    delete left;
    delete right;
}

IdentifierExp::IdentifierExp(std::string id_) : id(std::move(id_)) {}
IdentifierExp::~IdentifierExp() noexcept = default;

FCallExp::FCallExp(string fname, list<Exp*> args) : fname(std::move(fname)), args(std::move(args)) {}
FCallExp::~FCallExp() noexcept {
    for (Exp* arg : args) {
        delete arg;
    }
}

Stm::~Stm() noexcept = default;

AssignStatement::AssignStatement(std::string id_, Exp *exp_) : id(std::move(id_)), exp(exp_) {}
AssignStatement::~AssignStatement() noexcept {
    delete exp;
}

PrinteoStatement::PrinteoStatement(string TypePrint_, Exp *exp_) : TypePrint(std::move(TypePrint_)), exp(exp_) {}
PrinteoStatement::~PrinteoStatement() noexcept {
    delete exp;
}

IfStatement::IfStatement(list<Exp*> conditions_, list<StmList*> conditionBodies_)
        : conditions(std::move(conditions_)), conditionBodies(std::move(conditionBodies_)) {}

IfStatement::~IfStatement() noexcept {
    for (Exp* condition : conditions) delete condition;
    for (StmList* condicional : conditionBodies) delete condicional;
}

ForStatement::ForStatement(std::string id_, string i_o_d, Exp *exp1_, Exp *exp2_, StmList* stms_)
        : id(std::move(id_)), increase_or_decrease(std::move(i_o_d)), exp1(exp1_), exp2(exp2_), stms(stms_) {}

ForStatement::~ForStatement() noexcept {
    delete exp1;
    delete exp2;
    delete stms;
}

WhileStatement::WhileStatement(Exp *exp_, StmList *stms_) : exp(exp_), stms(stms_) {}
WhileStatement::~WhileStatement() noexcept {
    delete exp;
    delete stms;
}


StmList::StmList() : stms() {}
StmList::~StmList() {
    for (Stm* stm : stms) delete stm;
}

void StmList::add(Stm *stm) {
    stms.push_back(stm);
}

FCallStatement::FCallStatement(string fname, list<Exp*> args) : fname(fname), args(args) {}
FCallStatement::~FCallStatement() {
    while (!args.empty()) {
        delete args.front();
        args.pop_front();
    }
}


VarDec::VarDec(string type, list<string> vars) : type(std::move(type)), vars(std::move(vars)) {}
VarDec::~VarDec() = default;

VarDecList::VarDecList() : vardecs() {}
void VarDecList::add(VarDec* v) {
    vardecs.push_back(v);
}
VarDecList::~VarDecList() {
    for (auto v : vardecs) delete v;
}

FunDec::FunDec(string fname, list<string> paramTypes, list<string> paramNames, string rtype, Body* b)
        : fname(std::move(fname)), paramTypes(std::move(paramTypes)), paramNames(std::move(paramNames)), rtype(std::move(rtype)), body(b) {}

FunDec::~FunDec() {
    delete body;
}

FunDecList::FunDecList() : flist() {}
void FunDecList::add(FunDec* f) {
    flist.push_back(f);
}
FunDecList::~FunDecList() {
    for (auto f : flist) delete f;
}


Body::Body(VarDecList* v, StmList* s) : vardecs(v), statements(s) {}
Body::~Body() {
    delete vardecs;
    delete statements;
}


Program::Program(VarDecList* varDecs, FunDecList* funDecs, StmList* stmList, string NameProgram_, bool FirstFunctions_)
        : varDecs(varDecs), funDecs(funDecs), stmList(stmList), NameProgram(std::move(NameProgram_)),
        FirstFunctions(FirstFunctions_){}

Program::~Program() {
    delete varDecs;
    delete funDecs;
    delete stmList;
}


string Exp::BinaryToChar(BinaryOp op) {
    string operacion;
    switch (op) {
        case PLUS_OP: operacion = "+"; break;
        case MINUS_OP: operacion = "-"; break;
        case MUL_OP: operacion = "*"; break;
        case DIV_OP: operacion = "/"; break;
        case LE_OP: operacion = "<"; break;
        case LT_OP: operacion = "<="; break;
        case DE_OP: operacion = ">"; break;
        case DT_OP: operacion = ">="; break;
        case EQ_OP: operacion = "="; break;
        default: operacion = "$"; break;
    }
    return operacion;
}
