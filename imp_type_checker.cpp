#include"imp_type_checker.hh"

ImpTypeChecker::ImpTypeChecker():inttype(),booltype(),voidtype() {
  inttype.set_basic_type("Integer");
  booltype.set_basic_type("Boolean");
  longinttype.set_basic_type("longint");
  voidtype.set_basic_type("procedure");
}

void ImpTypeChecker::sp_incr(int n) {
  sp++;
  if (sp > max_sp) max_sp = sp;
}

void ImpTypeChecker::sp_decr(int n) {
  sp--;
  if (sp < 0) {
    cout << "stack less than 0" << endl;
    exit(0);
  }
}

void ImpTypeChecker::typecheck(Program* p) {
  env.clear();
  p->accept(this);
  return;
}

void ImpTypeChecker::visit(Program* p) {
  env.add_level();
  ftable.add_level();

    if(p->FirstFunctions){
        if(p->funDecs != nullptr){
            p->funDecs->accept(this);
        }
        else {
            sp = max_sp = 0;
            dir = max_dir = 0;
        }

        p->varDecs->accept(this);
    }
    else{
        p->varDecs->accept(this);
        if(p->funDecs != nullptr){
            p->funDecs->accept(this);
        }
        else {
            sp = max_sp = 0;
            dir = max_dir = 0;
        }
    }



  p->stmList->accept(this);


  env.remove_level();

  for(int i = 0; i < fnames.size(); i++) {
      cout << "\n";
    cout << "-- Function: " << fnames[i] << endl;
    FEntry fentry = ftable.lookup("L"+fnames[i]);

    cout << fentry.fname << " : " << fentry.ftype << endl;
    // added
    cout << "max stack height: " << fentry.max_stack << endl;
    cout << "mem local variables: " << fentry.mem_locals << endl;
  }
  
  return;
}

// dir : cantidad de variables declaradas tanto global como localmente (en una función)
// sp : Expresiones matemáticas intermedias, registra el uso del stack.

void ImpTypeChecker::visit(Body* b) {
  int start_dir = dir;  // added
  env.add_level();
  b->vardecs->accept(this);
  b->statements->accept(this);
  env.remove_level();
  dir = start_dir;  // added
  return;
}

void ImpTypeChecker::visit(VarDecList* decs) {
  list<VarDec*>::iterator it;
  for (it = decs->vardecs.begin(); it != decs->vardecs.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}

void ImpTypeChecker::visit(FunDecList* s) {
  list<FunDec*>::iterator it;

  // Primero agrego cada función al env de funciones.
  for (it = s->flist.begin(); it != s->flist.end(); ++it) {
    add_fundec(*it);
  }

  for (it = s->flist.begin(); it != s->flist.end(); ++it) {
    // added
    // Como las funciones generalmente son lo primero que se inicializa pues,
    sp = max_sp = 0;
    dir = max_dir = 0;
    // end-added
    (*it)->accept(this); // Rescato el tipo de la función y lo agrego todo
    FEntry fentry;
    string fname  = (*it)->fname;
    fentry.fname = fname;
    fentry.ftype = env.lookup("L" + fname);

    fnames.push_back(fname);
    // added

    // Aca almaceno toda la memoria utilizada en la funciones (osea todas las experesiones matemáticas utilizadas) y
    // todo el dir, osea todas las variables locales declaradas.
    fentry.max_stack = max_sp;
    fentry.mem_locals = max_dir;
    // end added

    // Meto todo el nombre de la función, el tipo, Exp utilizaedas, variables utilizadas en la función.
    ftable.add_var("L"+fname, fentry);
  }

  return;
}

void ImpTypeChecker::visit(VarDec* vd) {
  ImpType type;
  // Asigno el tipo a la varibale.
  type.set_basic_type(vd->type);
  // Verifico el tipo.
  if (type.ttype==ImpType::NOTYPE || type.ttype==ImpType::VOID) {
    cout << "Tipo invalido: " << vd->type << endl;
    exit(0);
  }

  // Almaceno la variable al env. Además, como es una declaraicón de varibale, aumento el dir.
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
    env.add_var(*it, type);
    dir++;
    if (dir > max_dir) max_dir = dir;
  }   
  return;

  // El env es tanto para variables como para funciones.
}

