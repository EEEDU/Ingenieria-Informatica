//------------------------------------------------------------------------------
// File:   practica_3.cpp
// Author: Jesus Lizama (nip:816473) && Marcos Pérez (nip:820532)
// Date:   noviembre 2022
// Coms:   Parte del práctica 4 de PSCD
//         Compilar mediante
//           make -f Makefile_p4
//------------------------------------------------------------------------------

#include "ControlParejas.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <string.h>
#include <unistd.h>


using namespace std;

// ----------------------------------------------------
// Pre : <fila> es un  índice de fila de <D>
// Post : devuelve el mía ximo de la fila <fila >
int maxFila(int D[N_FIL][N_COL], int fila)
{
    int max = 0;
    for(unsigned int i=0; i<N_COL; i++)
    {
        if(D[fila][i] > max)
        {
            max = D[fila][i];
        }
    }
    return max;
}
// Pre : <fila > es un  índice de fila de <D>
// Post : devuelve la suma de los els . de la fila <fila >
int sumaFila(int D[N_FIL][N_COL], int fila)
{
    int sum = 0;
    for(unsigned int i=0; i<N_COL; i++)
    {
        sum = sum + D[fila][i]; 
    }
    return sum;
}

// ----------------------------------------------------
/*
 * La sbr entudiante realiza un examen con un compañero, el cual le asigna el profesor, si mi nip es menor que el de mi compañero
 * me encargare de buscar el maximo de una fila de una matriz, si el nip es mayor tendre que calcular la suma de la fila.
 * Una vez los dos han complatado la tarea se lo hacen saber al profesor.
*/
void Estudiante(int nip, int matriz[N_FIL][N_COL], ControlParejas& monitor)
{
    int miPareja;
    int mi_fila;
    int max;


    //entre 50 y 100 milisegundos.
    //sleep();
    int num = 0;
    srand(time(NULL));
    num = rand();
    //entre 50 y 100
    num=50+rand()%(101-50);
    usleep(num);
    // esperar por una silla libre
    //< await n_sentados < 2
    //    sentado[n_sentados] = nip; 
    //    sentados++;
    //>
    monitor.sillaLibre(nip);

    // esperar me sea asignada pareja y fila
    //< await companyero[nip] != -1
    //    mi_fila = fila[nip];
    //>
    //busco el nip de mi compañero
    monitor.getDatosCompanyero(nip,miPareja,mi_fila);

    if (nip < miPareja) 
    {
        //calcular maximo de la fila
        max = maxFila(matriz,mi_fila);

        // calcular el maximo e indicar a mi pareja que ya lo tengo
        //< 
        // mximos[mi_fila] = max;
        // encontrado[mi_fila] = true; 
        //>
        int num = 0;
        srand(time(NULL));
        num = rand();
        //entre 50 y 100
        num=50+rand()%(101-50);
        usleep(num);
        monitor.pasarMaximo(mi_fila,max);
    }
    else
    {
        // calcular la suma de mi fila
        int suma = sumaFila(matriz,mi_fila);

        // coger info de max (de mi pareja)
        //< await encontrado[mi_fila] == true
        //    int max = maximos[mi_fila];
        //>
        monitor.getMaximo(mi_fila,max);

        // mostrar resultados
        cout << to_string(mi_fila) + "\t| " + to_string(miPareja) + "-" + to_string(nip) + "\t| " + to_string(max) + "\t| " + to_string(suma) + "\n";

        // comunicar finalizacion
        //< termine++; >
        monitor.parejaHaTerminado();
    }
}

// ----------------------------------------------------
/*
 * La sbr profesor se encarga de indicar los nips de las personas que se sientan en las sillas.
 * luego espera a que terminen todas las parejas para poder dar por finalizado el examen. 
*/
void Profesor (ControlParejas& monitor) 
{
    for(int i=0; i< N_FIL; i++)
    {
        // esperar a que haya dos
        //< await n_sentados == 2
        // comunicar a cada uno su pareja , y la fila que les toca
        //con esto l que estamos haciendo es, meter en el vectord e compañeros el nip de su otro compañero.
        //    companyeros[sentados[0]] = sentados[1];
        //    companyeros[sentados[1]] = sentados[0];
        //    //aqui lo que estamos haciendo es meter en las posiciones correspondeientes a los nips de la pareja el numero de fila a leer.
        //    filasPareja[sentados[0]] = i;
        //    filasPareja[sentados[1]] = i;
        //    n_sentados = 0;
        //>
        monitor.hay2sentados(i);
    }

    // esperar que todos hayan terminado
    //< await termine == N_FIL 
    //    mostrar mensaje de fin
    //>
    monitor.todosHanTermninado();
}

// ----------------------------------------------------
int main()
{
    //inicializar hilos y monitor
    thread P[N_EST];
    int matriz[N_FIL][N_COL];
    //clase monitor
    ControlParejas monitor;

    //cargar la matriz
    ifstream fich("datos.txt");
    if (fich.is_open())
    {
        for(unsigned i=0; i<N_FIL; i++)
        {
            for(unsigned int j=0; j<N_COL; j++)
            {
                fich >> matriz[i][j];
            }
        }
    }
    else
    {
        cerr << "No se pudo abrir el archivo \"datos.txt\".\n" << endl;

        return 1;
    }

    //inicializar threads
    for (int i=0; i<N_EST; i++){
        P[i] = thread(&Estudiante, i, matriz, ref(monitor));
    }

    thread th_prof(&Profesor,ref(monitor));

    //esperar que terminen los thread
    th_prof.join();

    for (int i=0; i<N_EST; i++) {
        P[i].join();
    }

    cout << "\nPrueba finalizada\n";

    return 0;
}