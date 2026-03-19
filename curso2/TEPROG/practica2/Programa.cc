#include "Programa.h"

Programa::Programa(std::vector<Instruccion*> inst)
    : lista_instrucciones(inst)
{
}

void Programa::ejecutar_programa()
{
    std::stack<int> pila;
    int pc = 0; 
    while (pc < lista_instrucciones.size())
    {
        lista_instrucciones[pc]->ejecutar(pila);
    }
}