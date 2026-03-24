#pragma once

#include "carga.h"
#include "almacen.h"

#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class Contenedor : public Carga, public Almacen<T>
{
public:
    Contenedor(const double vol);
    void print(ostream& os) const override;
};


template<typename T>
Contenedor<T>::Contenedor(const double capacidad)
    :  Transporte("Contenedor", capacidad, 0.0), Carga("Contenedor", capacidad, 0.0), Almacen<T>("Contenedor", capacidad) {}

template<typename T>
void Contenedor<T>::print(ostream& os) const
{
    os << "Contenedor(vol=" << _volumen << ", peso=" << getPeso() << ")\n";
    for (const T* c : this->_carga_almacenada)
        os << "  " << *c << "\n";
}
