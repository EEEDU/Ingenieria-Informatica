#pragma once

#include <iostream>
#include <string>

#include "Store.h"  

using namespace std;

template <typename T>
class StoreAvg : public Store<T>
{
    private:
        int cantidad_datos;
        T suma;
    public:
        StoreAvg();

        void push(const T& dato) override;
        T value() const override;
        string type() const override;

};

template <typename T>
StoreAvg<T>::StoreAvg()
    : Store<T>(T {}), cantidad_datos(0), suma(T{}) {}

template <>
void StoreAvg<vector<double>>::push(const vector<double>& dato) 
{
    if (suma.empty())
        suma = dato;
    else
        for (size_t i = 0; i < dato.size(); i++)
            suma[i] += dato[i];

    cantidad_datos++;
}

// push solo acumula
template <typename T>
void StoreAvg<T>::push(const T& dato)
{
    suma += dato;
    cantidad_datos++;
}

template <>
vector<double> StoreAvg<vector<double>>::value() const 
{
    if (cantidad_datos == 0) return {};

    std::vector<double> media = suma;
    for (double &x : media)
        x /= cantidad_datos;

    return media;
}

// value() calcula la media al final
template <typename T>
T StoreAvg<T>::value() const
{
    if (cantidad_datos == 0) return T{};
    return suma / static_cast<T>(cantidad_datos);
}

template <typename T>
string StoreAvg<T>::type() const
{
    return "average";
}