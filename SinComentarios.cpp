#include "SinComentarios.h"


SinComentarios::SinComentarios(const char* it_s) : input(it_s), previous(0), current(0) {}

string SinComentarios::DeleteComments() {

    while (current < input.length() && input[current] != '/') current += 1;
    if (current >= input.length()) return input;


    previous = current;
    current+=1;

    if(input[current] == '/'){

        while(input[current] != '\n') ++current;
        input.erase(previous, current-previous+1);

        current = previous;

        return DeleteComments();
    }
    else{
        return DeleteComments();
    }


}
