#include <iostream>
#include <algorithm>
#include "maxsolape.hpp"
using namespace std;
// calculaMaxSolape devuelve un registro tpSolape en el que el campo solape
// es el resultado del solape entre i y j que son dos intervalos
tpSolape calcularMaxSolape(double inters[N][2], unsigned i, unsigned j)
{
    tpSolape res;
    res.solape = 0;
    res.interA = i;
    res.interB = j;
    double ini = max(inters[i][0], inters[j][0]);
    double fin = min(inters[i][1], inters[j][1]);
    res.solape = fin - ini;
    return res;
}
// maxSolFBruta devuelve un registro tpSolape en el que el campo solape
// es el maximo solape entre parejas de los n primeros intervalos de inters,
// y los campos interA e interB son los indices de dichos intervalos.
// Para la matriz inters de ejemplo, el resultado es solape=4.5, interA=0,
// interB=3
// (los valores de interA e interB pueden estar intercambiados, es decir,
// el resultado para el ejemplo anterior también puede ser solape=4.5,
// interA=3, interB=0).
tpSolape maxSolFBruta(double inters[N][2], int n)
{
    tpSolape max;
    max.solape = 0.0;
    tpSolape aux;
    for (unsigned i = 0; i < n - 1; i++)
    {
        for (unsigned j = i + 1; j < n; j++)
        {
            aux = calcularMaxSolape(inters, i, j);
            if (aux.solape > max.solape)
            {
                max.solape = aux.solape;
                max.interA = aux.interA;
                max.interB = aux.interB;
            }
        }
    }
    return max;
}
// Crea un vector de tpInter con los n primeros intervalos de inters.
// Por ejemplo para la matrix inters de la funcion anterior y n=5, los
// valores de indinters seran:
// [{ind: 0, ini: 1.5, fin: 8.0},
// {ind: 1, ini: 0.0, fin: 4.5},
// {ind: 2, ini: 2.0, fin: 4.0},
// {ind: 3, ini: 1.0, fin: 6.0},
// {ind: 4, ini: 3.5, fin: 7.0}]
void crearvind(double inters[N][2], tpInter indinters[N], int n)
{
    for (unsigned i = 0; i < n; i++)
    {
        indinters[i].ind = i;
        indinters[i].ini = inters[i][0];
        indinters[i].fin = inters[i][1];
    }
}

// Ordena el vector de intervalos de forma que el que tenga menor inicio
// queda antes que el otro, sin importar cual es el final de cada intervalo
// Por ejemplo:
// [{ind: 0, ini: 0.0, fin: 8.0},
// {ind: 1, ini: 1.0, fin: 6.5},
// {ind: 2, ini: 2.0, fin: 4.0},
// {ind: 3, ini: 3.0, fin: 6.0},
// {ind: 4, ini: 3.5, fin: 7.0}]

// {ind: 2, ini: 2.0, fin: 4.0},
// {ind: 3, ini: 1.0, fin: 6.0},
// {ind: 4, ini: 3.5, fin: 7.0}]
void crearvind(double inters[N][2], tpInter indinters[N], int n)
{
    for (unsigned i = 0; i < n; i++)
    {
        indinters[i].ind = i;
        indinters[i].ini = inters[i][0];
        indinters[i].fin = inters[i][1];
    }
}
// Ordena el vector de intervalos de forma que el que tenga menor inicio
// queda antes que el otro, sin importar cual es el final de cada intervalo
// Por ejemplo:
// [{ind: 0, ini: 0.0, fin: 8.0},
// {ind: 1, ini: 1.0, fin: 6.5},
// {ind: 2, ini: 2.0, fin: 4.0},
// {ind: 3, ini: 3.0, fin: 6.0},
// {ind: 4, ini: 3.5, fin: 7.0}]

