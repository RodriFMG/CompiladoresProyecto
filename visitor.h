//
// Created by RODRIGO on 11/11/2024.
//

#ifndef COMPILADORESPROYECTO_VISITOR_H
#define COMPILADORESPROYECTO_VISITOR_H

#include <list>

// Forward declarations para las clases de expresiones y sentencias
class IdentifierExp;
class BinaryExp;
class NumberExp;
class BoolExp;
class AssignStatement;
class PrinteoStatement;
class IfStatement;
class ForStatement;
class WhileStatement;
class StmList;
class VarDec;
class VarDecList;
class FunDec;
class FunDecList;
class Program;
class Body;
class FCallExp;
class FCallStatement;
class DoWhileStatement;

// Clase abstracta Visitor
class Visitor {
public:
    virtual int visit(IdentifierExp* e) = 0;
    virtual int visit(BinaryExp* e) = 0;
    virtual int visit(NumberExp* e) = 0;
    virtual int visit(FCallExp* e) = 0;
    virtual int visit(BoolExp* e) = 0;

    virtual void visit(AssignStatement* s) = 0;
    virtual void visit(PrinteoStatement* s) = 0;
    virtual void visit(IfStatement* s) = 0;
    virtual void visit(ForStatement* s) = 0;
    virtual void visit(WhileStatement* s) = 0;
    virtual void visit(FCallStatement* s) = 0;
    virtual void visit(DoWhileStatement* s) = 0;

    virtual void visit(StmList* sl) = 0;
    virtual void visit(VarDec* v) = 0;
    virtual void visit(VarDecList* vl) = 0;
    virtual void visit(FunDec* f) = 0;
    virtual void visit(FunDecList* fl) = 0;
    virtual void visit(Body* b) = 0;
    virtual void visit(Program* p) = 0;
};

// Clase PrintVisitor para imprimir el programa
class PrintVisitor : public Visitor {
public:
    void imprimir(Program* p);

    int visit(IdentifierExp* e) override;
    int visit(BinaryExp* e) override;
    int visit(NumberExp* e) override;
    int visit (FCallExp* e) override;
    int visit(BoolExp* e) override;

    void visit(AssignStatement* s) override;
    void visit(PrinteoStatement* s) override;
    void visit(IfStatement* s) override;
    void visit(ForStatement* s) override;
    void visit(WhileStatement* s) override;
    void visit(DoWhileStatement* s) override;
    void visit(FCallStatement* s) override;

    void visit(StmList* sl) override;
    void visit(VarDec* v) override;
    void visit(VarDecList* vl) override;
    void visit(FunDec* f) override;
    void visit(FunDecList* fl) override;
    void visit(Body* b) override;
    void visit(Program* p) override;

private:
    int indentLevel = 0;
    void increaseIndent() { indentLevel++; }
    void decreaseIndent() { indentLevel--; }
    void printIndent();
};

#endif // COMPILADORESPROYECTO_VISITOR_H

