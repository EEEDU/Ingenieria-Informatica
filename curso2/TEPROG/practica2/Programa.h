#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "Instruccion.h"

#include <iostream>
#include <stack>
#include <vector>

class Programa {
    protected:
        std::vector<Instruccion*> lista_instrucciones;

    public:
        Programa(std::vector<Instruccion*> inst);
        ~Programa();

        void ejecutar_programa();
        void listar_instrucciones();
}

#endif