#include "Parser.h"

Parser::Parser(Scanner *scanner1) : scanner(scanner1) {

    previous = NULL;
    current = scanner->NextToken();

    if(current->type == Token::ERR){
        cout<<"Error en el primer token: "<<current->TypeText<<" (Constructor Parser)\n";
        exit(0);
    }

}

// Si borro la memoria del escaner, current o previous sale error de memoria.
Parser::~Parser() = default;

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

// Parse de VarDec y VarDecList

VarDec* Parser::ParseVarDec() {
    VarDec* vd = NULL;
    // En Pascal, las variables se declaran con `var` seguido de nombres y tipos de variables
    std::list<std::string> vars;
    if (match(Token::ID)){
        vars.push_back(previous->TypeText);

        while (match(Token::COMA)) {
            if (!match(Token::ID)) {
                std::cout << "Error: se esperaba un identificador después de ',' en la declaración de variables.\n";
                exit(1);
            }
            vars.push_back(previous->TypeText);
        }

        // Después de las variables, debería venir el token TypeDCL (que representa `:` en Pascal)
        if (!match(Token::TypeDCL)) {
            std::cout << "Error: se esperaba ':' después de los identificadores en la declaración de variables.\n";
            exit(1);
        }

        // Luego del `:` esperamos el tipo de dato (INTEGER o LONGINT)
        if (!(match(Token::INTEGER) || match(Token::LONGINT) || match(Token::BOOLEAN))) {
            std::cout << "Error: se esperaba un tipo de datos válido (INTEGER o LONGINT o BOOLEAN) después de ':'.\n";
            exit(1);
        }
        std::string type = previous->TypeText;

        if (!match(Token::PC)) {
            std::cout << "Error: se esperaba ';' al final de la declaración de variables.\n";
            exit(1);
        }
        vd = new VarDec(type, vars);
    }

    return vd;
}

VarDecList* Parser::ParseVarDecList() {
    VarDecList* vdl = new VarDecList();
    // Verifica si hay un `var` opcionalmente
    if (match(Token::VAR)) {
        VarDec* varDec = ParseVarDec();

        while (varDec != nullptr) {
            vdl->add(varDec);
            varDec = ParseVarDec();
        }
    }
    else if(match(Token::ID)){
        std::cout << "Error: se esperaba 'var' al principio de la declaración de variables.\n";
        exit(1);
    }
    else{
        vdl->vardecs.clear();
    }
    return vdl;
}

// Parse de Body

Body* Parser::ParseBody() {
    VarDecList* vdl = ParseVarDecList();
    StmList* sl = ParseStatementList();

    return new Body(vdl, sl);
}

// Parse de FunDec y FunDecList

