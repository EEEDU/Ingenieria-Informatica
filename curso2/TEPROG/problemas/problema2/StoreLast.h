#pragma once

#include <iostream>
#include <string>
    
#include "Store.h"  

using namespace std;

template <typename T>
class StoreLast : public Store<T>
{
    public:
        StoreLast();

        void push(const T& dato) override;
        string type() const override;
};

template <typename T>
StoreLast<T>::StoreLast()
    : Store<T>(T{}) {}

template <typename T>
void StoreLast<T>::push(const T& dato)
{
    this->dato_almacen = dato;
}

template <typename T>
string StoreLast<T>::type() const
{
    return "last";
}
