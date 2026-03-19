#pragma once

#include "Transporte.h"
#include <iostream>

template <typename T>
class Carga : public Transporte<T>
{      
    public:

    protected:
        Carga(const std::string nom, const double vol, const double pes);
};