#pragma once

#include <vector>
#include <complex>
#include <iostream>
#include "Store.h"  

using namespace std;

template <typename T>
class StoreMax : public Store<T>
{
    private:
        bool inicializado; 
    public:
        StoreMax();

        void push(const T& dato) override;

};

template <typename T>
StoreMax<T>::StoreMax()
    : Store<T>(T{}), inicializado(false) {}




template <typename T>
void StoreMax<T>::push(const T& dato)
{
    if (!inicializado || magnitude(dato) > magnitude(this->dato_almacen)) {
        this->dato_almacen = dato;
        inicializado = true;
    }
}