FunDec* Parser::ParseFunDec() {
    FunDec* fd = NULL;
    if (match(Token::FUNCTION)){
        if (!match(Token::ID)) {
            std::cout << "Error: se esperaba un nombre de función después de 'function'.\n";
            exit(1);
        }
        std::string fname = previous->TypeText;

        if (!match(Token::PI)) {
            std::cout << "Error: se esperaba '(' después del nombre de la función.\n";
            exit(1);
        }

        std::list<std::string> paramNames;
        std::list<std::string> paramTypes;

        if (!check(Token::PD)) {  // Si no hay ')', procesamos los parámetros

            do {
                if (!match(Token::ID)) {
                    std::cout << "Error: se esperaba un nombre de parámetro.\n";
                    exit(1);
                }
                paramNames.push_back(previous->TypeText);
            } while (match(Token::COMA));

            if (!match(Token::TypeDCL)) {
                std::cout << "Error: se esperaba ':' después de los nombres de los parámetros.\n";
                exit(1);
            }

            if (!(match(Token::INTEGER) || match(Token::LONGINT) || match(Token::BOOLEAN))) {
                std::cout << "Error: se esperaba un tipo de datos válido (INTEGER o LONGINT o BOOLEAN) después de ':'.\n";
                exit(1);
            }
            std::string paramType = previous->TypeText;

            for (size_t i = 0; i < paramNames.size(); i++) {
                paramTypes.push_back(paramType);
            }
        }

        if (!match(Token::PD)) {
            std::cout << "Error: se esperaba ')' después de la lista de parámetros.\n";
            exit(1);
        }

        if (!match(Token::TypeDCL)) {
            std::cout << "Error: se esperaba ':' después de los parámetros.\n";
            exit(1);
        }

        if (!(match(Token::INTEGER) || match(Token::LONGINT) || match(Token::BOOLEAN))) {
            std::cout << "Error: se esperaba un tipo de datos de retorno válido (INTEGER o LONGINT o BOOLEAN).\n";
            exit(1);
        }
        std::string rtype = previous->TypeText;



        if(!match(Token::PC)){
            std::cout << "Error: se esperaba ';' después del tipo de datos de retorno.\n";
            exit(1);
        }

        Body* body = ParseBody();

        fd = new FunDec(fname, paramTypes, paramNames, rtype, body);

        if(!match(Token::PC)){
            std::cout << "Error: se esperaba ';' después del tipo de datos de retorno.\n";
            exit(1);
        }
    }
    else if(match(Token::PROCEDURE)){
        if (!match(Token::ID)) {
            std::cout << "Error: se esperaba un nombre de función después de 'function'.\n";
            exit(1);
        }
        std::string fname = previous->TypeText;

        if (!match(Token::PI)) {
            std::cout << "Error: se esperaba '(' después del nombre de la función.\n";
            exit(1);
        }

        std::list<std::string> paramNames;
        std::list<std::string> paramTypes;

        if (!check(Token::PD)) {  // Si no hay ')', procesamos los parámetros

            do {
                if (!match(Token::ID)) {
                    std::cout << "Error: se esperaba un nombre de parámetro.\n";
                    exit(1);
                }
                paramNames.push_back(previous->TypeText);
            } while (match(Token::COMA));

            if (!match(Token::TypeDCL)) {
                std::cout << "Error: se esperaba ':' después de los nombres de los parámetros.\n";
                exit(1);
            }

            if (!(match(Token::INTEGER) || match(Token::LONGINT) || match(Token::BOOLEAN))) {
                std::cout << "Error: se esperaba un tipo de datos válido (INTEGER o LONGINT o BOOLEAN) después de ':'.\n";
                exit(1);
            }
            std::string paramType = previous->TypeText;

            for (size_t i = 0; i < paramNames.size(); i++) {
                paramTypes.push_back(paramType);
            }
        }

        if (!match(Token::PD)) {
            std::cout << "Error: se esperaba ')' después de la lista de parámetros.\n";
            exit(1);
        }

        if (match(Token::TypeDCL)) {
            std::cout << "Error: Una función void no tiene tipo de return.\n";
            exit(1);
        }

        std::string rtype = "procedure";

        if(!match(Token::PC)){
            std::cout << "Error: se esperaba ';' después del tipo de datos de retorno.\n";
            exit(1);
        }

        Body* body = ParseBody();

        fd = new FunDec(fname, paramTypes, paramNames, rtype, body);

        if(!match(Token::PC)){
            std::cout << "Error: se esperaba ';' después del tipo de datos de retorno.\n";
            exit(1);
        }
    }
    return fd;
}


FunDecList* Parser::ParseFunDecList() {
    FunDecList* fdl = new FunDecList();
    FunDec* funDec = ParseFunDec();

    if(funDec == NULL) fdl = nullptr;

    while (funDec != nullptr) {
        fdl->add(funDec);
        funDec = ParseFunDec();
    }
    return fdl;
}

// Parse del Programa

