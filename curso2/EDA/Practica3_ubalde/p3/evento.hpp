/******************************************************************************
 * Fichero: evento.hpp
 * Autores: Daniel Ubalde (875396)
 *          Eduardo Guerrero (816106)
 *
 * Asignatura: Estructuras de Datos y Algoritmos
 ********************************************************************************/

#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

using namespace std;

struct Evento;

/*
 * Operacion: crearEvento
 * ----------------
 * Descripcion: Crea una evento segun los parametros de entrada.
 *
 * Precondicion: Ninguna.
 *
 * Postcondicion: Devuelve un evento con descrip y prio
 *
 * Parametros:
 *   [in] descrip: descripcion inicial que tiene el evento
 *   [in] prio: prioridad inicial que tiene el evento
 *   [in,out] e: evento creado
 *
 * Complejidad: O(1)
 */
void crearEvento(const string &descrip, const unsigned int &prio, Evento &e);

/*
 * Operacion: descripcion
 * ----------------
 * Descripcion: Devuelve la descripcion que tiene el evento.
 *
 * Precondicion: Ninguan.
 *
 * Postcondicion: Devuelve el valor de descrip que tiene e
 *
 * Parametros:
 *   [in] e: evento donde se va a buscar la descripcion
 *
 * Complejidad: O(1)
 */
string descripcion(const Evento &e);

/*
 * Operacion: cambiarDescripcion
 * ----------------
 * Descripcion: Cambia la descripcion del evento.
 *
 * Precondicion: Ninguan.
 *
 * Postcondicion: Cambia descrip en el evento segun el parametro
 *
 * Parametros:
 *   [in,out] e: evento que va a cambiar su descripcion
 *   [in] nueva: nueva descripcion
 *
 * Complejidad: O(1)
 */
void cambiarDescripcion(Evento &e, const string &nueva);


/*
 * Operacion: suPrioridad
 * ----------------
 * Descripcion: Devuelve la prioridad que tiene el evento.
 *
 * Precondicion: Ninguan.
 *
 * Postcondicion: Devuelve el valor de prio que tiene e
 *
 * Parametros:
 *   [in] e: evento donde se va a buscar la priorirdad
 *
 * Complejidad: O(1)
 */
unsigned int suPrioridad(const Evento &e);

/*
 * Operacion: cambiarPrioridad
 * ----------------
 * Descripcion: Cambia la prioridad del evento.
 *
 * Precondicion: Ninguan.
 *
 * Postcondicion: Cambia prio en el evento segun el parametro
 *
 * Parametros:
 *   [in,out] e: evento que va a cambiar su descripcion
 *   [in] pri: nueva descripcion
 *
 * Complejidad: O(1)
 */
void cambiarPrioridad(Evento &e, const unsigned int &pri);

struct Evento {

  friend void crearEvento(const string &descrip, const unsigned int &prio, Evento &e);
  friend string descripcion(const Evento &e);
  friend void cambiarDescripcion(Evento &e, const string &nueva);
  friend unsigned int suPrioridad(const Evento &e);
  friend void cambiarPrioridad(Evento &e, const unsigned int &pri);

  private:
    string descrip;
    unsigned int prio;
};

#endif