#ifndef COMPILADORESPROYECTO_TOKEN_H
#define COMPILADORESPROYECTO_TOKEN_H

#include <iostream>
#include <string>

using namespace std;

class Token {

public:

    enum Type { MUL, MINUS, PLUS, DIV, END, NUM, ERR,
            PD, PI, ID, ASSIGN, TypeDCL, EQ, WriteLn, Write, PC,
            LE, LT, DE, DT};

    //LE <
    //LT <=
    //DE >
    //DT >=

    // ASSIGN :=
    // TypeDCL :
    // EQ = (para condiciones)
    // WriteLn imprime con salto de linea
    // Write imprime sin salto de linea

    Type type;
    string TypeText;

    explicit Token(Type type1);
    Token(Type type1, char op);
    Token(Type type1, const string& word, int first, int last);


    friend ostream& operator << (ostream& os, const Token& type1);
    friend ostream& operator << (ostream& os, const Token* type1);

};


#endif //COMPILADORESPROYECTO_TOKEN_H
