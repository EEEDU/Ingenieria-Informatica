#include "CuentaDivisa.h"

CuentaDivisa::CuentaDivisa(Cuenta* cuenta, double fact) 
    : Cuenta(cuenta->valor(0)),
      cuenta_divisa(cuenta),
      factor_cambio(fact)
{
}

double CuentaDivisa::valor(const int t) const
{
    int t_actual = t + mes;
    return cuenta_divisa->valor(t_actual) * factor_cambio;
}

double CuentaDivisa::tae() const
{
    return cuenta_divisa->tae();
}
