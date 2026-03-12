#pragma once

#include <iostream>
#include "Store.h"  

template <typename T>
class StoreLast : public Store<T>
{
    public:
        StoreLast();

        void push(const T& dato) override;
};

template <typename T>
StoreLast<T>::StoreLast()
    : Store<T>(T{}) {}

template <typename T>
void StoreLast<T>::push(const T& dato)
{
    this->dato_almacen = dato;
}