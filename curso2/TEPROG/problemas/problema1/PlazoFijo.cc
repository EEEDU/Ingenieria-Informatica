#include "PlazoFijo.h"

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
