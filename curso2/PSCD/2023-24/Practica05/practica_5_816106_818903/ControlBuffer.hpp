//-----------------------------------------------------------------------------
// File:   ControlBuffer.hpp
// Author: Eduardo Guerrero (nip:816106) && Ismael Martinez (nip:818903)
// Date:   noviembre 2023
// Coms:   Parte del práctica 4 de PSCD
//          monitor encargado de la conciurrencia del buffer
//------------------------------------------------------------------------------

// ControlBuffer.hpp
#ifndef CONTROL_BUFFER_HPP
#define CONTROL_BUFFER_HPP

#include <mutex>
#include <condition_variable>

#include "librerias/MultiBuffer/MultiBuffer.hpp"
#include "Tarea.hpp"

using namespace std;

const int N_CONTROLLERS = 10;

class ControlBuffer
{
private:
    mutex mutexMonitor;
    condition_variable bufferVacio, bufferLleno;
    

    MultiBuffer<Tarea, N_CONTROLLERS> mBT; // multi-buffer de tareas

public:
    ControlBuffer();

    void addTask(Tarea tarea);

    Tarea getTask();

    bool checkTf();
};

#endif // CONTROL_BUFFER_HPP