#include "imp_codegen.hh"

ImpCodeGen::ImpCodeGen(ImpTypeChecker* a):analysis(a) {

}

void ImpCodeGen::codegen(string label, string instr) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << endl;
}

void ImpCodeGen::codegen(string label, string instr, int arg) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << arg << endl;
}

void ImpCodeGen::codegen(string label, string instr, string jmplabel) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << jmplabel << endl;
}

string ImpCodeGen::next_label() {
  string l = "L";
  string n = to_string(current_label++);
  l.append(n);
  return l;
}

string ImpCodeGen::get_flabel(string fname) {
  string l = "L";
  l.append(fname);
  return l;
}

void ImpCodeGen::codegen(Program* p, string outfname) {
  nolabel = "";
  current_label = 0;
  p->accept(this);
  ofstream outfile;
  outfile.open(outfname);
  outfile << code.str();
  outfile.close();

  return;
}

// CAMBIAR.
void ImpCodeGen::visit(Program* p) {
  current_dir = 0;
  max_stack = 0;
  mem_globals = 0;
  direcciones.add_level();

    process_global = true;
    p->varDecs->accept(this);
    process_global = false;
    mem_globals = current_dir;
    // codegen
    codegen("start","skip");
    codegen(nolabel,"enter",mem_globals);
    codegen(nolabel,"alloc",mem_globals);
    codegen(nolabel,"mark");
    codegen(nolabel,"pusha",get_flabel(p->NameProgram));
    codegen(nolabel,"call");
    codegen(nolabel,"halt");

    if(p->funDecs != nullptr) p->funDecs->accept(this);
    else num_params = 0;

    codegen(get_flabel(p->NameProgram),"skip");

    codegen(nolabel,"enter", mem_globals + max_stack);
    codegen(nolabel,"alloc", 0);
    p->stmList->accept(this);
    codegen(nolabel,"return", 3);
    direcciones.remove_level();



  return ;
}

void ImpCodeGen::visit(Body * b) {
  int dir = current_dir;
  direcciones.add_level();

  // ni una variable sería global.

  b->vardecs->accept(this);
  b->statements->accept(this);
  
  direcciones.remove_level();

  current_dir = dir;
  return ;
}

void ImpCodeGen::visit(VarDecList* s) {
  list<VarDec*>::iterator it;
  for (it = s->vardecs.begin(); it != s->vardecs.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}

void ImpCodeGen::visit(VarDec* vd) {
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it){
    current_dir++;
    VarEntry ventry;
    ventry.dir = current_dir;
    ventry.is_global = process_global;
    direcciones.add_var(*it, ventry);
  }
  return ;
}

void ImpCodeGen::visit(FunDecList* s) {
  list<FunDec*>::iterator it;
  for (it = s->flist.begin(); it != s->flist.end(); ++it) {
    (*it)->accept(this);
  }
  return ;
}

void ImpCodeGen::visit(FunDec* fd) {
  FEntry fentry = analysis->ftable.lookup("L"+fd->fname);
  current_dir = 0;
  list<string>::iterator it, vit;
  // Parameters
  int i = 1;
  int m = fd->paramTypes.size();
  VarEntry ventry;
  for (it = fd->paramTypes.begin(), vit = fd->paramNames.begin();
       it != fd->paramTypes.end(); ++it, ++vit){
    ventry.is_global = false;
    ventry.dir = i-(m+3); // dirección de los parámetros de la función. (van bajando 1 por 1 en negativo)
    direcciones.add_var(*vit,ventry);
    i++;
    // cout << *it << " " << *vit;
  }

  ImpType ftype = fentry.ftype;
  if (ftype.types[ftype.types.size()-1] != ImpType::VOID) {
    ventry.dir = -(m+3); // El return tiene esta dirección. Es la última posición negativo, osea
    // -m+3 es return, -m+2 es primer parámetro -m+1 es 2do parámetro, y así con todos los parámetros hasta 0.
    direcciones.add_var(fd->fname, ventry); // maybe moficiar esto.
  }

  codegen(get_flabel(fd->fname),"skip");
  codegen(nolabel,"enter",fentry.mem_locals+fentry.max_stack);
  codegen(nolabel,"alloc",fentry.mem_locals);

  max_stack += fentry.max_stack;
  num_params = m;
  fd->body->accept(this);

  codegen(nolabel,"return",num_params+3);

  return ;
}


void ImpCodeGen::visit(StmList* s) {
  list<Stm*>::iterator it;
  for (it = s->stms.begin(); it != s->stms.end(); ++it) {
    (*it)->accept(this);
  }
  return ;
}

void ImpCodeGen::visit(AssignStatement* s) {
  s->exp->accept(this);
  VarEntry ventry = direcciones.lookup(s->id);
  if (ventry.is_global)
    codegen(nolabel,"store", ventry.dir);
  else
    codegen(nolabel,"storer", ventry.dir);
  return ;
}

void ImpCodeGen::visit(PrinteoStatement* s) {

  if(s->exp != NULL) s->exp->accept(this);

  if(s->TypePrint=="writeln") code << "print" << endl;
  else code << "printn" << endl;
  return ;
}

// hay que verlo.
void ImpCodeGen::visit(IfStatement* s) {

    list<string> etiquetas;

    list<Exp*>::iterator itExp;
    list<StmList*>::iterator itStmList;

    string l1 = next_label();

    for(int i = 0; i < s->conditions.size() ; ++i) etiquetas.push_back(next_label());

    list<string>::iterator itEtiquetas;

    for(itExp = s->conditions.begin(), itStmList = s->conditionBodies.begin(), itEtiquetas = etiquetas.begin();
            itExp != s->conditions.end(); ++itExp, ++itStmList, ++itEtiquetas){

        (*itExp)->accept(this);
        codegen(nolabel, "jmpz", *itEtiquetas);
        (*itStmList)->accept(this);
        codegen(nolabel, "goto", l1);
        codegen(*itEtiquetas, "skip");

    }

    if (s->conditions.size() != s->conditionBodies.size()) {
        (*itStmList)->accept(this); // Ejecutamos el bloque `else`
    }

    codegen(l1, "skip");


}

