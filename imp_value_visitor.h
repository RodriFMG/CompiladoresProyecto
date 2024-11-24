#ifndef COMPILADORESPROYECTO_IMP_VALUE_VISITOR_H
#define COMPILADORESPROYECTO_IMP_VALUE_VISITOR_H

#include "Exp.h"
#include "imp_value.h"

class ImpValueVisitor {
public:
    virtual void visit(Program* p) = 0;
    virtual void visit(Body* b) = 0;
    virtual void visit(VarDecList* e) = 0;
    virtual void visit(VarDec* e) = 0;
    virtual void visit(FunDecList* e) = 0;
    virtual void visit(FunDec* e) = 0;
    virtual void visit(StmList* e) = 0;
    virtual void visit(AssignStatement* e) = 0;
    virtual void visit(PrinteoStatement* e) = 0;
    virtual void visit(IfStatement* e) = 0;
    virtual void visit(WhileStatement* e) = 0;
    virtual void visit(DoWhileStatement* s) = 0;
    virtual void visit(ForStatement* e) = 0;
    virtual void visit(FCallStatement* e) = 0;
    virtual ImpValue visit(BinaryExp* e) = 0;
    virtual ImpValue visit(BoolExp* e)=0;
    virtual ImpValue visit(NumberExp* e) = 0;
    virtual ImpValue visit(IdentifierExp* e) = 0;
    virtual ImpValue visit(FCallExp* e) = 0;
};


#endif //COMPILADORESPROYECTO_IMP_VALUE_VISITOR_H
