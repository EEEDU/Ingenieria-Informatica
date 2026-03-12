#pragma once

#include <iostream>
#include <string>

#include "Store.h"  

using namespace std;

template <typename T>
class StoreMin : public Store<T>
{
    private:
        bool inicializado; 
    public:
        StoreMin();

        void push(const T& dato) override;
        string type() const override;

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

template <typename T>
string StoreMin<T>::type() const
{
    return "minimo";
}