void merge(tpInter w[N], int p, int m, int f)
{
    tpInter b[N];
    int h = p, i = p, j = m + 1;
    while (h <= m && j <= f)
    {
        if (w[h].ini <= w[j].ini)
        {
            b[i].ini = w[h].ini;
            b[i].fin = w[h].fin;
            b[i].ind = w[h].ind;
            h++;
        }
        else
        {
        }
        i++;
    }

    b[i].ini = w[j].ini;
    b[i].fin = w[j].fin;
    b[i].ind = w[j].ind;
    j++;

    if (h > m)
    {
        for (int k = j; k <= f; k++)
        {
            b[i].ini = w[k].ini;
            b[i].fin = w[k].fin;
            b[i].ind = w[k].ind;
            i++;
        }
    }
    else
    {
        for (int k = h; k <= m; k++)
        {
            b[i].ini = w[k].ini;
            b[i].fin = w[k].fin;
            b[i].ind = w[k].ind;
            i++;
        }
    }

    for (int k = p; k <= f; k++)
    {
        w[k].ini = b[k].ini;
        w[k].fin = b[k].fin;
        w[k].ind = b[k].ind;
    }
}
// Ordena con el algoritmo mergesort los intervalos de indinters
// comprendidos entre las componentes indexadas por p y f, ambas incluidas,
// de acuerdo al valor de inicio de los intervalos (orden creciente).
// Por ejemplo, para el vector de la funcion anterior, p=0 y f=4, el vector
// ordenado sera:
// [{ind: 1, ini: 0.0, fin: 4.5},
// {ind: 3, ini: 1.0, fin: 6.0},
// {ind: 0, ini: 1.5, fin: 8.0},
// {ind: 2, ini: 2.0, fin: 4.0},
// {ind: 4, ini: 3.5, fin: 7.0}]
void mergesortIndInters(tpInter indinters[N], int p, int f)
{
    if (p < f)
    {
        int medio = (p + f) / 2;
        mergesortIndInters(indinters, p, medio);
        mergesortIndInters(indinters, medio + 1, f);
        merge(indinters, p, medio, f);
    }
}
tpSolape mayorSol(tpInter indi[N], int p, int medio, int f)
{
    double fin = 0;
    int ind = 0;
    double ini = 0;
    for (unsigned i = 0; i < medio + 1; i++)
    {
        if (fin <= indi[i].fin)
        {
            fin = indi[i].fin;
            ind = indi[i].ind;
            ini = indi[i].ini;
        }
    }

    tpSolape res;
    double maxsolape = 0;

    for (unsigned i = 0; i <= medio; i++)
    {
        if (ind != indi[i].ind)
        {
            double iniult = max(ini, indi[i].ini);
            double finult = min(fin, indi[i].fin);
            double sol = finult - iniult;
            if (maxsolape <= sol)
            {
                maxsolape = sol;
                res.interA = ind;
                res.interB = indi[i].ind;
                res.solape = sol;
            }
        }
    }

    for (unsigned i = medio + 1; i <= f; i++)
    {
        if (ind != indi[i].ind)
        {
            double iniult2 = max(ini, indi[i].ini);
            double finult2 = min(fin, indi[i].fin);
            double sol2 = finult2 - iniult2;
            if (maxsolape <= sol2)
            {
                maxsolape = sol2;
                res.interA = ind;
                res.interB = indi[i].ind;
                res.solape = sol2;
            }
        }
    }
    return res;
}
// Dado un vector indinters, utiliza la tecnica de Divide y Venceras para
// devolver el maximo solape entre parejas de intervalos comprendidos
// entre las componentes indexadas por p y f, ambas incluidas.
// Por ejemplo, para el vector del procedimiento anterior,
// el resultado es solape=4.5, interA=0, interB=3 
tpSolape maxSolDyV(tpInter indinters[N], int p, int f){
    tpSolape res;
    if (p == f)
    {
        res.interA = indinters[p].ind;
        res.interB = indinters[p].ind;
        res.solape = 0;
        return res;
    }
    else
    {
        int medio = (p + f) / 2;
        maxSolDyV(indinters, p, medio);
        maxSolDyV(indinters, medio + 1, f);
        res = mayorSol(indinters, p, medio, f);
        return res;
    }
}
