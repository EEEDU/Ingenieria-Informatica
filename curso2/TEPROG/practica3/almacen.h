#pragma once

#include <iostream>
#include <vector>

#include "carga.h"

using namespace std;

template <typename T>
class Almacen: virtual public Transporte
{
protected:
    double _capacidad;
    double _volumen_usado;
    vector<T*> _carga_almacenada;
    Almacen(const string nombre, const double capacidad);
public:
    ~Almacen() = default;

    bool guardar(T* carga);
    double getPeso() const override;
};

template <typename T>
Almacen<T>::Almacen(const string nombre, const double capacidad)
:Transporte(nombre, capacidad), _volumen_usado(0) {}

template <typename T>
bool Almacen<T>::guardar(T* carga)
{
    if (_volumen_usado + carga->getVolumen() <= _volumen)
    {
        _carga_almacenada.push_back(carga);
        _volumen_usado += carga->getVolumen();
        return true;
    }
    return false;
}

template <typename T>
double Almacen<T>::getPeso() const
{
    double peso = 0;
    for (const T* carga : _carga_almacenada) 
        peso += carga->getPeso();
    return peso;
}
