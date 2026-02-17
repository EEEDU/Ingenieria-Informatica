#include "CuentaNomina.h"

CuentaNomina::CuentaNomina(double cap_ini, double nom)
    : Cuenta(cap_ini),
    nomina(nom)
{
}

double CuentaNomina::valor(const int t) const
{
    int t_actual = t + mes;
    return capital_inicial + t_actual * nomina;
}
