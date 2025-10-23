#include "evento.hpp"

// 
void crearEvento(string desc, int prio, evento& e){
    e.descripcion = desc;
    e.prioridad = prio;
}

// 
string descripcion(const evento& e){
    return e.descripcion;
}

// 
void cambiarDescripcion(evento& e, string desc){
    e.descripcion = desc;
}

// 
int suPrioridad(const evento& e){
    return e.prioridad;
}

// 
void cambiarPrioridad(evento& e, int prio){
    e.prioridad = prio;
}