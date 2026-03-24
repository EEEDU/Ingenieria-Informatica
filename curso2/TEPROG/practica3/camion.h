#pragma once

#include "almacen.h"
#include "carga.h"

#include <iostream>
#include <vector>

using namespace std;

class Camion : public Almacen<Carga>
{
public:
    Camion(const double capacidad);
    void print(ostream& os) const override;
};

Camion::Camion(const double capacidad)
    : Transporte("Contenedor", capacidad), Almacen("Camion", capacidad) {}

void Camion::print(ostream& os) const
{
    os << "Camion(vol=" << _volumen << ", peso=" << getPeso() << ")\n";
    for (const Carga* c : _carga_almacenada)
        os << "  " << *c << "\n";
}