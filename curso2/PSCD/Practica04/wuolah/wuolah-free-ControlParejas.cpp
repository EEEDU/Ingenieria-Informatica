//------------------------------------------------------------------------------
// File:   ControlParejas.cpp
// Author: Jesus Lizama (nip:816473) && Marcos Pérez (nip:820532)
// Date:   noviembre 2022
// Coms:   Parte del práctica 4 de PSCD
//------------------------------------------------------------------------------

#include "ControlParejas.hpp"
#include <iostream>

ControlParejas::ControlParejas()
{
    n_sentados = 0;

    terminado = 0;

    for(unsigned int i=0; i<2; i++)
    {
        sentados[i] = -1;
    }

    for(unsigned int i=0; i<N_EST; i++)
    {
        companyeros[i] = -1;
        filas[i] = -1;
    }

    for(unsigned int i=0; i<N_FIL; i++)
    {
        maximos[i] = -1;
        encontrado[i] = false;
    }
}; 

void ControlParejas::sillaLibre(int nip)
{
    unique_lock<mutex> lck(mutexMonitor);

    while (n_sentados >= 2)
    {
        //cout << to_string(nip) + " aka esperando de chill\n";
        c_sillaE.wait(lck);
    }
    sentados[n_sentados] = nip;
    n_sentados++;

    c_sillaP.notify_one();
};

void ControlParejas::hay2sentados(int i)
{
    unique_lock<mutex> lck(mutexMonitor);

    while(n_sentados < 2)
    {
        c_sillaP.wait(lck);
    }
    //cout << to_string(n_sentados) + " se han sentado\n";

    companyeros[sentados[0]] = sentados[1];
    companyeros[sentados[1]] = sentados[0];
    filas[sentados[0]] = i;
    filas[sentados[1]] = i;
    n_sentados = 0;

    c_sillaE.notify_one();
    c_sillaE.notify_one();
    c_esperarTenerCompanyero.notify_all();
};

void ControlParejas::getDatosCompanyero(int nip, int& nipCompanyero, int& fila)
{
    unique_lock<mutex> lck(mutexMonitor);

    while(companyeros[nip] == -1)
    {
        //cout << to_string(nip) + " no me quieren\n";
        c_esperarTenerCompanyero.wait(lck);
    }

    nipCompanyero = companyeros[nip];
    fila = filas[nip]; 
};

void ControlParejas::pasarMaximo(int fila, int max)
{
    unique_lock<mutex> lck(mutexMonitor);

    maximos[fila] = max;

    encontrado[fila] = true;


    //cambiarlo poe un vector de condiciones y despertar solo la posicion exacta.
    //c_esperarMaximo.notify_all();
    c_esperarMaximo[fila].notify_one();
};

void ControlParejas::getMaximo(int fila, int& max)
{
    unique_lock<mutex> lck(mutexMonitor);

    while(encontrado[fila] == false)
    {
        c_esperarMaximo[fila].wait(lck);
    }

    max = maximos[fila];
};

void ControlParejas::parejaHaTerminado()
{
    unique_lock<mutex> lck(mutexMonitor);

    terminado++;

    c_terminado.notify_one();
};

void ControlParejas::todosHanTermninado()
{
    unique_lock<mutex> lck(mutexMonitor);

    while(terminado < N_FIL)
    {
        c_terminado.wait(lck);
    }
};