#include "visitor.h"
#include "Exp.h"
#include <unordered_map>

unordered_map<string, int> memoria;

int IdentifierExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int BinaryExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor *visitor) {
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

int StmList::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int Program::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

/// Print Visitor

void PrintVisitor::imprimir(Program *p) {

    p->accept(this);

}

void PrintVisitor::visit(Program *p) {

    p->sl->accept(this);

}

void PrintVisitor::visit(StmList *sl) {

    for(auto stm : sl->stms){
        stm->accept(this);

        if(stm != sl->stms.back()) cout<<"\n";
    }

}

void PrintVisitor::visit(AssignStatement *s) {

    cout<<s->id <<" := ";
    s->exp->accept(this);
    cout<<";";

}

void PrintVisitor::visit(PrinteoStatement *s) {

    if(s->TypePrint == "WriteLn"){
        cout<<"WriteLn(";
        s->exp->accept(this);
        cout<<");";
    } else if(s->TypePrint == "Write"){
        cout<<"Write(";
        s->exp->accept(this);
        cout<<");";
    }
    else{
        cout<<"No debería llegar acá (PrintVisitor - PrinteoStatement)\n";
        exit(0);
    }

}

void PrintVisitor::visit(IfStatement *s) {

    list<Exp*>::iterator itExp;
    list<StmList*>::iterator itStmList;

    // La cantidad de s->conditions, será siempre uno menos que s->conditionBodies (por el else)
    // lo cual, cuando de leer s->conditions, solo quedará el s->conditionBody del else.
    for(itExp = s->conditions.begin(), itStmList = s->conditionBodies.begin();
    itExp != s->conditions.end(); ++itExp, ++itStmList){

        if(itExp == s->conditions.begin()){
            cout<<"if "; (*itExp)->accept(this); cout<<" then\nbegin\n";
            (*itStmList)->accept(this); cout<<"\n";
            cout<<"end\n";
        }
        else{
            cout<<"else if "; (*itExp)->accept(this); cout <<" then\nbegin\n";
            (*itStmList)->accept(this); cout<<"\n";
            cout<<"end\n";
        }

    }

    // Esto siempre y cuando va a cumplir cuando se declare else.
    if(itStmList != s->conditionBodies.end()){
        cout<<"else\nbegin\n";
        (*itStmList)->accept(this);
        cout<<"\nend\n";
    }

}

int PrintVisitor::visit(BinaryExp *e) {

    e->left->accept(this);
    cout<< ' ' << Exp::BinaryToChar(e->op) << ' ';
    e->right->accept(this);

    return 0;
}

int PrintVisitor::visit(NumberExp *e) {
    cout<<e->value;
    return 0;
}

int PrintVisitor::visit(IdentifierExp *e) {

    cout << e->id;
    return 0;

}

/// Eval Visitor

void EvalVisitor::ejecutar(Program *p) {
    p->accept(this);
}

void EvalVisitor::visit(Program *p) {
    p->sl->accept(this);
}

void EvalVisitor::visit(StmList *sl) {

    for(Stm* stm : sl->stms){
        stm->accept(this);
    }

}

void EvalVisitor::visit(AssignStatement *s) {

    memoria[s->id] = s->exp->accept(this);

}

void EvalVisitor::visit(PrinteoStatement *s) {

    if(s->TypePrint == "WriteLn"){
        cout << s->exp->accept(this) <<"\n";
    }
    else if(s->TypePrint == "Write"){
        cout<<s->exp->accept(this);
    }
    else{
        cout<<"No debería llegar acá (EvalVisitor - PrinteoStatement)\n";
        exit(0);
    }

}

void EvalVisitor::visit(IfStatement *s) {

    // ver PrintVisitor de IfStatement.

    list<Exp*>::iterator itExp;
    list<StmList*>::iterator itStmList;
    bool else_or_not = true;

    for(itExp = s->conditions.begin(), itStmList = s->conditionBodies.begin();
        itExp != s->conditions.end(); ++itExp, ++itStmList){

        if((*itExp)->accept(this)){
            (*itStmList)->accept(this);
            else_or_not = false;
            break;
        }

    }

    if(s->conditions.size() != s->conditionBodies.size()){
        if(else_or_not) (*itStmList)->accept(this);
    }

}

int EvalVisitor::visit(BinaryExp *e) {

    int left = e->left->accept(this);
    int right = e->right->accept(this);

    int result;

    switch (e->op) {
        case PLUS_OP: result = left + right; break;
        case MINUS_OP: result = left - right; break;
        case MUL_OP: result = left * right; break;
        case DIV_OP: {
            if(right == 0){
                cout<< "Error, se está dividiendo entre 0. (EvalVisitor - BinaryExp)";
                exit(0);
            }
            else{
                result = left / right ; break;
            }
        }
        case EQ_OP: result = (left == right); break;
        case LE_OP: result = (left < right); break;
        case LT_OP: result = (left <= right); break;
        case DE_OP: result = (left > right); break;
        case DT_OP: result = (left >= right); break;
        default:
            cout<<"No debería llegar acá (EvalVisitor - BinaryExp)";
            exit(0);

    }

    return result;

}

int EvalVisitor::visit(NumberExp *e) {
    return e->value;
}

int EvalVisitor::visit(IdentifierExp *e) {

    return memoria[e->id];

}
