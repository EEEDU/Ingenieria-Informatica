#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
class Store
{
    protected:
        T dato_almacen;
        
    public:
        Store(T dato);
        virtual ~Store() = default;
        
        virtual void push(const T& dato) = 0;
        virtual void push(const std::vector<T>& datos);
        virtual T value() const;
        virtual string type() const = 0;
        // std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);
        // virtual std::string nombre() const = 0;  // para identificar el almacen en el log

};

template <typename T>
Store<T>::Store(T dato)
    : dato_almacen(dato) {}

template <typename T>
void Store<T>::push(const std::vector<T>& datos)
{
    for(const T& dato : datos)
    {
        push(dato);
    }
}

template <typename T>
T Store<T>::value() const
{
    return dato_almacen;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (const auto& dato : v)
        os << dato << " ";
    os << "]";
    return os;
}

double magnitude(const vector<double>& dato)
{
    double suma = 0.0;
    for (double x : dato)
        suma += x;

    return suma;
}

template<typename T>
auto magnitude(const T& dato)
{
    using std::abs;
    return abs(dato);
}