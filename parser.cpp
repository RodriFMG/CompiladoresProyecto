#include "Parser.h"

Parser::Parser(Scanner *scanner1) : scanner(scanner1){

    previous = NULL;
    current = scanner->NextToken();

}

Parser::~Parser() {
    delete scanner;
    delete previous;
    delete current;
}

bool Parser::advance() {

    if(!isAtEnd()){

        Token* save = current;
        if(previous) delete previous;
        current = scanner->NextToken();
        previous = save;

        if(check(Token::ERR)){
            cout<<"Error en el análisis, el caracter " << current->TypeText << " no es válida.";
            exit(0);
        }

        return true;
    }

    return false;

}

bool Parser::check(Token::Type token) {

    if(isAtEnd()) return false;
    return (current->type == token);
}

bool Parser::match(Token::Type token) {

    if(check(token)){
        advance();
        return true;
    }

    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Exp *Parser::ParseExpression() {

    Exp* left = ParseTerm();

    while (match(Token::PLUS) || match(Token::MINUS)){

        BinaryOp op = PLUS_OP;
        if(previous->type == Token::PLUS) op = PLUS_OP;
        else if (previous->type == Token::MINUS) op = MINUS_OP;

        Exp* right = ParseTerm();
        left = new BinaryExp(left, op, right);

    }

    return left;

}

Exp *Parser::ParseTerm() {

    Exp* left = ParseFactor();

    while (match(Token::MUL) || match(Token::DIV)){
        BinaryOp op = MUL_OP;

        if(previous->type == Token::MUL) op = MUL_OP;
        else if(previous->type == Token::DIV) op = DIV_OP;

        Exp* right = ParseFactor();

        left = new BinaryExp(left, op, right);

    }

    return left;

}

Exp *Parser::ParseFactor() {

    if(match(Token::NUM)){
        return new NumberExp(stoi(previous->TypeText));
    }
    else if(match(Token::PI)){
        Exp* exp = ParseExpression();

        if(!match(Token::PD)){
            cout << " Error, caracter "<< previous->TypeText <<" incorrecto, debio seguir un )  (ParseFactor)";
            exit(0);
        }

        return exp;

    }

    cout << "Error: se esperaba un número o identificador. (ParseFactor)" << endl;
    exit(0);

}
