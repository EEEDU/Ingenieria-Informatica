/******************************************************************************
 * Fichero: evento.cpp
 * Autores: Daniel Ubalde (875396)
 *          Eduardo Guerrero (816106)
 *
 * Asignatura: Estructuras de Datos y Algoritmos
 ********************************************************************************/

#include "evento.hpp"

/*
 * Implementacion de crearEvento
 */
void crearEvento(const string &descrip, const unsigned int &prio, Evento &e) {
  e.descrip = descrip;
  e.prio = prio;
}

/*
 * Implementacion de descripcion
 */
string descripcion(const Evento &e) {
  return e.descrip;
}

/*
 * Implementacion de cambiarDescripcion
 */
void cambiarDescripcion(Evento &e, const string &nueva) {
  e.descrip = nueva;
}

/*
 * Implementacion de suPrioridad
 */
unsigned int suPrioridad(const Evento &e) {
  return e.prio;
}

/*
 * Implementacion de cambiarPrioridad
 */
void cambiarPrioridad(Evento &e, const unsigned int &pri) {
  e.prio = pri;
}

