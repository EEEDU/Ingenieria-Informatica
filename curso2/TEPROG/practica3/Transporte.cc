#include "Transporte.h"

using namespace std;

template <typename T>
Transporte<T>::Transporte(const std::string nom, const double vol, const double pes)
:nombre(nom), volumen(vol), peso(pes) {}

template <typename T>
ostream& operator<<(ostream& os, T elemento) {
    os << p.nombre << ":\n\tpeso:" << p.peso << "\n\tvolumen:" << p.volumen;
    return os;
}