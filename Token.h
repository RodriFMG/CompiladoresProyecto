#ifndef COMPILADORESPROYECTO_TOKEN_H
#define COMPILADORESPROYECTO_TOKEN_H

#include <iostream>
#include <string>

using namespace std;

class Token {

public:

    enum Type { MUL, MINUS, PLUS, DIV, END, NUM, ERR,
            PD, PI};

    Type type;
    string TypeText;

    explicit Token(Type type1);
    Token(Type type1, char op);
    Token(Type type1, const string& word, int first, int last);


    friend ostream& operator << (ostream& os, const Token& type1);
    friend ostream& operator << (ostream& os, const Token* type1);

};


#endif //COMPILADORESPROYECTO_TOKEN_H
