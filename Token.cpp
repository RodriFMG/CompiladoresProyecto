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
        case Token::PI: os << "TOKEN(PI)"; break;
        case Token::PD: os << "TOKEN(PD)"; break;
        case Token::ID: os << "TOKEN(ID)"; break;
        case Token::TypeDCL: os << "TOKEN(DOSPUNTOS)"; break;
        case Token::ASSIGN: os << "TOKEN(ASSIGN)"; break;
        case Token::EQ: os << "TOKEN(EQ)"; break;
        case Token::Write: os <<"TOKEN(WRITE)"; break;
        case Token::WriteLn: os << "TOKEN(WRITELN)"; break;
        case Token::PC: os << "TOKEN(PC)"; break;
        case Token::LE: os << "TOKEN(LE)"; break;
        case Token::LT: os << "TOKEN(LT)"; break;
        case Token::DE: os << "TOKEN(DE)"; break;
        case Token::DT: os << "TOKEN(DT)"; break;
        case Token::IF: os << "TOKEN(ID)"; break;
        case Token::THEN: os << "TOKEN(THEN)"; break;
        case Token::ELSE: os << "TOKEN(ELSE)"; break;
        case Token::BEGINIF: os << "TOKEN(BEGINIF)"; break;
        case Token::ENDIF: os << "TOKEN(ENDIF)"; break;
        case Token::FOR: os << "TOKEN(FOR)"; break;
        case Token::TO: os << "TOKEN(TO)"; break;
        case Token::DOWNTO: os << "TOKEN(DOWNTO)"; break;
        case Token::DO: os << "TOKEN(DO)"; break;
        case Token::WHILE: os << "TOKEN(WHILE)"; break;
        default:
            os<<"Token(UNKNOW)"; break;
    }

    return os;

}


ostream& operator<<(ostream& os, const Token* tokens){
    return os << *tokens;
}