//------------------------------------------------------------------------------
// File:   ControlParejas.hpp
// Author: Jesus Lizama (nip:816473) && Marcos Pérez (nip:820532)
// Date:   noviembre 2022
// Coms:   Parte del práctica 4 de PSCD
//          monitor encargado de la conciurrencia
//------------------------------------------------------------------------------

#include <mutex>
#include <condition_variable>

using namespace std;

// ----------------------------------------------------
const int N_EST = 60; //# de estudiantes
const int N_FIL = N_EST /2; //# de filas en la matriz
const int N_COL = 1000; //# de columnas

// ----------------------------------------------------
class ControlParejas
{
    private:
        mutex mutexMonitor;
        condition_variable c_sillaE, c_sillaP, /*c_esperarTenerCompanyero,*/ c_terminado;
        condition_variable c_esperarMaximo[N_FIL], c_esperarTenerCompanyero[N_EST];

        int n_sentados;
        int sentados[2];
        int companyeros[N_EST];
        int filas[N_EST];
        int maximos[N_FIL];
        bool encontrado[N_FIL];
        int terminado;
    public:
        ControlParejas();

        void sillaLibre(int nip);

        void hay2sentados(int i);

        void getDatosCompanyero(int nip, int& nipCompanyero, int& fila);

        void pasarMaximo(int fila, int max);

        void getMaximo(int fila, int& max);

        void parejaHaTerminado();

        void todosHanTermninado();
};