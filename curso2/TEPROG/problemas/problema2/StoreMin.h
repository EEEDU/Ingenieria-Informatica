#pragma once

#include <iostream>
#include "Store.h"  

template <typename T>
class StoreMin : public Store<T>
{
    private:
        bool inicializado; 
    public:
        StoreMin();

        void push(const T& dato) override;
};


template <typename T>
StoreMin<T>::StoreMin()
    : Store<T>(T{}), inicializado(false) {}

template <typename T>
void StoreMin<T>::push(const T& dato)
{
    if (!inicializado || magnitude(dato) < magnitude(this->dato_almacen)) {
        this->dato_almacen = dato;
        inicializado = true;
    }
}