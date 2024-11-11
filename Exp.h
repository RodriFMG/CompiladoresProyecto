//
// Created by RODRIGO on 11/11/2024.
//

#ifndef COMPILADORESPROYECTO_EXP_H
#define COMPILADORESPROYECTO_EXP_H

#include <iostream>
#include <string>
using namespace std;

enum BinaryOp {
    PLUS_OP, MINUS_OP, MUL_OP, DIV_OP
};

class Exp {
public:

    virtual ~Exp() = 0;
    virtual int accept() = 0;
    static string BinaryToChar(BinaryOp op);

};

class NumberExp : public Exp{
public:

    int value;

    explicit NumberExp(int v);
    int accept() override;
    ~NumberExp() override;

};

class BinaryExp : public Exp{
public:

    Exp *left, *right;
    BinaryOp op;

    BinaryExp(Exp* l, BinaryOp o, Exp* r);
    int accept() override;
    ~BinaryExp() override;


};

class Program {

    Exp* result;

    Program(Exp* r);
    int accept();
    ~Program();

};


#endif //COMPILADORESPROYECTO_EXP_H
