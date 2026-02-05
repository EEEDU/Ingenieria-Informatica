#include "Cuenta.h"
#include <cmath>


Cuenta::Cuenta(double cap_ini, double ini)
    : capital_inicial(cap_ini),
      interes(ini)
{
}

CuentaCorriente::CuentaCorriente(double cap_ini, double ini)
    : Cuenta(cap_ini, ini)
{
}

double CuentaCorriente::valor(int t) const
{
    return capital_inicial + pow(1 + (interes / 100), t);
}

PlazoFijo::PlazoFijo(double cap_ini, double ini, int pla)
    : Cuenta(cap_ini, ini),
    plazo(pla)
{
}

double PlazoFijo::valor(int t) const
{
    if (t < plazo)
    {
        return capital_inicial;
    }
    else{
        return capital_inicial + (1 + (interes / 100));
    }
}

CuentaNomina::CuentaNomina(double cap_ini, double ini, double nom)
    : Cuenta(cap_ini, ini),
    nomina(nom)
{
}

double CuentaNomina::valor(int t) const
{
    return capital_inicial + t * nomina;
}

CuentaDivisa::CuentaDivisa(double cap_ini, double ini)
    : Cuenta(cap_ini, ini)
{
}
