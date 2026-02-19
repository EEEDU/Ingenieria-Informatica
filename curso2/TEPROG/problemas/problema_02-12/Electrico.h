#ifndef ELECTRICO_H
#define ELECTRICO_H

#include <iostream>

class Electrico{
    protected:
        int carga;
        bool cargando;
    
    public:
        Electrico(int carga, bool cargando);
        ~Electrico();

        int get_carga();
        bool get_cargando();
        void set_carga(int carga);
        void set_cargando(bool cargando);
};