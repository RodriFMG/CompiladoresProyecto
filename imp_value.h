#ifndef COMPILADORESPROYECTO_IMP_VALUE_H
#define COMPILADORESPROYECTO_IMP_VALUE_H

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;


enum ImpVType { NOTYPE=0, TINT, TLONGINT, TBOOL};


class ImpValue {
public:
    ImpValue();
    ImpVType type;
    int int_value;
    long long longint_value;
    bool bool_value;
    void set_default_value(ImpVType tt);
    static ImpVType get_basic_type(string s);
};

std::ostream& operator << ( std::ostream& outs, const ImpValue & v );

#endif //COMPILADORESPROYECTO_IMP_VALUE_H


