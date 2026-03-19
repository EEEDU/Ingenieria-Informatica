#pragma once

#include "Transporte.h"
#include "Carga.h"

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Camion : public Transporte<T>
{
    protected:
        vector<Cargar> elementos;
        double volumen_actual;

    public:
        Camion(const std::string nom, const double vol, const double pes);

        void guardar(const Carga<T> elemento) override;
}