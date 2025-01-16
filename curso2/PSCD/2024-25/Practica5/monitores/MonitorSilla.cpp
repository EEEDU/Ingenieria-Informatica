//*****************************************************************
// File:   MonitorSilla.cpp
// Author: 816106
// Date:   noviembre 2024
// Coms:   Ver MonitorSilla.hpp
//*****************************************************************

#include <MonitorSilla.hpp>


//----------------------------------------------------------
MonitorSilla::MonitorSilla(const int n)
    : N_EST(n) {
    // Asignar memoria dinámica para los arreglos
    parejaAsignada = new int[N_EST];
    filaAsignada = new int[N_EST];
    tienePareja = new bool[N_EST];

    // Inicializar los arreglos
    for (int i = 0; i < N_EST; ++i) {
        parejaAsignada[i] = -1;  // Inicialización con -1
        filaAsignada[i] = -1;    // Inicialización con -1
        tienePareja[i] = false;  // Inicialización con false
    }
}

//----------------------------------------------------------
MonitorSilla::~MonitorSilla() {
    // Liberar la memoria asignada dinámicamente
    delete[] parejaAsignada;
    delete[] filaAsignada;
    delete[] tienePareja;
}

//----------------------------------------------------------
void MonitorSilla::sentarse(int nip) {
    unique_lock<mutex> lck(mtxMonitor);
    // Mientras la silla sea 2, esperar silla libre
    while (iSilla == 2)
    {
        cSillaLibre.wait(lck);
    }
    // Si hay una libre (iSilla < 2), sentarse en la silla
    nipSilla[iSilla] = nip;
    iSilla++;
    // Si ahora hay dos sentados, desbloquear para que se puedan asignar parejas y filas
    if (iSilla == 2) {
        cSillasOcupadas.notify_one();
    }
}

//----------------------------------------------------------
tuple<int, int> MonitorSilla::levantarse(int nip) {
    unique_lock<mutex> lck(mtxMonitor);
    // Esperar a que se levante el estudiante
    while (!tienePareja[nip])
    {
        cInformacionAsignada.wait(lck);
    }
    // Devolver la fila y la pareja
    return make_tuple(filaAsignada[nip], parejaAsignada[nip]);
}

//----------------------------------------------------------
void MonitorSilla::asignarInfo(int fila) {
    unique_lock<mutex> lck(mtxMonitor);
    // Esperar que haya dos sentados
    while (iSilla < 2)
    {
        cSillasOcupadas.wait(lck);
    }
    // Asignar la fila y la pareja
    filaAsignada[nipSilla[0]] = filaAsignada[nipSilla[1]] = fila;
    parejaAsignada[nipSilla[0]] = nipSilla[1];
    parejaAsignada[nipSilla[1]] = nipSilla[0];
    // Marcar que los estudiantes tienen pareja
    tienePareja[nipSilla[0]] = tienePareja[nipSilla[1]] = true;
    // Reiniciar el contador de sillas
    iSilla = 0;  
    // Desbloquear para que los estudiantes se puedan sentar
    cSillaLibre.notify_all();
    // Desbloquear para que los estudiantes asignados continuen
    cInformacionAsignada.notify_all();
}
