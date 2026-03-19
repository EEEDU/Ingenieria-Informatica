#pragma once

#include "Carga.h"

#include <iostream>

template <typename T>
class Contenedor : public Carga
{
    protected:
        vector<Carga> elementos;
        double volumen_actual;

    public:
        Contenedor(const std::string nom, const double vol, const double pes);

        void guardar(const Carga<T>& elemento);
};