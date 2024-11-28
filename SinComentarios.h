//
// Created by RODRIGO on 28/11/2024.
//

#ifndef COMPILADORESPROYECTO_SINCOMENTARIOS_H
#define COMPILADORESPROYECTO_SINCOMENTARIOS_H

#include "Token.h"

class SinComentarios {

private:
    int previous, current;
    string input;
public:

    SinComentarios(const char* it_s);
    string DeleteComments();

};


#endif //COMPILADORESPROYECTO_SINCOMENTARIOS_H
