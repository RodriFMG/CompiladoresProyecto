#ifndef COMPILADORESPROYECTO_IMP_INTERPRETER_H
#define COMPILADORESPROYECTO_IMP_INTERPRETER_H

#include <unordered_map>
#include "exp.h"
#include "imp_value_visitor.h"
#include "environment.h"

using namespace std;

class ImpInterpreter : public ImpValueVisitor {
private:
    Environment<ImpValue> env;
    Environment<FunDec*> fdecs;
    ImpValue retval;
    bool retcall;

public:
    void interpret(Program*);
    void visit(Program*);
    void visit(Body*);
    void visit(VarDecList*);
    void visit(VarDec*);
    void visit(FunDecList*);
    void visit(FunDec*);
    void visit(StmList*);
    void visit(AssignStatement*);
    void visit(PrinteoStatement*);
    void visit(IfStatement*);
    void visit(WhileStatement*);
    void visit(DoWhileStatement*);
    void visit(ForStatement* e);
    void visit(FCallStatement* e);
    ImpValue visit(BinaryExp* e);
    ImpValue visit(BoolExp* e);
    ImpValue visit(NumberExp* e);
    ImpValue visit(IdentifierExp* e);
    ImpValue visit(FCallExp* e);
};

#endif //COMPILADORESPROYECTO_IMP_INTERPRETER_H