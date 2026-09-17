#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


class Anagrama 
{
    private:
        int cantidad;
        string palabra_clave;
        vector<string> lista_anagramas;

    public:
        Anagrama();
        Anagrama(const string palabra);
        ~Anagrama() = default;

        bool push(const string palabra);
        int getCantidad() const;
        vector<string> getListaAnagramas() const;
};

Anagrama::Anagrama()
:cantidad(0), lista_anagramas({}) {}

Anagrama::Anagrama(const string palabra)
:cantidad(1), palabra_clave(palabra), lista_anagramas({palabra}){}

bool Anagrama::push(const string palabra_nueva)
{
    if (cantidad == 0){
        palabra_clave = palabra_nueva;
        lista_anagramas.push_back(palabra_nueva);
        cantidad++;  
        return true;    
    }
    
    if (palabra_clave.size() == palabra_nueva.size()){
        string clave_ordenada = palabra_clave;
        string nueva_ordenada = palabra_nueva;

        sort(clave_ordenada.begin(), clave_ordenada.end());
        sort(nueva_ordenada.begin(), nueva_ordenada.end());

        if (clave_ordenada == nueva_ordenada){
            lista_anagramas.push_back(palabra_nueva);
            cantidad++;  
            return true; 
        }
    }

    return false;

}

int Anagrama::getCantidad() const
{
    return cantidad;
}

vector<string> Anagrama::getListaAnagramas() const
{
    return lista_anagramas;
}

ostream& operator<<(std::ostream& os, const std::vector<string>& v) {
    os << "[";
    for (const string dato : v)
        os << dato << " ";
    os << "]";
    return os;
}