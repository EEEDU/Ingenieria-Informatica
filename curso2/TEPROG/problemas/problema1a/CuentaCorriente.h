#ifndef CUENTA_CORRIENTE_H
#define CUENTA_CORRIENTE_H

#include <iostream>
#include "Cuenta.h"

class CuentaCorriente : public Cuenta
{
    public:
        CuentaCorriente(double cap_ini, double ini);
        ~CuentaCorriente();
        double valor(int t) const override;
};

#endif