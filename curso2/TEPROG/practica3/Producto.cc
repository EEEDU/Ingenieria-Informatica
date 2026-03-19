#include "Producto.h"

template <typename T>
Producto<T>::Producto(const std::string nom, const double vol, const double pes)
:Carga(nom, vol, pes) {}s