//
// Created by RODRIGO on 11/11/2024.
//

#ifndef COMPILADORESPROYECTO_EXP_H
#define COMPILADORESPROYECTO_EXP_H

#include <iostream>
#include <string>
#include <list>
#include "visitor.h"
#include "imp_value_visitor.h"
#include "imp_value.h"
#include "type_visitor.h"
using namespace std;

enum BinaryOp {
    PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, EQ_OP, LE_OP, LT_OP, DE_OP, DT_OP, DIF_OP
};

class Exp {
public:
    virtual ~Exp() noexcept = 0;
    virtual int accept(Visitor* visitor) = 0;
    virtual ImpValue accept(ImpValueVisitor* Impvisitor)=0;
    virtual ImpType accept(TypeVisitor* typeVisitor) = 0;
    static string BinaryToChar(BinaryOp op);
};

class NumberExp : public Exp {
public:
    int value;
    explicit NumberExp(int v);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* Impvisitor)override;
    ImpType accept(TypeVisitor* typeVisitor);
    ~NumberExp() noexcept override;
};

class BoolExp : public Exp {
public:
    int value;
    explicit BoolExp(int v);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* Impvisitor)override;
    ImpType accept(TypeVisitor* typeVisitor);
    ~BoolExp() noexcept override;
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, BinaryOp o, Exp* r);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* Impvisitor)override;
    ImpType accept(TypeVisitor* typeVisitor);
    ~BinaryExp() noexcept override;
};

class IdentifierExp : public Exp {
public:
    string id;
    explicit IdentifierExp(string id_);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* Impvisitor)override;
    ImpType accept(TypeVisitor* typeVisitor);
    ~IdentifierExp() noexcept override;
};

class FCallExp : public Exp {
public:
    string fname;
    list<Exp*> args;
    FCallExp(string fname, list<Exp*> args);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* Impvisitor)override;
    ImpType accept(TypeVisitor* typeVisitor);
    ~FCallExp() noexcept override;
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() noexcept = 0;
    virtual void accept(ImpValueVisitor* Impvisitor)=0;
    virtual void accept(TypeVisitor* typeVisitor) = 0;
};

class AssignStatement : public Stm {
public:
    string id;
    Exp* exp;
    AssignStatement(string id_, Exp* exp_);
    int accept(Visitor* visitor) override;
    void accept(ImpValueVisitor* Impvisitor) override;
    void accept(TypeVisitor* typeVisitor);
    ~AssignStatement() noexcept override;
};

class PrinteoStatement : public Stm {
public:
    string TypePrint;
    Exp* exp;
    PrinteoStatement(string TypePrint_, Exp* exp_);
    int accept(Visitor* visitor) override;
    void accept(ImpValueVisitor* Impvisitor) override;
    void accept(TypeVisitor* typeVisitor);
    ~PrinteoStatement() noexcept override;
};

class ForStatement : public Stm {
public:
    string id;
    string increase_or_decrease;
    Exp* exp1;
    Exp* exp2;
    StmList* stms;
    ForStatement(string id_, string i_o_d, Exp* exp1_, Exp* exp2_, StmList* stms_);
    int accept(Visitor* visitor) override;
    void accept(ImpValueVisitor* Impvisitor) override;
    void accept(TypeVisitor* typeVisitor);
    ~ForStatement() noexcept override;
};

class WhileStatement : public Stm {
public:
    Exp* exp;
    StmList* stms;
    WhileStatement(Exp* exp_, StmList* stms_);
    int accept(Visitor* visitor) override;
    void accept(ImpValueVisitor* Impvisitor) override;
    void accept(TypeVisitor* typeVisitor);
    ~WhileStatement() noexcept override;
};

class DoWhileStatement : public Stm{
public:
    Exp* exp;
    StmList* stms;
    DoWhileStatement(Exp* exp_, StmList* stms_);
    int accept(Visitor* visitor) override;
    void accept(ImpValueVisitor* Impvisitor) override;
    void accept(TypeVisitor* typeVisitor);
    ~DoWhileStatement() noexcept override;

};

class IfStatement : public Stm {
public:
    list<Exp*> conditions;
    list<StmList*> conditionBodies;
    IfStatement(list<Exp*> conditions_, list<StmList*> conditionBodies_);
    int accept(Visitor* visitor) override;
    void accept(ImpValueVisitor* Impvisitor) override;
    void accept(TypeVisitor* typeVisitor);
    ~IfStatement() noexcept override;
};

class StmList {
public:
    list<Stm*> stms;
    StmList();
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~StmList();
    void add(Stm* stm);
};


class FCallStatement : public Stm {
public:
    string fname;
    list<Exp*> args;
    FCallStatement(string fname, list<Exp*> args);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~FCallStatement();
};

// Clases para declaraciones de variables y funciones

class VarDec {
public:
    string type;
    list<string> vars;
    VarDec(string type, list<string> vars);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~VarDec();
};

class VarDecList {
public:
    list<VarDec*> vardecs;
    VarDecList();
    void add(VarDec* vardec);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~VarDecList();
};

// Clase Body para almacenar variables y declaraciones en un bloque

class Body {
public:
    VarDecList* vardecs;
    StmList* statements;
    Body(VarDecList* vardecs, StmList* statements);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~Body();
};

class FunDec {
public:
    string fname;
    string rtype;
    list<string> paramTypes;
    list<string> paramNames;
    Body* body;
    FunDec(string fname, list<string> paramTypes, list<string> paramNames, string rtype, Body* body);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~FunDec();
};

class FunDecList {
public:
    list<FunDec*> flist;
    FunDecList();
    void add(FunDec* s);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~FunDecList();
};


class Program {
public:
    string NameProgram;
    VarDecList* varDecs;
    FunDecList* funDecs;
    StmList* stmList;
    bool FirstFunctions;

    Program(VarDecList* varDecs, FunDecList* funDecs, StmList* stmList, string NameProgram, bool FirstFunctions_);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* Impvisitor);
    void accept(TypeVisitor* typeVisitor);
    ~Program();
};

#endif //COMPILADORESPROYECTO_EXP_H



