#include "CuentaCorriente.h"
#include <cmath>

CuentaCorriente::CuentaCorriente(double cap_ini, double ini)
    : Cuenta(cap_ini), interes(ini)  
{
}

double CuentaCorriente::valor(const int t) const
{   
    int t_actual = t + mes;
    return capital_inicial * pow(1 + (interes / 100), t_actual);
}

