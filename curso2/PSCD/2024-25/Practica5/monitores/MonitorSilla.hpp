//*****************************************************************
// File:   MonitorSilla.hpp
// Author: Eduardo Guerrero (816106)
// Date:   noviembre 2024
// Coms:   Monitor para gestionar las sillas práctica 4 de PSCD
//*****************************************************************

#include <mutex>
#include <condition_variable>
#include <cassert>
#include <fstream>
#include <tuple>

//----------------------------------------------------
using namespace std;

class MonitorSilla {
    public:
        //------------------------- constructores
        //Pre:  n>=0
        //Post: N_EST=n
        //Coms: constructor con información para debugging
        //      Ejemplo: "MonitorSilla ms(2);"
        MonitorSilla(const int n);

        //------------------------- destructor
        ~MonitorSilla();

        /**
            * @brief El estudiante espera una silla libre y se sienta en una de las sillas
            *
            * En caso de que las sillas esten ocupadas, espera a que se libere una.
            * Cuando se sienta si ya hay dos personas sentadas, llama para que se asigne una pareja y una fila
            *
            * @param nip Identificador del estudiante
        */
        void sentarse(int nip);

        /**
            * @brief El estudiante espera que se le asigne pareja y estudiante
            *
            * Espera a que se le asigne una pareja y una fila.
            *
            * @param nip Identificador del estudiante
            * @return <fila, pareja>
        */
        tuple<int, int> levantarse(int nip); 

        /**
            * @brief El profesor espera que haya dos estudiantes sentados para asignar una pareja y una fila
            *
            * En caso de que las sillas esten vacias, espera a que se ocupen.
            * Cuando estan las dos ocupadas, asigna la pareja y la fila a los ocupantes de las sillas.
            *
            * @param fila Fila correspondiente a la pareja
        */
        void asignarInfo(int fila);


        int N_EST; 

    private:
        int nipSilla[2];
        int iSilla;

        // Usamos punteros para arreglos dinámicos
        int* parejaAsignada;
        int* filaAsignada;
        bool* tienePareja;

        mutex mtxMonitor; //FUNDAMENTAL: mutex usarán las funcs
        condition_variable cSillaLibre, cSillasOcupadas, cInformacionAsignada;
};