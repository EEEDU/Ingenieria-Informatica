#ifndef VMP_H
#define VMP_H

#include <iostream>

class Vmp{
    protected:
        int id;
        float localizacion[2];
        bool aparcado_estacion, disponible;

    public:
        Vmp(int id, float lat, float lon, bool ap_est, bool dis);
        ~Vmp();

        int get_id();
        float* get_localizacion();
        bool get_aparcado_estacion();
        bool get_disponible();
        void set_id(int id);    
        void set_localizacion(float lat, float lon);
        void set_aparcado_estacion(bool ap_est);
        void set_disponible(bool dis);

        float* distancia_centro();
        void alquilar();
        void devuelve();
        void liberar();
        void bloquear();

};

#endif
