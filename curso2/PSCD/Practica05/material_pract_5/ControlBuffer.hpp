//-----------------------------------------------------------------------------
// File:   ControlBuffer.hpp
// Author: Eduardo Guerrero (nip:816106) && Ismael Martinez (nip:818903)
// Date:   noviembre 2022
// Coms:   Parte del práctica 5 de PSCD
//          monitor encargado de la conciurrencia
//------------------------------------------------------------------------------

#include <mutex>
#include <condition_variable>


#include "librerias/MultiBuffer/MultiBuffer.hpp"

using namespace std;

const int N_CONTROLLERS = 10;

struct Tarea
{
    string tipoTarea;
    float cargaDeTrabajo;
};

class ControlBuffer 
{
    private:
    mutex mutexMonitor;
    condition_variable bufferVacio, bufferLleno;

    double tiempoTotalExitosoT1;
    double tiempoTotalExitosoT2;
    double tiempoTotalExitosoT3;


    public:
    ControlBuffer();
    
    // QUE COJONES HAGO CON EL BUFFER
    void addTask(Tarea tarea, MultiBuffer<Tarea, N_CONTROLLERS> &mBT);

    Tarea getTask(MultiBuffer<Tarea, N_CONTROLLERS> &mBT);
};