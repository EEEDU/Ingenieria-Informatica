//*****************************************************************
// File:   Cliente.cpp
// Author: Eduardo Guerrero (nip:816106) && Ismael Martinez (nip:818903)
// Date:   noviembre 2023
// Coms:   Cliente controlador con comunicación síncrona mediante sockets
//         Invocación
//                  ./Cliente
//*****************************************************************
#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.hpp"

#include "Tarea.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

string ejecutarTarea(string buffer){
    Tarea tareaEnEjecucion;

    size_t posicion = buffer.find(",");
    tareaEnEjecucion.tipoTarea = buffer.substr(0, posicion);
    try
    {
        tareaEnEjecucion.cargaDeTrabajo = stof(buffer.substr(posicion + 1));
    }
    catch (const std::invalid_argument &e)
    {
        cerr << "Error: No se pudo convertir la cadena a un número de punto flotante." << std::endl;
    }

    int numero_aleatorio = rand() % (100);
    float numeroAleatorio = static_cast<float>(rand()) / RAND_MAX;

    if (tareaEnEjecucion.tipoTarea == "t1" && numero_aleatorio < 5){
        return tareaEnEjecucion.tipoTarea + ",OK," + to_string(tareaEnEjecucion.cargaDeTrabajo);
    }
    else if(tareaEnEjecucion.tipoTarea == "t2" && numero_aleatorio < 7){
        return tareaEnEjecucion.tipoTarea + ",OK," + to_string(tareaEnEjecucion.cargaDeTrabajo);
;
    }
    else if(tareaEnEjecucion.tipoTarea == "t3" && numero_aleatorio < 10){
        return tareaEnEjecucion.tipoTarea + ",OK," + to_string(tareaEnEjecucion.cargaDeTrabajo);
    }
    return tareaEnEjecucion.tipoTarea + ",NO_OK," + to_string(tareaEnEjecucion.cargaDeTrabajo);
}

int main(int argc,char* argv[]) {
    const string MENS_FIN = "END OF SERVICE";
    const string MENS_PEDIR_TAREA = "GET_TASK";
    const string TAREA_FINAL = "TF";
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS_TASK = argv[1]; //alternativamente "127.0.0.1"
    string SERVER_ADDRESS_MAT = argv[3]; //alternativamente "127.0.0.1"
    int SERVER_TASK_PORT = stoi(argv[2]);
    int SERVER_MAT_PORT = stoi(argv[4]);

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan_task(SERVER_ADDRESS_TASK, SERVER_TASK_PORT);
    Socket chan_mat(SERVER_ADDRESS_MAT, SERVER_MAT_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count_task = 0;
    int socket_task;
    do {
        // Conexión con el servidor
        socket_task = chan_task.Connect();
        
        count_task++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_task == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_task == -1 && count_task < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_task == -1) {
        return socket_task;
    }

    // Conectamos con el servidor. Probamos varias conexiones
    int count_mat = 0;
    int socket_mat;
    do {
        // Conexión con el servidor
        socket_mat = chan_mat.Connect();
        
        count_mat++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_mat == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_mat == -1 && count_mat < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_mat == -1) {
        return socket_mat;
    }

    string mensaje;
    int read_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje

    do {
        // Leer mensaje de la entrada estandar
        do {
            cout << "Escribir " << MENS_PEDIR_TAREA << " para pedir una tarea: ";
            getline(cin, mensaje);
        } while (mensaje == ""); //no se puede enviar mensaje vacío

        // Enviamos el mensaje
        send_bytes = chan_task.Send(socket_task, mensaje);

        if(send_bytes == -1) {
            cerr << chan_task.error("Error al enviar datos");
            // Cerramos el socket
            chan_task.Close();
            exit(1);
        }

        if(mensaje == MENS_PEDIR_TAREA) {

            // Buffer para almacenar la respuesta
            string buffer;
            string resultado;
            // Recibimos la respuesta del servidor
            read_bytes = chan_task.Receive(socket_task, buffer, MESSAGE_SIZE);

            // Mostramos la respuesta
            cout << "Mensaje enviado: '" << mensaje << "'" << endl;
            cout << "Tarea recibida: " << buffer << endl;

            if (buffer == TAREA_FINAL){
                resultado = TAREA_FINAL;

                cout << "El resultado de la tarea es: " << resultado << endl;

                // Enviamos el mensaje
                send_bytes = chan_mat.Send(socket_mat, resultado);

                if(send_bytes == -1) {
                    cerr << chan_mat.error("Error al enviar datos");
                    // Cerramos el socket
                    chan_mat.Close();
                    exit(1);
                }    

                string respuesta = "";
                // Recibimos la respuesta del servidor
                read_bytes = chan_mat.Receive(socket_mat, respuesta, MESSAGE_SIZE);
                
                if (respuesta == "END"){

                    // Enviamos el mensaje
                    send_bytes = chan_task.Send(socket_task, respuesta);

                    if(send_bytes == -1) {
                        cerr << chan_task.error("Error al enviar datos");
                        // Cerramos el socket
                        chan_task.Close();
                        exit(1);
                    } 

                    cout << "Bye bye" << endl;
                    chan_task.Close();
                    exit(0);
                }

            } 
            else {
               resultado = ejecutarTarea(buffer);

                cout << "El resultado de la tarea es: " << resultado << endl;

                // Enviamos el mensaje
                send_bytes = chan_mat.Send(socket_mat, resultado);

                if(send_bytes == -1) {
                    cerr << chan_mat.error("Error al enviar datos");
                    // Cerramos el socket
                    chan_mat.Close();
                    exit(1);
                }    
            } 
            // resultado = ejecutarTarea(buffer);

            // cout << "El resultado de la tarea es: " << resultado << endl;

            // // Enviamos el mensaje
            // send_bytes = chan_mat.Send(socket_mat, resultado);

            // if(send_bytes == -1) {
            //     cerr << chan_mat.error("Error al enviar datos");
            //     // Cerramos el socket
            //     chan_mat.Close();
            //     exit(1);
            // }    

            // string respuesta = "";
            // // Recibimos la respuesta del servidor
            // read_bytes = chan_mat.Receive(socket_mat, respuesta, MESSAGE_SIZE);
            
            // if (respuesta == "END"){

            //     // Enviamos el mensaje
            //     send_bytes = chan_task.Send(socket_task, respuesta);

            //     if(send_bytes == -1) {
            //         cerr << chan_task.error("Error al enviar datos");
            //         // Cerramos el socket
            //         chan_task.Close();
            //         exit(1);
            //     } 

            //     cout << "Bye bye" << endl;
            //     chan_task.Close();
            //     exit(0);
            // }
            
   
        }
    } while(mensaje != MENS_FIN);

    // Cerramos el socket
    int error_code_task = chan_task.Close();
    int error_code_mat = chan_mat.Close();
    if(error_code_task == -1) {
        cerr << chan_task.error("Error cerrando el socket");
    }
    if(error_code_mat == -1) {
        cerr << chan_mat.error("Error cerrando el socket");
    }

    // Despedida
    cout << "Bye bye" << endl;

    return error_code_task;
}
