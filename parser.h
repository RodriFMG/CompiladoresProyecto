#ifndef COMPILADORESPROYECTO_PARSER_H
#define COMPILADORESPROYECTO_PARSER_H

#include "Scanner.h"
#include "Exp.h"
class Parser {
private:

    Scanner* scanner;
    Token *current, *previous;

    bool match(Token::Type token);
    bool check(Token::Type token);
    bool advance();
    bool isAtEnd();
    Exp* ParseCExpression();
    Exp* ParseExpression();
    Exp* ParseTerm();
    Exp* ParseFactor();

public:

    explicit Parser(Scanner* scanner1);

    Program* ParseProgram();
    StmList* ParseStatementList();
    Stm* ParseStatement();
    VarDec* ParseVarDec();
    VarDecList* ParseVarDecList();
    Body* ParseBody();
    FunDecList* ParseFunDecList();
    FunDec* ParseFunDec();


    ~Parser();

};


#endif //COMPILADORESPROYECTO_PARSER_H
