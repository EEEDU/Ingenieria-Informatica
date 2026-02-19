#include "CuentaCorriente.h"
#include <cmath>

CuentaCorriente::CuentaCorriente(double cap_ini, double ini)
    : Cuenta(cap_ini), interes(ini) 
{
}

double CuentaCorriente::valor(int t) const
{
    return capital_inicial * pow(1 + (interes / 100), t);
}