void ImpTypeChecker::add_fundec(FunDec* fd) {
  ImpType funtype;
  if (!funtype.set_fun_type(fd->paramTypes,fd->rtype)) {
    cout << "Tipo invalido en declaracion de funcion: " << fd->fname << endl;
    exit(0);
  }

  env.add_var("L"+fd->fname, funtype);
  return;
}

void ImpTypeChecker::visit(FunDec* fd) {
  env.add_level();
  ImpType funtype = env.lookup("L"+fd->fname);

  // rtype es el tipo de retorno, y el ptype es el tipo de parámetro.
  ImpType rtype, ptype;

  // El últimmo type de la lista de type de la función = type de retorno.
  rtype.set_basic_type(funtype.types.back());
  list<string>::iterator it;
  int i=0;
  for (it = fd->paramNames.begin(); it != fd->paramNames.end(); ++it, ++i) {
      // Cada parámetro de la función lo meto al env.
    ptype.set_basic_type(funtype.types[i]);
    env.add_var(*it,ptype);
  }
  // con el rtype, creo una "variable" llamada return, para asegurar el tipo de retorno.
  env.add_var(fd->fname, rtype);

  fd->body->accept(this);
  env.remove_level();
  return;
}

void ImpTypeChecker::visit(StmList* s) {
  list<Stm*>::iterator it;
  for (it = s->stms.begin(); it != s->stms.end(); ++it) {
    (*it)->accept(this);
  }
  return;
}

void ImpTypeChecker::visit(AssignStatement* s) {
  ImpType type = s->exp->accept(this);
  // Si esa variable no está ubicada en la lista de variables disponibles, pues ERROR.
  if (!env.check(s->id)) {
    cout << "Variable " << s->id << " undefined" << endl;
    exit(0);
  }

  sp_decr(1);
  ImpType var_type = env.lookup(s->id);

  if(var_type.match(booltype)){
      if(!type.match(var_type)) {
          cout << "Tipo incorrecto en Assign a " << s->id << endl;
          exit(0);
      }
  }
  else if (var_type.match(inttype) || var_type.match(longinttype)){

      if(!type.match(longinttype) && !type.match(inttype)){
          cout << "Tipo incorrecto en Assign a " << s->id << endl;
          exit(0);
      }

  }
  else{
      cout << "Tipo de variable " << s->id << " no soportado para asignación." << endl;
      exit(0);
  }

  return;
}

void ImpTypeChecker::visit(PrinteoStatement* s) {

  if(s->exp != NULL){
      s->exp->accept(this);
      sp_decr(1); // Dentro del write() haces una operación matemática, pues aumento.
  }
  return;
}


void ImpTypeChecker::visit(IfStatement* s) {

    list<Exp*>::iterator itExp;
    list<StmList*>::iterator itStmList;


    for( itExp = s->conditions.begin(), itStmList = s->conditionBodies.begin();
    itExp != s->conditions.end(); ++itExp, ++itStmList){

        if(!(*itExp)->accept(this).match(booltype)){
            cout << "Alguna de las condiciones colocadas no es de tipo BOOL.\n";
            exit(1);
        }

        sp_decr(1);

        (*itStmList)->accept(this);

    }


    if( s->conditions.size() != s->conditionBodies.size() ){
        (*itStmList)->accept(this);
    }

  return;
}

void ImpTypeChecker::visit(WhileStatement* s) {
    // si la condicional no es del tipo esperado, (OSEA BOOL) ERRROORR.
  if (!s->exp->accept(this).match(booltype)) {
    cout << "Expresion conditional en IF debe de ser bool" << endl;
    exit(0);
  }
  // aumento por la condicional del while.
  sp_decr(1);
  s->stms->accept(this);
 return;
}



void ImpTypeChecker::visit(ForStatement* s) {

  if(env.lookup(s->id).match(booltype)){
      cout << "La variable de id " << s->id << " es de tipo BOOLEAN, se esperaba un INTEGER o LOGNINT para el FOR.";
      exit(0);
  }

  if(s->exp1->accept(this).match(booltype) || s->exp2->accept(this).match(booltype)) {
    cout << "Expresiones en for deben de ser int o longint" << endl;
    exit(0);
  }

  sp_decr(3);
  s->stms->accept(this);
  return;
}


