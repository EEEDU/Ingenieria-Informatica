º//------------------------------------------------------------------------------
// File:   practica_3.cpp
// Author: Jesus Lizama (nip:816473) && Marcos Pérez (nip:820532)
// Date:   noviembre 2022
// Coms:   Parte del práctica 3 de PSCD
//         Compilar mediante
//           make -f Makefile_p3
//------------------------------------------------------------------------------

#include "librerias/Semaphore_V4/Semaphore_V4.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <string.h>

using namespace std;

// ----------------------------------------------------
const int N_EST = 60; //# de estudiantes
const int N_FIL = N_EST /2; //# de filas en la matriz
const int N_COL = 1000; //# de columnas

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
//Funcion de pasar testigo
void pasarTestigo(Semaphore& testigo, Semaphore& s_silla, int& c_silla, int& n_sentados, Semaphore* s_asignadas[], int c_asignadas[], int companyero[], 
    Semaphore* s_espera[], int c_espera[], bool encontrado[], Semaphore& s_sillaP,int& c_sillaP,Semaphore& s_termina,int& c_termina,int& terminado)
{
    bool testigoPasado = false;
    unsigned int i = 0;

    while(i<N_EST && testigoPasado == false)
    {
        if((companyero[i] != -1) && (c_asignadas[i] > 0))
        {
            c_asignadas[i]--;
            s_asignadas[i]->signal();  

            testigoPasado = true;
        }

        i++;
                   }

    i = 0;

    while(i<N_FIL && testigoPasado == false)
    {
        if((encontrado[i] == true) && (c_espera[i] > 0))
        {
            c_espera[i]--;
            s_espera[i]->signal();

            return;
        }

        i++;
    }
    
    if(testigoPasado == false)
    {
        if ((n_sentados < 2) && (c_silla > 0))
        {
            c_silla--;
            s_silla.signal();
        }
        else if((n_sentados == 2) && (c_sillaP > 0))
        {
            c_sillaP--;
            s_sillaP.signal();
        }
        else if((terminado == N_FIL) && (c_termina > 0))
        {
            c_termina--;
            s_termina.signal();
        }
        else
        {
            testigo.signal();
        }
    }
}

// ----------------------------------------------------
/*
 * La sbr entudiante realiza un examen con un compañero, el cual le asigna el profesor, si mi nip es menor que el de mi compañero
 *  me encargare de buscar el maximo de una fila de una matriz, si el nip es mayor tendre que calcular la suma de la fila.
 * Una vez los dos han complatado la tarea se lo hacen saber al profesor.
*/
void Estudiante(int nip, int matriz[N_FIL][N_COL], int sentados[],int companyeros[], int maximos[], bool encontrado[], int filas[], 
    Semaphore& testigo, Semaphore& s_silla, int& c_silla, int& n_sentados, Semaphore* s_asignadas[], int c_asignadas[],Semaphore* s_espera[], int c_espera[], 
    Semaphore& s_sillaP,int& c_sillaP,Semaphore& s_termina,int& c_termina,int& terminado)
{
    int mi_fila;
    int max;

    // esperar por una silla libre
    //< await n_sentados < 2
    //    sentado[n_sentados] = nip; 
    //    sentados++;
    //>
    //B = n_sentados < 2
    //b = s_silla
    //d = c_silla
    testigo.wait();
    if(n_sentados >= 2)
    {
        c_silla++;
        testigo.signal();
        s_silla.wait();
    }
    sentados[n_sentados] = nip;
    n_sentados++;
    pasarTestigo(testigo,s_silla,c_silla,n_sentados,s_asignadas,c_asignadas,companyeros,s_espera,c_espera,encontrado,s_sillaP,c_sillaP, s_termina, c_termina,terminado);

    // esperar me sea asignada pareja y fila
    //< await companyero[nip] != -1
    //    mi_fila = fila[nip];
    //>
    //B = companyero[nip] != -1
    //b = s_asignadas
    //d = c_asignadas

    testigo.wait();
    if(companyeros[nip] == -1)
    {
        c_asignadas[nip]++;
        testigo.signal();
        s_asignadas[nip]->wait();
    }
    mi_fila = filas[nip];
    pasarTestigo(testigo,s_silla,c_silla,n_sentados,s_asignadas,c_asignadas,companyeros,s_espera,c_espera,encontrado,s_sillaP,c_sillaP, s_termina, c_termina,terminado);
    //busco el nip de mi compañero
    int miPareja = companyeros[nip];

    if (nip < miPareja) 
    {
        // calcular el maximo e indicar a mi pareja que ya lo tengo
        //< 
        // mximos[mi_fila] = maxFila();
        // encontrado[mi_fila] = true; 
        //>
        testigo.wait();
        maximos[mi_fila] = maxFila(matriz,mi_fila);
        encontrado[mi_fila] = true;
        pasarTestigo(testigo,s_silla,c_silla,n_sentados,s_asignadas,c_asignadas,companyeros,s_espera,c_espera,encontrado,s_sillaP,c_sillaP, s_termina, c_termina,terminado);
    }
    else
    {
        // calcular la suma de mi fila
        int suma = sumaFila(matriz,mi_fila);

        // coger info de max (de mi pareja)
        //< await encontrado[mi_fila] == true
        //    int max = maximos[mi_fila];
        //>
        //B = encontrado[mi_fila] == true
        //b = s_espera
        //d = c_espera
        
        testigo.wait();
        if(encontrado[mi_fila] != true)
        {
            c_espera[mi_fila]++;
            testigo.signal();
            s_espera[mi_fila]->wait();
        }
        max = maximos[mi_fila];
        pasarTestigo(testigo,s_silla,c_silla,n_sentados,s_asignadas,c_asignadas,companyeros,s_espera,c_espera,encontrado,s_sillaP,c_sillaP, s_termina, c_termina,terminado);

        // mostrar resultados
        cout << to_string(mi_fila) + "\t| " + to_string(miPareja) + "-" + to_string(nip) + "\t| " + to_string(max) + "\t| " + to_string(suma) + "\n";
        // comunicar finalizacion
        //< termine++; >
        testigo.wait();
        terminado++;
        pasarTestigo(testigo,s_silla,c_silla,n_sentados,s_asignadas,c_asignadas,companyeros,s_espera,c_espera,encontrado,s_sillaP,c_sillaP, s_termina, c_termina,terminado);
    }
}
// ----------------------------------------------------
/*
 * La sbr profesor se encarga de indicar los nips de las personas que se sientan en las sillas.
 * luego espera a que terminen todas las parejas para poder dar por finalizado el examen. 
*/
void Profesor (int sentados[],int companyeros[], bool encontrado[], int filas[], Semaphore& testigo, Semaphore& s_silla, int& c_silla, int& n_sentados, 
    Semaphore* s_asignadas[], int c_asignadas[], Semaphore* s_espera[], int c_espera[], Semaphore& s_sillaP, int& c_sillaP,Semaphore& s_termina,int& c_termina,int& terminado) 
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

        testigo.wait();
        if(n_sentados != 2)
        {
            c_sillaP++;
            testigo.signal();
            s_sillaP.wait();
        }
        companyeros[sentados[0]] = sentados[1];
        companyeros[sentados[1]] = sentados[0];
        filas[sentados[0]] = i;
        filas[sentados[1]] = i;
        n_sentados = 0;
        pasarTestigo(testigo,s_silla,c_silla,n_sentados,s_asignadas,c_asignadas,companyeros,s_espera,c_espera,encontrado,s_sillaP,c_sillaP, s_termina, c_termina,terminado);
    }
    // esperar que todos hayan terminado
    //< await termine == N_FIL 
    //    mostrar mensaje de fin
    //>
    testigo.wait();
    if(terminado != N_FIL)
    {
        c_termina++;
        testigo.signal();
        s_termina.wait();
    }
    pasarTestigo(testigo,s_silla,c_silla,n_sentados,s_asignadas,c_asignadas,companyeros,s_espera,c_espera,encontrado,s_sillaP,c_sillaP,s_termina,c_termina,terminado);
}