Program* Parser::ParseProgram() {

    FunDecList* funDecs = NULL;
    VarDecList* varDecs = NULL;
    bool FirstFunction;

    if(!match(Token::PROGRAM)){
        cout << "Falto colocar el Token PROGRAM al inicio del código. (ParseProgram)";
        exit(0);
    }
    if(!match(Token::ID)){
        cout << "Falto colocar el nombre del programa al inicio del código. (ParseProgram)";
        exit(0);
    }

    string NameProgram = previous->TypeText;

    if(!match(Token::PC)){
        cout << "Falto colocar el ; del programa al inicio del código. (ParseProgram)";
        exit(0);
    }

    if(check(Token::FUNCTION) || check(Token::PROCEDURE)){
        FirstFunction = true;
        funDecs = ParseFunDecList();
        varDecs = ParseVarDecList();
    }
    else if(check(Token::VAR)){
        FirstFunction = false;
        varDecs = ParseVarDecList();
        funDecs = ParseFunDecList();
    }

    StmList* stmList = ParseStatementList();

    if(!match(Token::PUNTO)){
        std::cout << "Error: se esperaba '.' al final del programa" << current->TypeText<<endl;
        exit(1);
    }
    return new Program(varDecs, funDecs, stmList, NameProgram, FirstFunction);
}


StmList* Parser::ParseStatementList() {
    if (!match(Token::BEGIN)) {
        std::cout << "Error: se esperaba 'begin', pero se encontro:" << current->TypeText<<endl;
        exit(1);
    }

    StmList* stmList = new StmList();

    stmList->add(ParseStatement());

    if (!match(Token::PC)) {
        std::cout << "Error: se esperaba ';' después de la primera sentencia en el bloque 'begin ... end'.\n";
        exit(1);
    }

    while (check(Token::ID) || check(Token::IF) || check(Token::WHILE) || check(Token::FOR) || check(Token::Write) || check(Token::WriteLn)
    || check(Token::REPEAT)) {
        stmList->add(ParseStatement());

        if (!match(Token::PC)) {
            std::cout << "Error: se esperaba ';' después de la sentencia en el bloque 'begin ... end'.\n";
            exit(1);
        }
    }

    if (!match(Token::ENDP)) {
        std::cout << "Error: se esperaba 'end', pero se encontró: "<< current;
        exit(1);
    }

    return stmList;
}


Stm *Parser::ParseStatement() {

    Exp* e = NULL;
    Stm* s = NULL;

    if(current == NULL){
        cout << "Error:  Token actual es NULL (ParseStatement)" << endl;
        exit(1);
    }

    if(match(Token::ID)){
        string lex = previous->TypeText;
        if(match(Token::ASSIGN)){

            e = ParseCExpression();
            s = new AssignStatement(lex, e);
        }
        else if (match(Token::PI)){
            list<Exp*> args;
            if (!check(Token::PD)){
                args.push_back(ParseCExpression());
                while (match(Token::COMA)){
                    args.push_back(ParseCExpression());
                }
            }
            if (!match(Token::PD)){
                cout << "Falta paréntesis derecho, pero se encontro:" << current->TypeText<<endl;
                exit(1);
            }
            s = new FCallStatement(lex, args);
        }
        else {
            cout << "Error: Se esperaba una asignación de tipo := (ParseStatement), pero se encontro:" << current->TypeText<<endl;
            exit(1);
        }


    }
    else if(match(Token::Write) || match(Token::WriteLn)){

        string TypePrint;
        if(previous->type == Token::WriteLn) TypePrint = "writeln";
        else if(previous->type == Token::Write) TypePrint = "write";

        if(match(Token::PI)){
            e = ParseCExpression();

            if(!match(Token::PD)){
                cout<< "Se esperaba un parentesis derecho en el printeo. (ParseStatement)";
                exit(0);
            }
        }


        s = new PrinteoStatement(TypePrint, e);

    }
    else if(match(Token::IF)){

        list<Exp*> expList;
        list<StmList*> stmList;

        expList.push_back(ParseCExpression());

        if(!match(Token::THEN)){
            cout << "Se esperaba el Token THEN, pero se encontró: "<<current->TypeText << " (ParseStatement - IF)";
            exit(0);
        }

        stmList.push_back(ParseStatementList()); // maybe hay que poner el ;


        while (match(Token::ELSE)){

            if(match(Token::IF)){

                expList.push_back(ParseCExpression());

                if(!match(Token::THEN)){
                    cout << "Se esperaba el Token THEN, pero se encontró: "<<current->TypeText << " (ParseStatement - IF - while)";
                    exit(0);
                }


                stmList.push_back(ParseStatementList());

            }
            else{

                stmList.push_back(ParseStatementList());

                break;
            }
        }

        s = new IfStatement(expList, stmList);

    }
    else if(match(Token::FOR)){

        if(!match(Token::ID)){
            cout<<"Se esperaba un ID, pero se encontró: "<< current->TypeText << ". (ParseStatement - FOR)";
            exit(0);
        }

        string id = previous->TypeText;

        if(!match(Token::ASSIGN)){
            cout<<"Se esperaba una ASSIGN, pero se encontró: "<< current->TypeText << ". (ParseStatement - FOR)";
            exit(0);
        }

        Exp* exp1 = ParseCExpression();

        if(!match(Token::TO) && !match(Token::DOWNTO)){
            cout<<"Se esperaba el Token TO o Token DOWNTO, pero se encontró: "<< current->TypeText << ". (ParseStatement - FOR)";
            exit(0);
        }

        string i_o_d = previous->TypeText;

        Exp* exp2 = ParseCExpression();

        if(!match(Token::DO)){
            cout<<"Se esperaba el Token DO, pero se encontró: "<< current->TypeText << ". (ParseStatement - FOR)";
            exit(0);
        }


        StmList* stms = ParseStatementList();


        s = new ForStatement(id, i_o_d, exp1, exp2, stms);

    }
    else if(match(Token::WHILE)){

        e = ParseCExpression();

        if(!match(Token::DO)){
            cout<<"Se esperaba el Token DO, pero se encontró: "<< current->TypeText << ". (ParseStatement - WHILE)";
            exit(0);
        }

        StmList* stms = ParseStatementList();


        s = new WhileStatement(e, stms);

    }
    else if(match(Token::REPEAT)){

        StmList* stms = ParseStatementList();

        if(!match(Token::PC)){
            cout << " Se esperaba un ; despues de todos los statements."<< endl;
            exit(0);
        }

        if(!match(Token::UNTIL)){
            cout << "Error: se esperaba 'until' despues de los statements." << endl;
            cout << "Se encontró " << current << endl;
            exit(0);
        }

        Exp* exp = ParseCExpression();

        s = new DoWhileStatement(exp, stms);

    }
    else{
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }


    return s;

}

