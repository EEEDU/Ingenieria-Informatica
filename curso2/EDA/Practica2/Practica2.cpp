/* Nombres completos y NIPs de los autores:
 * [PONER AQUÍ VUESTROS NOMBRES Y NIPS]
 */

#include <iostream>
#include <string>
#include <limits>

#include "agrupacion.hpp"
#include "contacto.hpp"
#include "producto.hpp"
using namespace std;

// Prototipos de funciones auxiliares
void probarAgendaContactos();
void probarListaCompra();
void mostrarMenuContactos();
void mostrarMenuProductos();

int main() {
    cout << "=== PRACTICA 2: TAD GENERICO AGRUPACION ===" << endl << endl;
    
    // PARTE 1: Probar agenda de contactos
    cout << ">>> PRUEBA 1: AGENDA DE CONTACTOS <<<" << endl;
    probarAgendaContactos();
    
    cout << endl << "========================================" << endl << endl;
    
    // PARTE 2: Probar lista de la compra
    cout << ">>> PRUEBA 2: LISTA DE LA COMPRA <<<" << endl;
    probarListaCompra();
    
    return 0;
}

// ============= PRUEBA DE AGENDA DE CONTACTOS =============
void probarAgendaContactos() {
    agrupacion<contacto> agenda;
    iniciar(agenda);
    
    int opcion;
    do {
        mostrarMenuContactos();
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer
        
        switch(opcion) {
            case 1: { // Añadir contacto
                string nom, dir;
                int tlf;
                cout << "Nombre: ";
                getline(cin, nom);
                cout << "Direccion: ";
                getline(cin, dir);
                cout << "Telefono: ";
                if (!(cin >> tlf)) { // si la entrada no es un int
                    cout << "Error: el telefono debe ser un numero entero." << endl;
                    cin.clear(); // limpiar el estado de error
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar toda la entrada inválida
                    break; // salir del case
                }
                cin.ignore(); // limpiar salto de línea pendiente
                
                contacto c;
                crear(nom, dir, tlf, c);
                if (anyadir(agenda, c)) {
                    cout << "Contacto " << nombre(c) << " agregado correctamente" << endl;
                } else {
                    cout << "Error: Agenda llena" << endl;
                }
                break;
            }
            case 2: { // Mostrar todos los contactos
                if (vacia(agenda)) {
                    cout << "La agenda esta vacia" << endl;
                } else {
                    cout << "\n--- CONTACTOS EN LA AGENDA ---" << endl;
                    iniciarIterador(agenda);
                    int contador = 1;
                    do {
                        contacto c = siguiente(agenda);
                        cout << contador << ". " << nombre(c) 
                             << " | Tel: " << telefono(c)
                             << " | Dir: " << direccion(c) << endl;
                        contador++;
                        avanza(agenda);
                    } while (existeSiguiente(agenda));
                }
                break;
            }
            case 3: { // Borrar último contacto
                if (!vacia(agenda)) {
                    borrarUltimo(agenda);
                    cout << "Ultimo contacto borrado" << endl;
                } else {
                    cout << "La agenda esta vacia" << endl;
                }
                break;
            }
            case 4: { // Buscar contacto
                string nom;
                cout << "Nombre a buscar: ";
                getline(cin, nom);
                
                contacto c;
                crear(nom, "", 0, c); // Crear contacto temporal para buscar
                if (esta(agenda, c)) {
                    cout << "El contacto '" << nom << "' esta en la agenda" << endl;
                } else {
                    cout << "El contacto '" << nom << "' NO esta en la agenda" << endl;
                }
                break;
            }
            case 0:
                cout << "Saliendo de la agenda..." << endl;
                break;
            default:
                cout << "Opción no válida" << endl;
        }
        cout << endl;
    } while (opcion != 0);
}

