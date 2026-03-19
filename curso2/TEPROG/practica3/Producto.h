#pragma once

#include "Carga.h"

#include <iostream>

template <typename T>
class Producto : public Carga
{
    protected:

    public:
        Producto(const std::string nom, const double vol, const double pes);
}