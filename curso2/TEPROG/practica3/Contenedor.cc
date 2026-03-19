#include "Contenedor.h"

template <typename T>
Contenedor<T>::Contenedor(const std::string nom, const double vol, const double pes)
:Carga(nom, vol, pes), volumen_actual(0) {}

template <typename T> 
void Contenedor<T>::guardar(const Carga<T> elemento)
{
    if(this->volumen < volumen_actual + elemento->volumen )
    {
        this->peso += elemento->peso;
        this->volumen += elemento->volumen;
        elementos.push_back(elemento);
    }
}