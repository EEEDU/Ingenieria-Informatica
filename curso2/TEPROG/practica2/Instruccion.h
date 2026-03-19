#ifndef INSTRUCCION_H
#define INSTRUCCION_H

#include <iostream>
#include <stack>

class Instruccion   {
    public:
        virtual void ejecutar(std::stack<int>& pila);
        ~Instruccion();
}

#endif