//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: Eduardo Guerrero (816106), Ismael Martinez (818903)
// Date:   Noviembre 2023
// Coms:   Servidor multicliente tareas
//         Invocación
//                  ./ServidorMulticliente
//*****************************************************************

#include "Socket.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <fstream> // Necesario para trabajar con archivos


#include "ControlMatriz.hpp"
#include "librerias/MultiBuffer/MultiBuffer.hpp"

using namespace std;

//-------------------------------------------------------------
// Espera "secs" segundos y se conecta. Usado para desbloquear un "accept"
// Y pone "fin" a true
void timeOut(int secs,bool& fin,int port) {
	Socket chan("localhost", port);
	this_thread::sleep_for(chrono::seconds(secs));
	fin = true;
	int sfd = chan.Connect(); //sacará al servidor del "Accept"
	chan.Close();
}
//-------------------------------------------------------------
void servCliente(Socket& chan, int client_fd, ControlMatriz &controlMatriz) {
    string MENS_FIN = "END OF SERVICE";
    string TAREA_FINAL = "TF";
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;



    bool out = false; // Inicialmente no salir del bucle
    while(!out) {
        // Recibimos el mensaje del cliente
        int rcv_bytes = chan.Receive(client_fd,buffer,length);

        if (rcv_bytes == -1) {
            cerr << chan.error("Error al recibir datos");
            // Cerramos los sockets
            chan.Close(client_fd);
        }

        cout << "Tarea recibida: " + buffer + "\n";
        
        // Si recibimos "END OF SERVICE" --> Fin de la comunicación
        if (buffer == MENS_FIN) {
            out = true; // Salir del bucle
        } 
        else if (buffer == TAREA_FINAL){
            // Enviamos la respuesta
            string s = "END";

            int send_bytes = chan.Send(client_fd, s);
            cout << "Mensaje enviada: " + s + "\n";

            if(send_bytes == -1) {
                cerr << chan.error("Error al enviar datos");
                // Cerramos los sockets
                chan.Close(client_fd);
                exit(1);
            }
            out = true;
        }
        else {
            Tarea tareaEnEjecucion;
            istringstream ss(buffer);
    
            string tarea, resultado, tiempo;

            getline(ss, tarea, ',');
            getline(ss, resultado, ',');
            getline(ss, tiempo, ',');
            tareaEnEjecucion.tipoTarea = tarea;
            try
            {
                tareaEnEjecucion.cargaDeTrabajo = stof(tiempo);
            }
            catch (const std::invalid_argument &e)
            {
                cerr << "Error: No se pudo convertir la cadena a un número de punto flotante." << std::endl;
            }

            controlMatriz.addResult(tareaEnEjecucion, resultado);

            // // Enviamos la respuesta
            // string s = "OK";

            // int send_bytes = chan.Send(client_fd, s);
            // cout << "Mensaje enviada: " + s + "\n";

            // if(send_bytes == -1) {
            //     cerr << chan.error("Error al enviar datos");
            //     // Cerramos los sockets
            //     chan.Close(client_fd);
            //     exit(1);
            // }
        }
    }
    chan.Close(client_fd);
}
//-------------------------------------------------------------
int main(int argc,char* argv[]) {
    cout << "-------------------------------------------------\n"; 
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = stoi(argv[1]); //normalmente será un parámetro de invocación. P.e.: argv[1]
    vector<thread> cliente;
    vector<int> client_fds;

    ControlMatriz controlMatriz;
    
    bool fin = false;

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1) {
        cerr << chan.error("Error en el bind");
        exit(1);
    }

    //lisen
    int error_code = chan.Listen();
    if (error_code == -1) {
        cerr << chan.error("Error en el listen");
        // Cerramos el socket
        chan.Close();
        exit(1);
    }
    //para desbloquear servidor y terminar
    thread timeControl(&timeOut,6000,ref(fin),SERVER_PORT);
    

    int i = 0;
    while (!fin && i < 10) {
    	i++;
        // Accept
        int new_client_fds = chan.Accept();


        if(new_client_fds == -1) {
            cerr << chan.error("Error en el accept");
            // Cerramos el socket
            chan.Close();
            exit(1);
        } else {
        	if (!fin) {
                //introducir en el vector el cliente y arrancar el thread
        	    client_fds.push_back(new_client_fds);
        	    cliente.push_back(thread(&servCliente, ref(chan), new_client_fds, ref(controlMatriz)));
        	    cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
        	}
        	else {
        		cout << "He acabado" << endl;
        	}
        }
    }

    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i=0; i<cliente.size(); i++) {
        cliente[i].join();
        
    }


    timeControl.join();

    // Cerramos el socket del servidor
    error_code = chan.Close();
    if (error_code == -1) {
        cerr << chan.error("Error cerrando el socket del servidor");
    }

    // Despedida
    cout << "Bye bye" << endl;

    return error_code;
}
//-------------------------------------------------------------
