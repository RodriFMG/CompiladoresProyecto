//
// Created by RODRIGO on 11/11/2024.
//

#ifndef COMPILADORESPROYECTO_EXP_H
#define COMPILADORESPROYECTO_EXP_H

#include <iostream>
#include <string>
#include <list>
#include "visitor.h"
using namespace std;

enum BinaryOp {
    PLUS_OP, MINUS_OP, MUL_OP, DIV_OP
};

class Exp {
public:

    virtual ~Exp() noexcept = 0;
    virtual int accept(Visitor* visitor) = 0;
    static string BinaryToChar(BinaryOp op);

};

class NumberExp : public Exp{
public:

    int value;

    explicit NumberExp(int v);
    int accept(Visitor* visitor) override;
    ~NumberExp() noexcept override;

};

class BinaryExp : public Exp{
public:

    Exp *left, *right;
    BinaryOp op;

    BinaryExp(Exp* l, BinaryOp o, Exp* r);
    int accept(Visitor* visitor) override;
    ~BinaryExp() noexcept override;


};

class IdentifierExp : public Exp{
public:

    string id;

    explicit IdentifierExp(string id_);
    int accept(Visitor* visitor) override;
    ~IdentifierExp() noexcept override;

};

class Stm{
public:

    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() noexcept = 0;

};


class AssignStatement : public Stm{
public:

    string id;
    Exp* exp;

    AssignStatement(string id_, Exp* exp_);
    int accept(Visitor* visitor) override;
    ~AssignStatement() noexcept override;

};

class PrinteoStatement : public Stm{
public:

    string TypePrint;
    Exp* exp;
    PrinteoStatement(string TypePrint_, Exp* exp_);
    int accept(Visitor* visitor) override;
    ~PrinteoStatement() noexcept override;

};

class StmList{
public:
    list<Stm*> stms;
    StmList();
    int accept(Visitor* visitor);
    ~StmList();

    void add(Stm* stm);

};

class Program {
public:

    StmList* sl;

    explicit Program(StmList* sl_);
    int accept(Visitor* visitor);
    ~Program();

};


#endif //COMPILADORESPROYECTO_EXP_H
