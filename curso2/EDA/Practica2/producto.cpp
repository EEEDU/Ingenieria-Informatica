#include "producto.hpp"


/* Dada una cadena nombre y un entero cantidad, 
devuelve un producto p con esos datos. 
*/
void crear(string nombre, int cantidad, producto& p){
    p.nombre = nombre;
    p.cantidad = cantidad;
}

/* Dado un producto p, devuelve la cadena correspondiente al nombre de p. 
*/
string nombre(const producto& p){
    return p.nombre;
}

/* Dado un producto p, devuelve la cadena correspondiente a la cantidad de p. 
*/
int cantidad(const producto& p){
    return p.cantidad;
}


/* Devuelve verdad si y sólo si los productos p1 y p2 tienen el mismo nombre. 
*/
bool iguales(const producto& p1, const producto& p2){
    return p1.nombre == p2.nombre;
}