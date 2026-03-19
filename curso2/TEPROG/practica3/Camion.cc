#include "Camion.h"

template <typename T>
Camion<T>::Camion(const std::string nom, const double vol, const double pes)
:Transporte(nom, vol, pes), volumen_actual(0){}

template <typename T> 
void Camion<T>::guardar(const Carga<T> elemento)
{
    if(this->volumen < volumen_actual + elemento->volumen )
    {
        this->peso += elemento->peso;
        this->volumen += elemento->volumen;
        elementos.push_back(elemento);
    }
}