/* Nombre, apellidos y NIP del (de los) autor(es).
*/

#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include<iostream>
using namespace std;

// PREDECLARACION DEL TAD contacto (inicio INTERFAZ)

/* Los valores del TAD contacto representan informacion de personas,
a las que llamamos contactos, para las que se tiene 
informacion de su nombre, su direccion y su numero de telefono.
Dos contactos se consideran iguales (con el operador ==) si tienen el mismo nombre.
*/
struct producto;

/* Crear un nuevo producto con un nombre y cantidad
*/
void crear(string nom, int c, producto& p);

/*  Devuelve el nombre de un producto
*/
string nombre(const producto& p);

/*  Devuelve la cantidad de un producto
*/
int cantidad(const producto& p);

/*  Compara dos producto y devuelve si son iguales
*/
bool iguales(const producto& p1, const producto& p2);


// FIN de la PREDECLARACION DEL TAD contacto (fin INTERFAZ)

// DECLARACION DEL TAD contacto 

struct producto {
  friend void crear(string nom, int c, producto& p);
  friend string nombre(const producto& p);
  friend int cantidad(const producto& p);
  friend bool iguales(const producto& p1, const producto& p2);
  private:  //declaracion de la representacion interna del tipo
            //... a completar COMPLETAR CON documentacion sobre la representacion interna ...
    string nombre;
    int cantidad;
};

#endif
