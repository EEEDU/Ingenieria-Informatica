#include "agenda.hpp"
#include "contacto.hpp"

#include <iostream>
#include <string>
#include <functional>  
#include <list>  

using namespace std;

contacto preguntar_inicio();
int preguntar_accion();
contacto preguntar_crear_contacto();
void preguntar_comparar_contactos(list<contacto>);
void preguntar_crear_agenda(agenda&);
void preguntar_agregar_contacto(agenda&, list<contacto>);
void preguntar_borrar_ultimo_contacto(agenda&);
void preguntar_contacto_en_agenda(agenda&, list<contacto>);

int main() {    
    contacto usuario;
    list<contacto> contactos_creados;   // lista vacía de contactos
    // list<agenda> agendas_creadas;   // lista vacía de agendas
    // Como no tienes forma de identificar las agendas solo se puede crear una
    agenda agenda_creada;
    int accion;


    usuario = preguntar_inicio();

    accion = preguntar_accion();

    switch (accion) {
        case 11:  // Crear nuevo contacto
            cout << "\nHas elegido crear un contacto\n" << endl;
            contactos_creados.push_back(preguntar_crear_contacto());
            break;  

        case 12:  // Comprobar dos contactos
            cout << "\nHas elegido comprobar el nombre de dos contactos\n" << endl;
            preguntar_comparar_contactos(contactos_creados);
            break;

        case 21:  // Crear agenda
            cout << "\nHas elegido crear una nueva agenda\n" << endl;
            preguntar_crear_agenda(ref(agenda_creada));
            break;

        case 22:  // Agregar contacto en la agenda
            cout << "\nHas elegido agregar contacto en la agenda\n" << endl;
            preguntar_agregar_contacto(ref(agenda_creada), contactos_creados);
            break;

        case 23:  // Agregar vaciar agenda
            cout << "\nHas elegido vaciar la agenda\n" << endl;
            preguntar_borrar_ultimo_contacto(ref(agenda_creada));
            break;

        case 24:  // Comprobar si un contacto esta en la agenda
            cout << "\nHas elegido comprobar si un contacto esta en la agenda\n" << endl;
            preguntar_contacto_en_agenda(ref(agenda_creada), contactos_creados);
            break;
        default:
            cout << "Opción no válida\n";
            break;
    }
    
}

contacto preguntar_inicio(){
    string nombre_usuario, direccion_usuario;
    int telefono_usuario;
    contacto usuario;
    cout << "Buenos dias, necesito que me respondas a unas preguntas" << endl;
    cout << "Como te llamas?" << endl;
    getline(cin, nombre_usuario);
    cout << "Cual es tu direccion de correo electronico?" << endl;
    getline(cin, direccion_usuario);
    cout << "Y por ultimo, cual es tu numero de telefono?" << endl;
    cin >> telefono_usuario;
    cin.ignore();
    
    crear(nombre_usuario, direccion_usuario, telefono_usuario, ref(usuario));
    
    cout << "\nGENIAL!!  Ya te hemos registrado como un nuevo contacto." << endl;
    cout << "   Nombre: " << nombre(ref(usuario)) << endl;
    cout << "   Direccion correo electronico: " << direccion(ref(usuario)) << endl;
    cout << "   Telefono: " << telefono(ref(usuario)) << endl;

    return usuario;
}

int preguntar_accion(){
    int respuesta_tipo, respuesta_accion;
    bool respuesta_valida = false;

    cout << "\nTe gustaria manipular contactos o agendas?" << endl;
    cout << "   [1] Contactos" << endl;
    cout << "   [2] Agendas" << endl;
    cin >> respuesta_tipo;
    if (respuesta_tipo == 1 || respuesta_tipo == 2) {
        respuesta_valida = true;
    }
    // TODO: Comprobar que la respuesta es valida

    cout << "Que accion te gustaria hacer?" << endl;
    if (respuesta_tipo == 1){
        cout << "   [1] Crear un contacto" << endl;
        cout << "   [2] Comprobar si dos contactos tienen el mismo nombre" << endl;
    }
    else {
        cout << "   [1] Crear la agenda" << endl;
        cout << "   [2] Agregar un contacto a la agenda" << endl;
        cout << "   [3] Borrar el ultimo contacto a la agenda" << endl;
        cout << "   [4] Comprobar si un contacto esta en la agenda" << endl;
    }
    cin >> respuesta_accion;

    return stoi(to_string(respuesta_tipo) + to_string(respuesta_accion));
}

contacto preguntar_crear_contacto(){
    string nombre_contacto, direccion_contacto;
    int telefono_contacto;
    contacto contacto;
    cout << "Cual es el nombre del nuevo contacto?" << endl;
    getline(cin, nombre_contacto);
    cout << "Cual es la direccion de correo electronico del nuevo contacto?" << endl;
    getline(cin, direccion_contacto);
    cout << "Cual es tu numero de telefono del nuevo contacto?" << endl;
    cin >> telefono_contacto;
    cin.ignore();
    
    crear(nombre_contacto, direccion_contacto, telefono_contacto, ref(contacto));
    
    cout << "\nGENIAL!!  Hemos creado el nuevo contacto." << endl;
    cout << "   Nombre: " << nombre(ref(contacto)) << endl;
    cout << "   Direccion correo electronico: " << direccion(ref(contacto)) << endl;
    cout << "   Telefono: " << telefono(ref(contacto)) << endl;
    return contacto;
}