ImpType ImpTypeChecker::visit(BinaryExp* e) {
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);

  // Se espera que ambos sean tipo ENTEROS (HAY QUE MODIFICAR ESTO.)
  if (t1.match(booltype) || t2.match(booltype)) {
    cout << "Tipos en BinExp deben de ser int o longint" << endl;
    exit(0);
  }
  ImpType result;
  switch(e->op) {
  case PLUS_OP:
  case MINUS_OP:
  case MUL_OP:
  case DIV_OP:
    if(t1.match(longinttype) || t2.match(longinttype)) result = longinttype;
    else result = inttype;
    break;
  case LT_OP: 
  case LE_OP:
  case DE_OP:
  case DT_OP:
  case EQ_OP:
    result = booltype;
    break;
  }

  sp_decr(1);
  return result;
}

ImpType ImpTypeChecker::visit(NumberExp* e) {
  sp_incr(1);

  return inttype;
}

ImpType ImpTypeChecker::visit(BoolExp* e) {
    // los bools tambien cuentan como operaciones.
  sp_incr(1);
  return booltype;
}

// Creo que por cada posible número, id o booleano usado para una operación, pues se suma un 1 al sp.

ImpType ImpTypeChecker::visit(IdentifierExp* e) {
    // asignar un id tambien es +1.
  sp_incr(1);
  if (env.check(e->id))
    return env.lookup(e->id);
  else {
    cout << "Variable indefinida: " << e->id << endl;
    exit(0);
  }
}

ImpType ImpTypeChecker::visit(FCallExp* e) {
  return process_fcall(e);
}

ImpType ImpTypeChecker::process_fcall(FCallExp* e) {

    if (!env.check("L" + e->fname)) {
      cout << "(Function call): " << e->fname <<  " no existe" << endl;
      exit(0);
    }

    ImpType funtype = env.lookup("L"+e->fname);
    if (funtype.ttype != ImpType::FUN) {
      cout << "(Function call): " << e->fname <<  " no es una funcion" << endl;
      exit(0);
    }


    // check args
    int num_fun_args = funtype.types.size()-1;
    int num_fcall_args = e->args.size();
    ImpType rtype;
    // Rescato el tipo de retorno.
    rtype.set_basic_type(funtype.types[num_fun_args]);
    // added

    // Si la función no es de tipo void significa que tendrá un EXP matemática de retorno.


    if (!rtype.match(voidtype)) {
      sp_incr(1);
    }

    // Compara los parámetros de la función con los mandados en la llamada.
    if (num_fun_args != num_fcall_args) {
      cout << "(Function call) Numero de argumentos no corresponde a declaracion de: " << e->fname << endl;
      exit(0);
    }
    ImpType argtype;
    list<Exp*>::iterator it;
    int i = 0;
    for (it = e->args.begin(); it != e->args.end(); ++it) {
      argtype = (*it)->accept(this);
      // compara todos los tipos de dato de cada parámetro que le mandamos con los parámetro originales de la función.
      if (argtype.ttype != funtype.types[i]) {
          cout << "(Function call) Tipo de argumento no corresponde a tipo de parametro en fcall de: " << e->fname << endl;
          exit(0);
      }
      i++;
    }

    sp_decr(1); // xd?


    return rtype;
}


//Si o si debe ser de tipo void
void ImpTypeChecker::visit(FCallStatement* s) {
  if (!env.check("L"+s->fname)) {
    cout << "(Function call): " << s->fname <<  " no existe" << endl;
    exit(0);
  }
  ImpType funtype = env.lookup("L"+s->fname);
  if (funtype.ttype != ImpType::FUN) {
    cout << "(Function call): " << s->fname <<  " no es una funcion" << endl;
    exit(0);
  }
  // check args
  int num_fun_args = funtype.types.size()-1;
  int num_fcall_args = s->args.size();
  ImpType rtype;
  rtype.set_basic_type(funtype.types[num_fun_args]);
  // added
  if (!rtype.match(voidtype)) {
    cout << "(Function call): " << s->fname <<  " no es de tipo void" << endl;
    exit(0);
  }
      if (num_fun_args != num_fcall_args) {
      cout << "(Function call) Numero de argumentos no corresponde a declaracion de: " << s->fname << endl;
      exit(0);
    }
    ImpType argtype;
    list<Exp*>::iterator it;
    int i = 0;
    for (it = s->args.begin(); it != s->args.end(); ++it) {
      argtype = (*it)->accept(this);
      if (argtype.ttype != funtype.types[i]) {
          cout << "(Function call) Tipo de argumento no corresponde a tipo de parametro en fcall de: " << s->fname << endl;
          exit(0);
      }
      i++;
    }
    sp_decr(i);
  return;
}