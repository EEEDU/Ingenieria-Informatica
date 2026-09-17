#pragma once

#include "carga.h"

#include <iostream>

using namespace std;

class Producto : public Carga
{
public:
    Producto(const string nom, const double vol, const double pes);
    void print(ostream &os) const override;
};

Producto::Producto(const string nombre, const double volumen, const double peso)
    : Transporte(nombre, volumen, peso), Carga(nombre, volumen, peso) {}

void Producto::print(ostream &os) const
{
    os << "Producto(" << _nombre
       << ", vol=" << _volumen
       << ", peso=" << _peso << ")";
}