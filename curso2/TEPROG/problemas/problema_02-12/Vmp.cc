#include "Vmp.h"

Vmp::Vmp(int id, float lat, float lon, bool ap_est, bool dis)
    : id(id), aparcado_estacion(ap_est), disponible(dis)
{
    localizacion[0] = lat;
    localizacion[1] = lon;
}

int Vmp::get_id(){
    return id;
}
float* Vmp::get_localizacion(){
    return localizacion;
}
bool Vmp::get_aparcado_estacion(){
    return aparcado_estacion;
}
bool Vmp::get_disponible(){
    return disponible;
}
void Vmp::set_id(int id){
    this->id = id;
}    
void Vmp::set_localizacion(float lat, float lon)
{
    localizacion[0] = lat;
    localizacion[1] = lon;
}
void Vmp::set_aparcado_estacion(bool ap_est){
    aparcado_estacion = ap_est;
}
void Vmp::set_disponible(bool dis){
    disponible = dis;
}

float* Vmp::distancia_centro()
{
    float distancia[2];
    
}
        void alquilar();
        void devuelve();
        void liberar();
        void bloquear();