// ----------------------------------------------------
int main()
{
    //inicializar semaforos
    thread P[N_EST];
    int matriz[N_FIL][N_COL];
    int sentados[2] = {-1,-1};
    int companyeros[N_EST];
    int maximos[N_FIL]; 
    bool encontrado[N_FIL];
    int filas[N_EST]; 
    Semaphore testigo(1);
    Semaphore s_silla(0);
    int c_silla = 0; 
    int n_sentados = 0;
    Semaphore* s_asignadas[N_EST];
    int c_asignadas[N_EST];
    Semaphore* s_espera[N_FIL];
    int c_espera[N_FIL];
    Semaphore s_sillaP(0);
    int c_sillaP = 0;
    Semaphore s_termina(0);
    int c_termina = 0;
    int terminado = 0;

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
    }

    //inicializar vectores
    for(unsigned int i=0; i<N_EST; i++)
    {
        companyeros[i] = -1;
        filas[i] = -1;
        s_asignadas[i] = new Semaphore(0);
        c_asignadas[i] = 0;
    }
    for(unsigned int i=0; i<N_FIL; i++)
    {
        maximos[i] = -1;
        encontrado[i] = false;
        s_espera[i] = new Semaphore(0);
        c_espera[i] = 0;
    }

    //inicializar threads
    for (int i=0; i<N_EST; i++){
        P[i] = thread(&Estudiante, i, matriz, sentados, companyeros, maximos, encontrado, filas, ref(testigo), ref(s_silla), ref(c_silla), ref(n_sentados), s_asignadas, c_asignadas, s_espera, c_espera, ref(s_sillaP), ref(c_sillaP), ref(s_termina), ref(c_termina), ref(terminado));
    }

    thread th_prof(&Profesor, sentados, companyeros, encontrado, filas, ref(testigo), ref(s_silla), ref(c_silla), ref(n_sentados), s_asignadas, c_asignadas, s_espera, c_espera, ref(s_sillaP), ref(c_sillaP), ref(s_termina), ref(c_termina), ref(terminado));

    //esperar que terminen los thread
    th_prof.join();

    for (int i=0; i<N_EST; i++) {
        P[i].join();
    }

    cout << "\nPrueba finalizada\n";

    return 0;
}