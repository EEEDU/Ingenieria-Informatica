//*****************************************************************
// File:   MonitorFinal.cpp
// Author: 816106
// Date:   noviembre 2024
// Coms:   Ver MonitorFinal.hpp
//*****************************************************************

#include <MonitorFinal.hpp>


//----------------------------------------------------------
MonitorFinal::MonitorFinal(const int n) : N_EST(n) {}

//----------------------------------------------------------
MonitorFinal::~MonitorFinal() {}

//----------------------------------------------------------
void MonitorFinal::avisarFin() {
    unique_lock<mutex> lck(mtxMonitor);
    // Aumenta en 1 el numero de estudiantes terminados
    estudianteTerminado++;
    // Avisar de que se ha terminado un estudiante
    cFinal.notify_one();
}

//----------------------------------------------------------
void MonitorFinal::esperarFin() {
    unique_lock<mutex> lck(mtxMonitor);
    // Esperar a que se envie el numero maximo
    while (estudianteTerminado < N_EST)
    {
        cFinal.wait(lck);
    }
}