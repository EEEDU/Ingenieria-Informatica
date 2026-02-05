#ifndef CUENTA_H
#define CUENTA_H

#include <iostream>

class Cuenta
{
    protected:
        double capital_inicial;
        double interes;

    public:
        Cuenta(double cap_ini, double ini);
        ~Cuenta();
        virtual double valor(int t) const =0;
        double tae();
};

#endif