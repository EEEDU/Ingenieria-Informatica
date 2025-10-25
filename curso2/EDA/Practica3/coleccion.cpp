#include "Coleccion.hpp"

template<typename I, typename V>  
void crear(Coleccion<I,V>& c){
    c.primera = nullptr;
    c.tamano = 0;
    c.actual = nullptr;
}

template<typename I, typename V>  
int tamano(const Coleccion<I,V>& c){
    return c.tamano;
}

template<typename I, typename V>  
bool esVacia(const Coleccion<I,V>& c){
    return c.tamano == 0;
}

template<typename I, typename V>  
bool existe(I id, const Coleccion<I,V>& c){
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    // Recorrer mientras haya nodos Y el identificador sea menor
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    // Si encontramos un nodo y su ident es igual a id
    if (aux != nullptr && aux->ident == id) {
        return true;
    }
    return false;
}

template<typename I, typename V>  
bool existeDependiente(I id, Coleccion<I,V>& c){
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    // Recorrer mientras haya nodos Y el identificador sea menor
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    // Si encontramos un nodo y su ident es igual a id
    if (aux != nullptr && aux->ident == id && aux->esDependiente == true) {
        return true;
    }
    return false;
}

template<typename I, typename V>  
bool existeIndependiente(I id, Coleccion<I,V>& c){
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    // Recorrer mientras haya nodos Y el identificador sea menor
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    // Si encontramos un nodo y su ident es igual a id
    if (aux != nullptr && aux->ident == id && aux->esDependiente == false) {
        return true;
    }
    return false;
}

template<typename I, typename V>  
void anyadirIndependiente(Coleccion<I,V>& c, I id, V v){
    // Si la coleccion esta vacia, se crea el nuevo elemento en la primera
    if(c.primera == nullptr) {
        c.primera = new typename Coleccion<I,V>::Elemento;
        c.primera->ident = id;
        c.primera->val = v;
        c.primera->esDependiente = false;
        c.primera->numDepend = 0;
        c.primera->sig = nullptr;
        c.tamano = 1;
        return;
    }

    // Si el nuevo id es menor que el id del primero, se guarda la pimera en un aux y se crea en el primera
    typename Coleccion<I,V>::Elemento* aux = c.primera;
    if (id < c.primera->ident) {
        c.primera = new typename Coleccion<I,V>::Elemento;
        c.primera->ident = id;
        c.primera->val = v;
        c.primera->esDependiente = false;
        c.primera->numDepend = 0;
        c.primera->sig = aux;
        c.tamano += 1;
        return;
    }
    
    // Sino, se busca el lugar del elemento
    // Recorrer mientras haya nodos Y el identificador sea menor
    while (aux != nullptr && aux->ident < id) {
        aux = aux->sig;
    }
    // Si encontramos un nodo y su ident es igual a id salimos de la funcion, ya esta creado
    if (aux != nullptr && aux->ident == id) {
        return;
    }
    // Si no existe, se crea uno nuevo despues del aux
    typename Coleccion<I,V>::Elemento* nuevo = new typename Coleccion<I,V>::Elemento;
    nuevo->ident = id;
    nuevo->val = v;
    nuevo->esDependiente = false;
    nuevo->numDepend = 0;
    nuevo->sig = aux->sig;
    aux->sig = nuevo;
    c.tamano += 1;
}