void ImpCodeGen::visit(WhileStatement* s) {
  string l1 = next_label();
  string l2 = next_label();

  codegen(l1, "skip");
  s->exp->accept(this);
  codegen(nolabel, "jmpz", l2);
  s->stms->accept(this);
  codegen(nolabel, "goto", l1);
  codegen(l2, "skip");

}

void ImpCodeGen::visit(DoWhileStatement* s){
    string l1 = next_label();
    string l2 = next_label();

    codegen(l1, "skip");
    s->stms->accept(this);

    dowhile_op = true;
    s->exp->accept(this);
    dowhile_op = false;

    codegen(nolabel, "jmpz", l2);
    codegen(nolabel, "goto", l1);
    codegen(l2, "skip");
}

void ImpCodeGen::visit(ForStatement* s) {

    string l1 = next_label();
    string l2 = next_label();

    VarEntry varEntry = direcciones.lookup(s->id);

    s->exp1->accept(this);

    bool is_var_global = varEntry.is_global;

    if(is_var_global) codegen(nolabel, "store", varEntry.dir);
    else codegen(nolabel, "storer", varEntry.dir);

    codegen(l1, "skip");

    if(is_var_global) codegen(nolabel, "load", varEntry.dir);
    else codegen(nolabel, "loadr", varEntry.dir);

    s->exp2->accept(this);

    codegen(nolabel, "le");

    codegen(nolabel, "jmpz", l2);

    s->stms->accept(this);

    if(is_var_global) codegen(nolabel, "load", varEntry.dir);
    else codegen(nolabel, "loadr", varEntry.dir);

    if(s->increase_or_decrease == "to") codegen(nolabel, "push", 1);
    else codegen(nolabel, "push", -1);

    codegen(nolabel, "add");

    if(is_var_global) codegen(nolabel, "store", varEntry.dir);
    else codegen(nolabel, "storer", varEntry.dir);

    codegen(nolabel, "goto", l1);
    codegen(l2, "skip");


  return ;
}


int ImpCodeGen::visit(BinaryExp* e) {
  e->left->accept(this);
  e->right->accept(this);
  string op = "";
  switch(e->op) {
  case PLUS_OP: op =  "add"; break;
  case MINUS_OP: op = "sub"; break;
  case MUL_OP:  op = "mul"; break;
  case DIV_OP:  op = "div"; break;
  case LT_OP: {
      if(dowhile_op) op = "ge";
      else op = "lt";
      break;
  }
  case LE_OP: {
      if(dowhile_op) op = "gt";
      else op = "le";
      break;
  }
  case DT_OP: {
      if(dowhile_op) op = "le";
      else op = "gt";
      break;
  }
  case DE_OP: {
      if(dowhile_op) op = "lt";
      else op = "ge";
      break;
  }
  case EQ_OP: {
      if(dowhile_op) op = "df";
      else op = "eq";
      break;
  }
  case DIF_OP: {
      if(dowhile_op) op = "eq";
      else op = "df";
      break;
  }
  default: cout << "binop " << Exp::BinaryToChar(e->op) << " not implemented" << endl;
  }
  codegen(nolabel, op);
  return 0;
}

int ImpCodeGen::visit(NumberExp* e) {

  if(e->value < 0){
      codegen(nolabel, "push", -1);
      codegen(nolabel, "push", abs(e->value));
      codegen(nolabel, "mul");
  }

  codegen(nolabel,"push",e->value);
  return 0;
}

int ImpCodeGen::visit(BoolExp* e) {
  codegen(nolabel,"push",e->value ? 1 : 0);
 
  return 0;
}

int ImpCodeGen::visit(IdentifierExp* e) {
  VarEntry ventry = direcciones.lookup(e->id);
  if (ventry.is_global)
    codegen(nolabel,"load",ventry.dir);
  else
    codegen(nolabel,"loadr",ventry.dir);
  return 0;
}

// ver esto.
int ImpCodeGen::visit(FCallExp* e) {
  // nuevo
  FEntry fentry = analysis->ftable.lookup("L"+e->fname);
  ImpType ftype = fentry.ftype;
  if (ftype.types[ftype.types.size()-1] != ImpType::VOID) {
    codegen(nolabel,"alloc",1);
  }
  list<Exp*>::iterator it;
  for (it = e->args.begin(); it != e->args.end(); ++it) {
    (*it)->accept(this);
  }
  codegen(nolabel,"mark");
  codegen(nolabel,"pusha",get_flabel(e->fname));
  codegen(nolabel,"call");
  return 0;
}


void ImpCodeGen::visit(FCallStatement* s) {
  // nuevo
  FEntry fentry = analysis->ftable.lookup("L"+s->fname);
  ImpType ftype = fentry.ftype;
  if (ftype.types[ftype.types.size()-1] != ImpType::VOID) {
    codegen(nolabel,"alloc",1);
  }
  list<Exp*>::iterator it;
  for (it = s->args.begin(); it != s->args.end(); ++it) {
    (*it)->accept(this);
  }
  codegen(nolabel,"mark");
  codegen(nolabel,"pusha",get_flabel(s->fname));
  codegen(nolabel,"call");
  return ;
}
