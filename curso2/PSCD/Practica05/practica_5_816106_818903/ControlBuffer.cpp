//-----------------------------------------------------------------------------
// File:   ControlBuffer.cpp
// Author: Eduardo Guerrero (nip:816106) && Ismael Martinez (nip:818903)
// Date:   noviembre 2023
// Coms:   Parte del práctica 4 de PSCD
//          monitor encargado de la conciurrencia del Buffer
//------------------------------------------------------------------------------

#include "ControlBuffer.hpp"
#include <iostream>

int buscarBufferLibre(MultiBuffer<Tarea, N_CONTROLLERS> mBT){
    int i=0;
    while(i < mBT.size() && mBT.isNew(i) == 1){
        i++;
    }
    return i;
}

int buscarBufferLleno(MultiBuffer<Tarea, N_CONTROLLERS> mBT){
    int i=0;
    while(i < mBT.size() && mBT.isNew(i) == 0){
        i++;
    }
    return i;
}


ControlBuffer::ControlBuffer()
{
    
}

void ControlBuffer::addTask(Tarea tarea)
{
    unique_lock<mutex> lck(mutexMonitor);
    
    while (mBT.numberNews() >= N_CONTROLLERS)
    {
        bufferVacio.wait(lck);
    }
    
    mBT.put(tarea, buscarBufferLibre(mBT));
    bufferLleno.notify_one();
}

Tarea ControlBuffer::getTask(){
    unique_lock<mutex> lck(mutexMonitor);
    
    while (mBT.numberNews() <= 0)
    {
        bufferLleno.wait(lck);
    }
    
    Tarea tareaController = mBT.get(buscarBufferLleno(mBT));
    bufferVacio.notify_one();

    return tareaController;
}

bool ControlBuffer::checkTf(){
    int contador = 0;
    for (int i = 0; i < mBT.size(); i++){
        if (mBT.watch(i).tipoTarea == "TF") contador ++;
    }
    if (contador == mBT.size()) return true;
    return false;
}



