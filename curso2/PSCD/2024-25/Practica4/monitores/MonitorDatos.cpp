//*****************************************************************
// File:   MonitorDatos.cpp
// Author: 816106
// Date:   noviembre 2024
// Coms:   Ver MonitorDatos.hpp
//*****************************************************************

#include <MonitorDatos.hpp>


//----------------------------------------------------------
MonitorDatos::MonitorDatos(const int n)
    : N_FIL(n) {
    // Asignar memoria dinámica para los arreglos
    maximos = new int[N_FIL];
    datosEnviados = new int[N_FIL];

    // Inicializar los arreglos
    for (int i = 0; i < N_FIL; ++i) {
        maximos[i] = -1;  // Inicialización con -1
        datosEnviados[i] = false;  // Inicialización con false
    }
}

//----------------------------------------------------------
MonitorDatos::~MonitorDatos() {
    // Liberar la memoria asignada dinámicamente
    delete[] maximos;
    delete[] datosEnviados;
}

//----------------------------------------------------------
void MonitorDatos::enviarMax(int fila, int numMax) {
    unique_lock<mutex> lck(mtxMonitor);
    // Agrega el numero maximo al array y marcarlo en el vector de bool
    maximos[fila] = numMax;
    datosEnviados[fila] = true;
    // Avisar de que se ha guardado el numero maximo
    cDatosEnviados.notify_all();
}

//----------------------------------------------------------
int MonitorDatos::cogerMax(int fila) {
    unique_lock<mutex> lck(mtxMonitor);
    // Esperar a que se envie el numero maximo
    while (!datosEnviados[fila])
    {
        cDatosEnviados.wait(lck);
    }
    // Devolver el numero maximo
    return maximos[fila];
}