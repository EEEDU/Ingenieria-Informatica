#ifndef CUENTA_NOMINA_H
#define CUENTA_NOMINA_H

#include <iostream>
#include "Cuenta.h"

class CuentaNomina : public Cuenta
{
    private:
        double nomina;

    public:
        CuentaNomina(double cap_ini, double nom);
        ~CuentaNomina();
        double valor(const int t) const override;
};

#endif