/* Daniel Ubalde y Eduardo Guerrero.
*/

#ifndef EVENTO_HPP
#define EVENTO_HPP

#include<iostream>
using namespace std;

// PREDECLARACION DEL TAD envento (inicio INTERFAZ)

struct evento;

// 
void crearEvento(string desc, int prio, evento& e);

// 
string descripcion(const evento& e);

// 
void cambiarDescripcion(evento& e, string nuevdesca);

// 
int suPrioridad(const evento& e);

// 
void cambiarPrioridad(evento& e, int prio);


struct evento {
  friend void crearEvento(string desc, int prio, evento& e);
  friend string descripcion(const evento& e);
  friend void cambiarDescripcion(evento& e, string desc);
  friend int suPrioridad(const evento& e);
  friend void cambiarPrioridad(evento& e, int prio);
  private:  //declaracion de la representacion interna del tipo
    string descripcion;
    int prioridad;
};

#endif
