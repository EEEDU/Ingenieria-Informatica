#include "Cuenta.h"


Cuenta::Cuenta(double cap_ini)
    : capital_inicial(cap_ini), mes(0)
{
}
Cuenta::~Cuenta() = default;


double Cuenta::tae() const
{
    return 100 * ((valor(12)/capital_inicial) - 1);
}

void Cuenta::actualizar()
{
    mes++;
}

