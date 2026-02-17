#ifndef CUENTA_DIVISA_H
#define CUENTA_DIVISA_H

#include <iostream>
#include "Cuenta.h"

class CuentaDivisa : public Cuenta
{
    private:
        Cuenta* cuenta_divisa;
        double factor_cambio;
    
    public:
        CuentaDivisa(Cuenta* cuenta, double fact);
        double valor(const int t) const override;
        double tae() const override;
        ~CuentaDivisa();
};

#endif