Exp *Parser::ParseCExpression() {

    Exp* left = ParseExpression();

    if(match(Token::EQ) || match(Token::LE) || match(Token::LT) || match(Token::DE) || match(Token::DT) || match(Token::DIFERENTES)){
        BinaryOp op = EQ_OP;

        if(previous->type == Token::EQ) op = EQ_OP;
        else if(previous->type == Token::LE) op = LE_OP;
        else if(previous->type == Token::LT) op = LT_OP;
        else if(previous->type == Token::DE) op = DE_OP;
        else if(previous->type == Token::DT) op = DT_OP;
        else if(previous->type == Token::DIFERENTES) op = DIF_OP;

        Exp* right = ParseExpression();
        left = new BinaryExp(left, op, right);

    }

    return left;
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
    else if (match(Token::ID)) {
        string texto = previous->TypeText;
        //Parse FCallExp
        if (match(Token::PI)){
            list<Exp*> args;
            if (!check(Token::PD)){
                args.push_back(ParseCExpression());
                while (match(Token::COMA)){
                    args.push_back(ParseCExpression());
                }
            }
            if (!match(Token::PD)){
                cout << "Falta paréntesis derecho" << endl;
                exit(0);
            }
            return new FCallExp(texto, args);
        }
        else{
            return new IdentifierExp(previous->TypeText);
        }

    }
    else if(match(Token::TRUE)){
        return new BoolExp(1);
    }
    else if(match(Token::FALSE)){
        return new BoolExp(0);
    }
    else if(match(Token::PI)){
        Exp* exp = ParseExpression();

        if(!match(Token::PD)){
            cout << " Error, caracter "<< previous->TypeText <<" incorrecto, debio seguir un )  (ParseFactor)";
            exit(0);
        }

        return exp;

    }

    cout << "Error: se esperaba un número o identificador. (ParseFactor)" << current->TypeText <<endl;
    exit(0);

}
