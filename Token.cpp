#include "Token.h"


Token::Token(Token::Type type1) : type(type1){}

Token::Token(Token::Type type1, char op) : type(type1){
    TypeText = string (1, op);
}

Token::Token(Token::Type type1, const std::string &word, int first, int last) : type(type1){
    TypeText = word.substr(first, last);
}


ostream& operator<<(ostream& os, const Token& tokens){

    switch (tokens.type) {

        case Token::PLUS: os << "TOKEN(PLUS)"; break;
        case Token::MINUS: os << "TOKEN(MINUS)"; break;
        case Token::DIV: os << "TOKEN(DIV)"; break;
        case Token::MUL: os << "TOKEN(MUL)"; break;
        case Token::END: os << "TOKEN(END)"; break;
        case Token::ERR: os << "TOKEN(ERR)"; break;
        case Token::NUM: os << "TOKEN(NUM)"; break;
        default:
            os<<"Token(UNKNOW)"; break;
    }

    return os;

}


ostream& operator<<(ostream& os, const Token* tokens){
    return os << *tokens;
}