void preguntar_comparar_contactos(list<contacto> contactos_creados){
    contacto contacto1, contacto2;
    string nombre_contacto1, nombre_contacto2;
    bool encontrado_contacto1 = false, encontrado_contacto2 = false;

    cout << "Como se llama el primer contacto?" << endl;
    cin >> nombre_contacto1;

    cout << "Como se llama el segundo contacto?" << endl;
    cin >> nombre_contacto2;

    // Buscar contactos
    for (contacto i_contacto : contactos_creados) {
        if (nombre_contacto1 == nombre(ref(i_contacto))){
            encontrado_contacto1 = true;
            contacto1 = i_contacto;
        }
        else if (nombre_contacto2 == nombre(ref(i_contacto))){
            encontrado_contacto2 = true;
            contacto2 = i_contacto;
        }
        if (encontrado_contacto1 && encontrado_contacto2) {break;}
    }

    if (!encontrado_contacto1 || !encontrado_contacto2){
        cout << "ERROR: No se ha encontrado alguno de los dos contactos" << nombre_contacto1 << " o " << nombre_contacto2 << endl;
        return;
    }

    if (operator==(ref(contacto1), ref(contacto2))){
        cout << "Los dos contactos tienen el mismo nombre" << endl;
    }
    else {
        cout << "Los contactos tienen nombres diferentes" << endl;
    }
    return;
}

void preguntar_crear_agenda(agenda& agenda_creada){
    char respuesta;
    if (vacia(ref(agenda_creada))){
        iniciar(ref(agenda_creada));
    }
    else {
        cout << "Seguro que quieres crear la agenda? Esta borrara los datos que tienes" << endl;
        cout << "   [S] Si" << endl;
        cout << "   [N] No" << endl;
        cin >> respuesta;
        if (respuesta == 'S'){
            iniciar(ref(agenda_creada));
            cout << "La agenda se ha creado" << endl;
            return;
        }
        cout << "La agenda no se ha creado" << endl;
        return;
    }
}

void preguntar_agregar_contacto(agenda& agenda_creada, list<contacto> contactos_creados){
    string nombre_contacto;
    bool nombre_encontrado = false, creacion_correcta;
    cout << "Como se llama el contacto que quieres agregar?" << endl;
    getline(cin, nombre_contacto);

    for (contacto i_contacto: contactos_creados){
        if (nombre(ref(i_contacto)) == nombre_contacto){
            nombre_encontrado = true;
            creacion_correcta = anyadir(ref(agenda_creada), ref(i_contacto));
            if (creacion_correcta){
                cout << "El contacto se ha agregado" << endl;
            }
            else {
                cout << "El contacto no se ha podido agregar, debido a que la agenda esta llena" << endl;
            }
            break;
        }
    }
    if (!nombre_encontrado){
        cout << "El contacto " << nombre_contacto << " no se ha encontrado" << endl;
    }
    return;
}

void preguntar_borrar_ultimo_contacto(agenda& agenda_creada){
    char respuesta;
    cout << "Seguro que quieres borrar el ultimo contacto de la agenda?" << endl;
    cout << "   [S] Si" << endl;
    cout << "   [N] No" << endl;
    cin >> respuesta;
    if (respuesta == 'S' and !vacia(ref(agenda_creada))){
        borrarUltimo(ref(agenda_creada));
        cout << "Se ha borrado el ultimo contacto de la agenda" << endl;
        return;
    }
    cout << "No se ha podido borrar el ultimo contacto de la agenda" << endl;
    return;
}

void preguntar_contacto_en_agenda(agenda& agenda_creada, list<contacto> contactos_creados){
    string nombre_contacto;
    bool nombre_encontrado = false, contacto_esta;
    cout << "Como se llama el contacto que quieres comprobar si esta en la agenda?" << endl;
    getline(cin, nombre_contacto);

    for (contacto i_contacto: contactos_creados){
        if (nombre(ref(i_contacto)) == nombre_contacto){
            nombre_encontrado = true;
            contacto_esta = esta(ref(agenda_creada), ref(i_contacto));
            if (contacto_esta){
                cout << "El contacto esta en la agenda" << endl;
            }
            else {
                cout << "El contacto no esta en la agenda" << endl;
            }
            break;
        }
    }
    if (!nombre_encontrado){
        char respuesta;
        cout << "El contacto " << nombre_contacto << " no se ha encontrado" << endl;
        cout << "Quieres crear el contacto y luego agregarlo a la agenda?" << endl;
        cout << "   [S] Si" << endl;
        cout << "   [N] No" << endl;
        cin >> respuesta;    
        if (respuesta == 'S'){
            contactos_creados.push_back(preguntar_crear_contacto());
            cout << "Se ha creado el contacto" << endl;
            return;
        }
        cout << "No se ha creado el contacto" << endl;
        return;
    }
    return;
}
