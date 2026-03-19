#pragma once

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class Transporte
{
    protected:
        const std::string nombre;
        const double volumen;
        double peso;

    public:
        Transporte(const std::string nom, const double vol, const double pes);
        ~Transporte() = default;

        ostream& operator<<(ostream& os, T elemento);
};
