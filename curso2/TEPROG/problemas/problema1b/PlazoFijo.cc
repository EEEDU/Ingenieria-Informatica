#include "PlazoFijo.h"

PlazoFijo::PlazoFijo(double cap_ini, double ini, int pla)
    : Cuenta(cap_ini),
    interes(ini),
    plazo(pla)
{
}

double PlazoFijo::valor(const int t) const
{
    int t_actual = t + mes;
    if (t_actual)
    {
        return capital_inicial;
    }
    else{
        return capital_inicial + (1 + (interes / 100));
    }
}

double PlazoFijo::tae() const
{
    return capital_inicial + 12 * ((valor(plazo) - capital_inicial) / plazo);
}

