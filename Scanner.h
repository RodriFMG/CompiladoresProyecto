//
// Created by RODRIGO on 10/11/2024.
//

#ifndef COMPILADORESPROYECTO_SCANNER_H
#define COMPILADORESPROYECTO_SCANNER_H

#include "Token.h"

class Scanner{
private:

    int first, current;
    string input;

public:

    explicit Scanner(const char* it_s);
    Token* NextToken();
    void reset();
    ~Scanner();

};


#endif //COMPILADORESPROYECTO_SCANNER_H
