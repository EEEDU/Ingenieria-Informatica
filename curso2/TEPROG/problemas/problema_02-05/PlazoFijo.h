#ifndef PLAZO_FIJO_H
#define PLAZO_FIJO_H

#include <iostream>
#include "Cuenta.h"

class PlazoFijo : public Cuenta
{
    private:
        int plazo;
        double interes;

    public:
        PlazoFijo(double cap_ini, double ini, int pla);
        ~PlazoFijo();
        double valor(int t) const override;
};

#endif