// ============= PRUEBA DE LISTA DE LA COMPRA =============
void probarListaCompra() {
    agrupacion<producto> listaCompra;
    iniciar(listaCompra);
    
    int opcion;
    do {
        mostrarMenuProductos();
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer
        
        switch(opcion) {
            case 1: { // Añadir producto
                string nom;
                int cant;
                cout << "Nombre del producto: ";
                getline(cin, nom);
                cout << "Cantidad: ";
                cin >> cant;
                cin.ignore();
                
                producto p;
                crear(nom, cant, p);
                if (anyadir(listaCompra, p)) {
                    cout << "Producto agregado correctamente" << endl;
                } else {
                    cout << "Error: Lista llena" << endl;
                }
                break;
            }
            case 2: { // Mostrar todos los productos
                if (vacia(listaCompra)) {
                    cout << "La lista de la compra esta vacia" << endl;
                } else {
                    cout << "\n--- PRODUCTOS EN LA LISTA ---" << endl;
                    iniciarIterador(listaCompra);
                    int contador = 1;
                    while (existeSiguiente(listaCompra)) {
                        producto p = siguiente(listaCompra);
                        cout << contador << ". " << nombre(p) 
                             << " (cantidad: " << cantidad(p) << ")" << endl;
                        contador++;
                    }
                }
                break;
            }
            case 3: { // Borrar último producto
                if (!vacia(listaCompra)) {
                    borrarUltimo(listaCompra);
                    cout << "Ultimo producto borrado" << endl;
                    
                    // Mostrar productos restantes
                    cout << "\n--- PRODUCTOS RESTANTES ---" << endl;
                    if (vacia(listaCompra)) {
                        cout << "La lista esta vacia" << endl;
                    } else {
                        iniciarIterador(listaCompra);
                        int contador = 1;
                        while (existeSiguiente(listaCompra)) {
                            producto p = siguiente(listaCompra);
                            cout << contador << ". " << nombre(p) 
                                 << " (cantidad: " << cantidad(p) << ")" << endl;
                            contador++;
                        }
                    }
                } else {
                    cout << "La lista esta vacia" << endl;
                }
                break;
            }
            case 4: { // Buscar producto por nombre
                string nom;
                cout << "Nombre del producto a buscar: ";
                getline(cin, nom);
                
                producto p;
                crear(nom, 0, p); // Crear producto temporal para buscar
                if (esta(listaCompra, p)) {
                    cout << "El producto '" << nom << "' esta en la lista" << endl;
                } else {
                    cout << "El producto '" << nom << "' NO esta en la lista" << endl;
                }
                break;
            }
            case 5: { // PRUEBA DE ERROR: intentar añadir un contacto
                cout << "\n>>> PRUEBA: Intentando añadir un contacto a la lista de productos..." << endl;
                cout << "Esto NO debería compilar:" << endl;
                cout << "// contacto c;" << endl;
                cout << "// crear(\"Juan\", \"Calle Mayor\", 123456, c);" << endl;
                cout << "// anyadir(listaCompra, c);  <-- ERROR DE COMPILACION" << endl;
                cout << "\nSi descomentamos esas líneas, el compilador dará error de tipos" << endl;
                cout << "porque listaCompra es agrupacion<producto>, no agrupacion<contacto>" << endl;
                
                // Si descomentas estas líneas, dará error de compilación:
                // contacto c;
                // crear("Juan", "Calle Mayor", 123456, c);
                // anyadir(listaCompra, c);
                break;
            }
            case 0:
                cout << "Saliendo de la lista de la compra..." << endl;
                break;
            default:
                cout << "Opción no válida" << endl;
        }
        cout << endl;
    } while (opcion != 0);
}

// ============= MENÚS =============
void mostrarMenuContactos() {
    cout << "\n--- MENU AGENDA ---" << endl;
    cout << "1. Agregar contacto" << endl;
    cout << "2. Mostrar todos los contactos" << endl;
    cout << "3. Borrar ultimo contacto" << endl;
    cout << "4. Buscar contacto por nombre" << endl;
    cout << "0. Salir" << endl;
}

void mostrarMenuProductos() {
    cout << "\n--- MENU LISTA DE LA COMPRA ---" << endl;
    cout << "1. Agregar producto" << endl;
    cout << "2. Mostrar todos los productos" << endl;
    cout << "3. Borrar ultimo producto y mostrar restantes" << endl;
    cout << "4. Buscar producto por nombre" << endl;
    cout << "5. [PRUEBA] Intentar agregar contacto (error compilacion)" << endl;
    cout << "0. Salir" << endl;
}