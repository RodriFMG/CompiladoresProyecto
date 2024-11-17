#include "imp_interpreter.h"

ImpValue BinaryExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue BoolExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue NumberExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue IdentifierExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue FCallExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void AssignStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void PrinteoStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void IfStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void WhileStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ForStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void StmList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void VarDec::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void FunDec::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void VarDecList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void FunDecList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void Body::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void Program::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void FCallStatement::accept(ImpValueVisitor *visitor) {
    return visitor->visit(this);
}

void ImpInterpreter::interpret(Program* p) {
    env.clear();
    p->accept(this);
    return;
}

void ImpInterpreter::visit(Program* p) {
    env.add_level();


    fdecs.add_level();
    p->funDecs->accept(this);
    p->varDecs->accept(this);
    p->stmList->accept(this);

}

void ImpInterpreter::visit(Body* b) {
    env.add_level();
    b->vardecs->accept(this);
    b->statements->accept(this);
    env.remove_level();
    return;
}

void ImpInterpreter::visit(VarDecList* decs) {
    list<VarDec*>::iterator it;
    for (it = decs->vardecs.begin(); it != decs->vardecs.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpInterpreter::visit(FunDecList* s) {
    list<FunDec*>::iterator it;
    for (it = s->flist.begin(); it != s->flist.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpInterpreter::visit(VarDec* vd) {

    list<string>::iterator it;
    ImpValue v;
    ImpVType tt = ImpValue::get_basic_type(vd->type);
    if (tt == NOTYPE) {
        cout << "Tipo invalido: " << vd->type << endl;
        exit(0);
    }
    v.set_default_value(tt);
    for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
        env.add_var(*it, v);
    }
    return;
}

void ImpInterpreter::visit(FunDec* fd) {

    fdecs.add_var(fd->fname, fd);
     return;
}

void ImpInterpreter::visit(StmList* s) {
    list<Stm*>::iterator it;
    for (it = s->stms.begin(); it != s->stms.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpInterpreter::visit(AssignStatement* s) {
    ImpValue v = s->exp->accept(this);

    if (!env.check(s->id)) {
        cout << "Variable " << s->id << " undefined" << endl;
        exit(0);
    }

    ImpValue lhs = env.lookup(s->id);

    if (lhs.type != v.type) {
        cout << "Type Error en Assign: Tipos de variable " << s->id;
        cout << " no coinciden" << endl;
        exit(0);
    }

    env.update(s->id, v);
    return;
}

void ImpInterpreter::visit(PrinteoStatement* s) {

    ImpValue v = s->exp->accept(this);

    if(s->TypePrint == "writeln"){
        cout << v << "\n";
    }
    else{
        cout << v;
    }

    return;
}

void ImpInterpreter::visit(IfStatement* s) {
    // Iteradores para recorrer las condiciones y los cuerpos de sentencias

    list<Exp*>::iterator itExp;
    list<StmList*>::iterator itStmList;
    bool else_or_not = true;


    // Recorremos las condiciones y los cuerpos de las sentencias en paralelo
    for (itExp = s->conditions.begin(), itStmList = s->conditionBodies.begin();
         itExp != s->conditions.end(); ++itExp, ++itStmList) {

        // Evaluamos cada condición

        ImpValue v = (*itExp)->accept(this);

        // Validamos que el valor de la condición sea de tipo booleano
        if (v.type != TBOOL) {
            cout << "Type error en If: esperaba bool en condicional" << endl;
            exit(0);
        }

        // Si la condición es verdadera, ejecutamos el bloque correspondiente
        if (v.bool_value) {
            (*itStmList)->accept(this);
            else_or_not = false; // Indica que no hay necesidad de evaluar más condiciones
            break;
        }
    }

    // Si hay un bloque `else` (caso especial donde el número de condiciones y cuerpos no coincide)
    if (s->conditions.size() != s->conditionBodies.size() && else_or_not) {
        (*itStmList)->accept(this); // Ejecutamos el bloque `else`
    }

    return;
}


void ImpInterpreter::visit(WhileStatement* s) {
    ImpValue v = s->exp->accept(this);
    if (v.type != TBOOL) {
        cout << "Type error en WHILE: esperaba bool en condicional" << endl;
        exit(0);
    }
    while(s->exp->accept(this).bool_value){
        s->stms->accept(this);
    }
}

void ImpInterpreter::visit(ForStatement* s) {

    env.add_level();

    ImpValue start = s->exp1->accept(this);
    ImpValue end = s->exp2->accept(this);

    if (start.type != TINT || end.type != TINT){
        cout << "Type error en FOR: esperaba int" << endl;
        exit(0);
    }

    ImpValue paso;
    paso.type = TINT;
    paso.set_default_value(TINT);
    if(s->increase_or_decrease == "downto") paso.int_value -= 1;
    else paso.int_value += 1;

    if (!env.check(s->id)) {
        cout << "Variable " << s->id << " undefined" << endl;
        exit(0);
    }

    ImpValue var = env.lookup(s->id);

    if(var.type != start.type){
        cout << "Type error en FOR: la comparación de la variable usada con el for, no se puede realizar.\n"
                "se esta comparando un " << var.type << " con un " << start.type;
    }


    env.update(s->id, start);


    while(env.lookup(s->id).int_value <= end.int_value){
        s->stms->accept(this);

        paso.int_value +=1;
        env.update(s->id, paso);
    }

    return;
}

ImpValue ImpInterpreter::visit(BinaryExp* e) {

    ImpValue result;
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);
    if (v1.type != TINT || v2.type != TINT) {
        cout << "Error de tipos: operandos en operacion binaria tienen que ser "
                "enteros"
             << endl;
        exit(0);
    }

    int iv, iv1, iv2;

    bool bv;

    ImpVType type = NOTYPE;
    iv1 = v1.int_value;
    iv2 = v2.int_value;


    switch (e->op) {
        case PLUS_OP:
            iv = iv1 + iv2;
            type = TINT;
            break;
        case MINUS_OP:
            iv = iv1 - iv2;
            type = TINT;
            break;
        case MUL_OP:
            iv = iv1 * iv2;
            type = TINT;
            break;
        case DIV_OP:
            iv = iv1 / iv2;
            type = TINT;
            break;
        case LT_OP:
            bv = (iv1 < iv2) ? 1 : 0;
            type = TBOOL;
            break;
        case LE_OP:
            bv = (iv1 <= iv2) ? 1 : 0;
            type = TBOOL;
            break;
        case EQ_OP:
            bv = (iv1 == iv2) ? 1 : 0;
            type = TBOOL;
            break;
        case DE_OP:
            bv = (iv1 >= iv2) ? 1 : 0;
            type = TBOOL;
            break;
        case DT_OP:
            bv = (iv1 > iv2) ? 1 : 0;
            type = TBOOL;
            break;
    }

    if (type == TINT)
        result.int_value = iv;
    else
        result.bool_value = bv;

    result.type = type;
    return result;
}

ImpValue ImpInterpreter::visit(NumberExp* e) {

    ImpValue v;
    v.set_default_value(TINT);
    v.int_value = e->value;
    return v;

}

ImpValue ImpInterpreter::visit(BoolExp* e) {

    ImpValue v;
    v.set_default_value(TBOOL);
    v.int_value = e->value;
    return v;
}

ImpValue ImpInterpreter::visit(IdentifierExp* e) {

    if (env.check(e->id))
        return env.lookup(e->id);
    else {
        cout << "Variable indefinida: " << e->id << endl;
        exit(0);
    }
}

ImpValue ImpInterpreter::visit(FCallExp* e) {


    FunDec* fdec = fdecs.lookup(e->fname);

    env.add_level();

    list<Exp*>::iterator it;

    list<string>::iterator varit;

    list<string>::iterator vartype;

    ImpVType tt;

    if (fdec->paramNames.size() != e->args.size()) {
        cout << "Error: Numero de parametros incorrecto en llamada a "
             << fdec->fname << endl;
        exit(0);
    }


    for (it = e->args.begin(), varit = fdec->paramNames.begin(),
         vartype = fdec->paramTypes.begin();
         it != e->args.end(); ++it, ++varit, ++vartype) {

        tt = ImpValue::get_basic_type(*vartype);

        ImpValue v = (*it)->accept(this);

        if (v.type != tt) {
            cout << "Error FCall: Tipos de param y arg no coinciden. Funcion "
                 << fdec->fname << " param " << *varit << endl;
            exit(0);
        }

        env.add_var(*varit, v);
    }

    // Agregamos el nombre de la función como variable.
    ImpValue v1;
    ImpVType tt1 = ImpValue::get_basic_type(fdec->rtype);
    if (tt1 == NOTYPE) {
        cout << "El tipo de retorno de la función, no es válida, " << fdec->rtype;
        exit(0);
    }

    v1.set_default_value(tt1);

    if(env.lookup(fdec->fname, v1)){
        cout << "Error, ya existe una variable con el mismo nombre de la función.\n";
        exit(0);
    }

    env.add_var(fdec->fname,v1);

    fdec->body->accept(this);

    retval = env.lookup(fdec->fname);

    env.remove_level();

    return retval;
}


// Esto es más que todo para funciones void, las cuales aún no hemos realizado.
void ImpInterpreter::visit(FCallStatement* s) {

    FunDec* fdec = fdecs.lookup(s->fname);

    env.add_level();

    list<Exp*>::iterator it;

    list<string>::iterator varit;

    list<string>::iterator vartype;

    ImpVType tt;

    if (fdec->paramNames.size() != s->args.size()) {
        cout << "Error: Numero de parametros incorrecto en llamada a "
             << fdec->fname << endl;
        exit(0);
    }

    for (it = s->args.begin(), varit = fdec->paramNames.begin(),
         vartype = fdec->paramTypes.begin();
         it != s->args.end(); ++it, ++varit, ++vartype) {

        tt = ImpValue::get_basic_type(*vartype);

        ImpValue v = (*it)->accept(this);

        if (v.type != tt) {
            cout << "Error FCall: Tipos de param y arg no coinciden. Funcion "
                 << fdec->fname << " param " << *varit << endl;
            exit(0);
        }

        env.add_var(*varit, v);
    }

    fdec->body->accept(this);

    env.remove_level();



}