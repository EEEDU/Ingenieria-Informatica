//*****************************************************************
// File:   MonitorDatos.hpp
// Author: Eduardo Guerrero (816106)
// Date:   noviembre 2024
// Coms:   Monitor para gestionar los datos práctica 4 de PSCD
//*****************************************************************

#include <mutex>
#include <condition_variable>
#include <cassert>
#include <fstream>

//----------------------------------------------------
using namespace std;

class MonitorDatos {
    public:
        //------------------------- constructores
        //Pre:  n>=0
        //Post: N_FIL=n
        //Coms: constructor con información para debugging
        //      Ejemplo: "MonitorDatos ms(2);"
        MonitorDatos(const int n);

        //------------------------- destructor
        ~MonitorDatos();

        /**
            * @brief El estudiante envia el numero maximo y avisa
            *
            * Almacena el numero maximo y avisa a procesos que esten esperando
            *
            * @param fila numero de fila al que pertenece el maximo
            * @param numMax numero maximo calculado por estudiante
        */
        void enviarMax(int fila, int numMax);

        /**
            * @brief El estudiante espera que se envie el numero maximo para cogerlo
            *
            * Espera a que la pareja envie el number maximo y lo coge
            *
            * @param fila numero de fila al que pertenece el maximo
            * @return el numero maximo
        */
        int cogerMax(int fila); 

        int N_FIL; 

    private:
        // Usamos punteros para arreglos dinámicos
        int* maximos;
        int* datosEnviados;

        mutex mtxMonitor; //FUNDAMENTAL: mutex usarán las funcs
        condition_variable cDatosEnviados;
};