#include "imp_value.h"

ImpValue::ImpValue():type(NOTYPE) { }

ImpVType ImpValue::get_basic_type(string s) {
    ImpVType tt;
    if (s.compare("Integer")==0 || s.compare("integer")==0) tt = TINT;
    else if (s.compare("Longint")==0 || s.compare("longint")==0) tt = TLONGINT;
    else if (s.compare("Boolean")==0 || s.compare("boolean")==0) tt = TBOOL;
    else tt = NOTYPE;
    return tt;
}

void ImpValue::set_default_value(ImpVType tt) {
    type = tt;
    if (tt == TINT) {
        int_value = 0;
    }
    else if (tt == TLONGINT) {
        longint_value=0;
    }
    else if (tt == TBOOL) {
        bool_value = true;
    }
    return;
}

std::ostream& operator << ( std::ostream& outs, const ImpValue & v )
{
    if (v.type == TINT) {
        outs << v.int_value;
    }
    else if (v.type == TLONGINT){
        outs << v.longint_value;
    }
    else if (v.type == TBOOL) {
        if (v.bool_value)
            outs << "true";
        else
            outs << "false";
    } else {
        outs << "NOTYPE";
    }
    return outs;
}


