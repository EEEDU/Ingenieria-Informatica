#ifndef CUENTA_H
#define CUENTA_H

#include <iostream>

class Cuenta
{
    protected:
        double capital_inicial;
        int mes;

    public:
        Cuenta(double cap_ini);
        ~Cuenta();
        virtual double valor(int t) const =0;
        virtual double tae() const;
        void actualizar();
};

#endif