#include "Carga.h"

template <typename T>
Carga<T>::Carga(const std::string nom, const double vol, const double pes)
:Transporte(nom, vol, pes) {}