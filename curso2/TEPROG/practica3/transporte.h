#pragma once

#include <iostream>
#include <string>

using namespace std;

class Transporte
{
    protected:
        const std::string _nombre;
        const double _volumen;
        double _peso;

    public:
        Transporte(const std::string nombre, const double volumen);
        Transporte(const std::string nombre, const double volumen, const double peso);
        ~Transporte() = default;

        string getNombre() const;
        double getVolumen() const;
        virtual double getPeso() const;

        virtual void print(ostream& os) const = 0;
        friend ostream& operator<<(ostream& os, const Transporte& t);
};

Transporte::Transporte(const std::string nombre, const double volumen)
: _nombre(nombre), _volumen(volumen) {}

Transporte::Transporte(const std::string nombre, const double volumen, const double peso)
: _nombre(nombre), _volumen(volumen), _peso(peso) {}

string Transporte::getNombre() const 
{
    return _nombre;
}

double Transporte::getVolumen() const
{
    return _volumen;
}

double Transporte::getPeso() const
{
    return _peso;
}

ostream& operator<<(ostream& os, const Transporte& t) {
    t.print(os);
    return os;
}
