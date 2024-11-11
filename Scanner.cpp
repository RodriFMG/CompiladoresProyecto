#include "Scanner.h"


Scanner::Scanner(const char *it_s) : input(it_s), first(0), current(0){}

bool is_white_space(char c){
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

Token *Scanner::NextToken() {

    Token* token;
    while (current < input.length() && is_white_space(input[current])) current+=1;
    if(current >= input.length()) return new Token(Token::END);

    char c = input[current];
    first = current;

    if(isdigit(c)){

        while (current < input.length() && isdigit(input[current])) ++current;

        token = new Token(Token::NUM, input, first, current-first);

    }
    else if(isalpha(c)){

    }
    else if(strchr("+-/*()", c)){
        switch (c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            default :
                token = new Token(Token::ERR, c);
                cout<< "No debería poder llegar acá";
                break;
        }
        ++current;
    }
    else{
        token = new Token(Token::ERR, c);
        ++current;
    }

    return token;

}
