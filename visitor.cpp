#include "visitor.h"
#include "Exp.h"
#include <unordered_map>
#include <iostream>
using namespace std;

unordered_map<string, int> memoria;

////////////////////////////////////////////////////////////////////////////////
// Implementación de aceptaciones para cada expresión y sentencia
////////////////////////////////////////////////////////////////////////////////

int IdentifierExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int BinaryExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int FCallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}


int AssignStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int PrinteoStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int ForStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int StmList::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int FCallStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int VarDec::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int VarDecList::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int FunDec::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int FunDecList::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int Body::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int Program::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Implementación de PrintVisitor
////////////////////////////////////////////////////////////////////////////////

void PrintVisitor::imprimir(Program *p) {
    p->accept(this);
    cout << ".";
}

void PrintVisitor::visit(Program *p) {

    cout << "program " << p->NameProgram << ";\n";
    p->varDecs->accept(this);

    if(p->funDecs != nullptr){
        p->funDecs->accept(this);
    }
    cout <<"begin" <<endl;
    p->stmList->accept(this);
    cout <<"end";
}

void PrintVisitor::visit(StmList *stm) {
    increaseIndent();
    for(auto i: stm->stms){
        printIndent();
        i->accept(this);
        cout << endl;
    }
    decreaseIndent();
}

void PrintVisitor::visit(AssignStatement *s) {
    cout << s->id << " := ";
    s->exp->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrinteoStatement *s) {
    if (s->TypePrint == "writeln") {
        cout << "writeln";
        if (s->exp != NULL) {
            cout << "(";
            s->exp->accept(this);
            cout << ");";
        }
    } else if (s->TypePrint == "write") {
        cout << "write(";
        s->exp->accept(this);
        cout << ");";
    }
}

void PrintVisitor::visit(IfStatement *s) {
    auto itExp = s->conditions.begin();
    auto itStmList = s->conditionBodies.begin();

    cout << "if ";
    (*itExp)->accept(this);
    cout << " then\n";
    printIndent();
    cout <<"begin" <<endl;
    (*itStmList)->accept(this);
    printIndent();
    cout <<"end\n";

    ++itExp;
    ++itStmList;

    while (itExp != s->conditions.end()) {
        printIndent();
        cout << "else if ";
        (*itExp)->accept(this);
        cout << " then\n";
        printIndent();
        cout <<"begin" <<endl;
        (*itStmList)->accept(this);
        printIndent();
        cout <<"end;";
        ++itExp;
        ++itStmList;
    }

    if (itStmList != s->conditionBodies.end()) {
        printIndent();
        cout << "else\n";
        printIndent();
        cout <<"begin" <<endl;
        (*itStmList)->accept(this);
        printIndent();
        cout <<"end;";
    }
}

void PrintVisitor::visit(ForStatement *s) {
    cout << "for " << s->id << " := ";
    s->exp1->accept(this);
    cout << " " << s->increase_or_decrease << " ";
    s->exp2->accept(this);
    cout << " do\n";
    printIndent();
    cout <<"begin" <<endl;
    s->stms->accept(this);
    printIndent();
    cout <<"end;";
}

void PrintVisitor::visit(WhileStatement *s) {
    cout << "while ";
    s->exp->accept(this);
    cout << " do\n";
    printIndent();
    cout <<"begin" <<endl;
    s->stms->accept(this);
    printIndent();
    cout <<"end;";
}

void PrintVisitor::visit(FCallStatement* s) {
    cout << s->fname << "(";
    bool first = true;
    for (Exp* arg : s->args) {
        if (!first) cout << ", ";
        arg->accept(this);
        first = false;
    }
    cout << ")";
}

int PrintVisitor::visit(BinaryExp *e) {
    e->left->accept(this);
    cout << ' ' << Exp::BinaryToChar(e->op) << ' ';
    e->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp *e) {
    cout << e->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if(exp->value) cout << "true";
    else cout << "false";
    return 0;
}

int PrintVisitor::visit(FCallExp* exp) {
    cout << exp->fname << "(";
    bool first = true;
    for (Exp* arg : exp->args) {
        if (!first) cout << ", ";
        arg->accept(this);
        first = false;
    }
    cout << ")";
    return 0;
}


int PrintVisitor::visit(IdentifierExp *e) {
    cout << e->id;
    return 0;
}

void PrintVisitor::visit(VarDec *stm) {
    for (auto it = stm->vars.begin(); it != stm->vars.end(); ++it) {
        cout << *it;
        if (std::next(it) != stm->vars.end()) {
            cout << ", ";
        }
    }
    cout << " : " << stm->type << ";"<< endl;
}

void PrintVisitor::visit(VarDecList *stm) {
    if (!stm->vardecs.empty()){
        cout << "var" << endl;
    }
    increaseIndent();
    for (auto i : stm->vardecs) {
        printIndent();
        i->accept(this);
    }
    decreaseIndent();
    if (!stm->vardecs.empty()){
        cout << endl;
    }


}

void PrintVisitor::visit(FunDec *stm) {
    printIndent();
    cout << "function " << stm->fname << "(";
    // Agrupar parámetros por tipo
    auto paramNameIt = stm->paramNames.begin();
    auto paramTypeIt = stm->paramTypes.begin();
    while (paramTypeIt != stm->paramTypes.end()) {
        std::list<std::string> paramsWithSameType;
        std::string currentType = *paramTypeIt;
        while (paramTypeIt != stm->paramTypes.end() && *paramTypeIt == currentType) {
            paramsWithSameType.push_back(*paramNameIt);
            ++paramNameIt;
            ++paramTypeIt;
        }
        // Imprimir parámetros con el mismo tipo
        for (auto it = paramsWithSameType.begin(); it != paramsWithSameType.end(); ++it) {
            cout << *it;
            if (std::next(it) != paramsWithSameType.end()) {
                cout << ", ";
            }
        }
        cout << " : " << currentType;
        if (paramTypeIt != stm->paramTypes.end()) {
            cout << "; ";
        }
    }
    cout << ") : " << stm->rtype << ";" << endl;
    stm->body->accept(this);
    cout << ";" << endl;
}


void PrintVisitor::visit(FunDecList *stm) {
    for (auto i : stm->flist) {
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body *stm) {
    stm->vardecs->accept(this);
    cout <<"begin" <<endl;
    increaseIndent();
    stm->statements->accept(this);
    decreaseIndent();
    cout <<"end";
}

void PrintVisitor::printIndent() {
    for (int i = 0; i < indentLevel * 2; i++) {
        cout << ' ';
    }
}

