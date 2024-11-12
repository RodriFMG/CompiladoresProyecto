//
// Created by RODRIGO on 11/11/2024.
//

#ifndef COMPILADORESPROYECTO_VISITOR_H
#define COMPILADORESPROYECTO_VISITOR_H


class IdentifierExp;
class BinaryExp;
class NumberExp;
class AssignStatement;
class PrinteoStatement;
class IfStatement;
class StmList;
class Program;

class Visitor {
public:

    virtual int visit(IdentifierExp* e) = 0;
    virtual int visit(BinaryExp* e) = 0;
    virtual int visit(NumberExp* e) = 0;

    virtual void visit(AssignStatement* s) = 0;
    virtual void visit(PrinteoStatement* s) = 0;
    virtual void visit(IfStatement* s) = 0;

    virtual void visit(StmList* sl) = 0;
    virtual void visit(Program* p) = 0;

};

class EvalVisitor : public Visitor{
public:

    void ejecutar(Program* p);
    int visit(IdentifierExp* e) override;
    int visit(BinaryExp* e) override;
    int visit(NumberExp* e) override;

    void visit(AssignStatement* s) override;
    void visit(PrinteoStatement* s) override;
    void visit(IfStatement* s) override;

    void visit(StmList* sl) override;
    void visit(Program* p) override;

};

class PrintVisitor : public Visitor{
public:

    void imprimir(Program* p);
    int visit(IdentifierExp* e) override;
    int visit(BinaryExp* e) override;
    int visit(NumberExp* e) override;

    void visit(AssignStatement* s) override;
    void visit(PrinteoStatement* s) override;
    void visit(IfStatement* s) override;

    void visit(StmList* sl) override;
    void visit(Program* p) override;

};


#endif //COMPILADORESPROYECTO_VISITOR_H
