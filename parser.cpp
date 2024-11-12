#include "Parser.h"

Parser::Parser(Scanner *scanner1) : scanner(scanner1) {

    previous = NULL;
    current = scanner->NextToken();

    if(current->type == Token::ERR){
        cout<<"Error en el primer token: "<<current->TypeText<<" (Constructor Parser)\n";
        exit(0);
    }

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

Program* Parser::ParseProgram(){

    StmList* stmList = ParseStatementList();

    return new Program(stmList);

}

StmList* Parser::ParseStatementList(){

    StmList* stmList = new StmList();

    stmList->add(ParseStatement());

    // Modificar esto si quiero que todas las sentencias terminen en ;
    while(match(Token::PC)) stmList->add(ParseStatement());

    return stmList;
}

Stm *Parser::ParseStatement() {

    Exp* e = NULL;
    Stm* s = NULL;

    if(current == NULL){
        cout << "Error:  Token actual es NULL (ParseStatement)" << endl;
        exit(0);
    }

    if(match(Token::ID)){

        string lex = previous->TypeText;

        if(!match(Token::ASSIGN)){
            cout << "Error: Se esperaba una asignación de tipo := (ParseStatement)"<<endl;
            exit(0);
        }

        e = ParseExpression();

        s = new AssignStatement(lex, e);
    }
    else if(match(Token::Write) || match(Token::WriteLn)){

        string TypePrint;
        if(previous->type == Token::WriteLn) TypePrint = "WriteLn";
        else if(previous->type == Token::Write) TypePrint = "Write";

        if(!match(Token::PI)){
            cout<< "Se esperaba un parentesis izquierdo en el printeo. (ParseStatement)";
            exit(0);
        }

        e = ParseExpression();

        if(!match(Token::PD)){
            cout<< "Se esperaba un parentesis derecho en el printeo. (ParseStatement)";
            exit(0);
        }

        s = new PrinteoStatement(TypePrint, e);

    }


    return s;

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
    else if(match(Token::ID)){
        return new IdentifierExp(previous->TypeText);
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
