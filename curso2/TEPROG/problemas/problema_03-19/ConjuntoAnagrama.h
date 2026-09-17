#pragma once

#include "Anagrama.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class ConjuntoAnagrama 
{
    private:
        vector<Anagrama> lista_anagrama;

    public:
        ConjuntoAnagrama(const vector<Anagrama> conj);
        ~ConjuntoAnagrama() = default;
        void listarAnagramas() const;
};

ConjuntoAnagrama::ConjuntoAnagrama(const vector<Anagrama> conj)
:lista_anagrama(conj) {}

void ConjuntoAnagrama::listarAnagramas() const
{

    vector<Anagrama> copia = lista_anagrama;

    // Ordenamos los objetos Anagrama por cantidad
    sort(copia.begin(), copia.end(), [](const Anagrama& a, const Anagrama& b) {
        return a.getCantidad() < b.getCantidad();  // menor a mayor
    });

    // Insertamos las palabras de cada Anagrama en el resultado
    for (const Anagrama anagrama : copia)
    {
        cout << anagrama.getListaAnagramas() << endl;
    }
}