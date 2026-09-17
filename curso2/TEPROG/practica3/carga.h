#pragma once

#include "transporte.h"

#include <iostream>

using namespace std;

class Carga : virtual public Transporte
{
public:
    Carga(const string nombre, const double volumen, const double peso);
    ~Carga() = default;

    // virtual void print(ostream &os) const = 0;
};

Carga::Carga(const string nombre, const double volumen, const double peso)
: Transporte(nombre, volumen, peso) {};


// ostream &operator<<(ostream &os, const Carga &carga)
// {
//     carga.print(os);
//     return os;
// }