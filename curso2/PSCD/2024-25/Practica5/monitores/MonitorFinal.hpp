//*****************************************************************
// File:   MonitorFinal.hpp
// Author: Eduardo Guerrero (816106)
// Date:   noviembre 2024
// Coms:   Monitor para gestionar el final del estudiante práctica 4 de PSCD
//*****************************************************************

#include <mutex>
#include <condition_variable>
#include <cassert>
#include <fstream>

//----------------------------------------------------
using namespace std;

class MonitorFinal {
    public:
        //------------------------- constructores
        //Pre:  n>=0
        //Post: N_EST=n
        //Coms: constructor con información para debugging
        //      Ejemplo: "MonitorFinal ms(2);"
        MonitorFinal(const int n);

        //------------------------- destructor
        ~MonitorFinal();

        /**
            * @brief El estudiante avisa de que ha terminado
        */
        void avisarFin();

        /**
            * @brief Espera que todos los estudiantes hayan termiando
        */
        void esperarFin(); 


    private:
        int N_EST; 
        int estudianteTerminado;

        mutex mtxMonitor; //FUNDAMENTAL: mutex usarán las funcs
        condition_variable cFinal;
};