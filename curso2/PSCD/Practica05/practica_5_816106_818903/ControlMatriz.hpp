//-----------------------------------------------------------------------------
// File:   ControlMatriz.hpp
// Author: Eduardo Guerrero (nip:816106) && Ismael Martinez (nip:818903)
// Date:   noviembre 2023
// Coms:   Parte del práctica 4 de PSCD
//          monitor encargado de la conciurrencia de la matriz
//------------------------------------------------------------------------------

// ControlMatriz.hpp
#ifndef CONTROL_MATRIZ_HPP
#define CONTROL_MATRIZ_HPP

#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>

#include "Tarea.hpp"

using namespace std;

class ControlMatriz
{
private:
    vector<vector<double>> matrizResultados;

    double tiempoTotalExitosoT1;
    double tiempoTotalExitosoT2;
    double tiempoTotalExitosoT3;

public:
    ControlMatriz();

    string runTask(Tarea tarea);

    void addResult(Tarea tarea, string resultado);

    void writeMatriz();
};

#endif // CONTROL_MATRIZ_HPP