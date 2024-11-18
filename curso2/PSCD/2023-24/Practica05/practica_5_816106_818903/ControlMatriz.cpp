//-----------------------------------------------------------------------------
// File:   ControlMatriz.cpp
// Author: Eduardo Guerrero (nip:816106) && Ismael Martinez (nip:818903)
// Date:   noviembre 2023
// Coms:   Parte del práctica 4 de PSCD
//          monitor encargado de la conciurrencia de la matriz
//------------------------------------------------------------------------------

#include "ControlMatriz.hpp"
#include <iostream>

ControlMatriz::ControlMatriz()
{
    for (int i = 0; i < 3; i++)
    {
        matrizResultados.push_back({0, 0, 0});
    }

    tiempoTotalExitosoT1 = 0;
    tiempoTotalExitosoT2 = 0;      
    tiempoTotalExitosoT3 = 0;      
}

void escribirMatriz(vector<vector<double>> &matrizResultados, Tarea tareaController, string resultado){
    if (tareaController.tipoTarea == "t1"){
        matrizResultados[0][0] ++;
        if (resultado == "OK"){
            matrizResultados[0][1] ++;
            matrizResultados[0][2] += tareaController.cargaDeTrabajo;
        } 
    }
    else if (tareaController.tipoTarea == "t2"){
        matrizResultados[1][0] ++;
        if (resultado == "OK"){
            matrizResultados[1][1] ++;
            matrizResultados[1][2] += tareaController.cargaDeTrabajo;
        } 
    }
    
    else if (tareaController.tipoTarea == "t3"){
        matrizResultados[2][0] ++;
        if (resultado == "OK"){
            matrizResultados[2][1] ++;
            matrizResultados[2][2] += tareaController.cargaDeTrabajo;
        } 
    }

}

void ControlMatriz::addResult(Tarea tarea, string resultado)
{
    escribirMatriz(matrizResultados, tarea, resultado);
}


void ControlMatriz::writeMatriz()
{
    for (int i = 0; i < matrizResultados.size(); i++){

        cout << "[" << endl;
        cout << "   Tipo de tarea t" << i+1 << endl;
        cout << "   Total: " << matrizResultados[i][0] << endl;
        cout << "   Exito: " << matrizResultados[i][1] << endl;
        cout << "   Tiempo: " << matrizResultados[i][2] << endl;
        cout << "]" << endl;
        
    }
}

