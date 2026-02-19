#include "Cuenta.h"


Cuenta::Cuenta(double cap_ini, double ini)
    : capital_inicial(cap_ini)
{
}
Cuenta::~Cuenta() = default;


double Cuenta::tae()
{
    return 100 * ((valor(12)/capital_inicial) - 1);
}

