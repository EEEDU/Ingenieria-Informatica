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
#include <thread>
#include <vector>

#include <fstream> // Necesario para trabajar con archivos


#include "ControlBuffer.hpp"
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
void servCliente(Socket& chan, int client_fd, MultiBuffer<Tarea, N_CONTROLLERS> &mBT, ControlBuffer &controlBuffer) {
    string MENS_FIN = "END";
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

        cout << "Mensaje recibido: " + buffer + "\n";
        
        if (buffer == "GET_TASK"){
            Tarea tareaController = controlBuffer.getTask(mBT);
        }

        // Si recibimos "END OF SERVICE" --> Fin de la comunicación
        if (buffer == MENS_FIN) {
            out = true; // Salir del bucle
        } 
        else if (buffer == "GET_TASK") {
            Tarea tareaController = controlBuffer.getTask(mBT);

            // Enviamos la respuesta
            string s = tareaController.tipoTarea + "," + to_string(tareaController.cargaDeTrabajo);

            int send_bytes = chan.Send(client_fd, s);
            if(send_bytes == -1) {
                cerr << chan.error("Error al enviar datos");
                // Cerramos los sockets
                chan.Close(client_fd);
                exit(1);
            }
        }
    }
    chan.Close(client_fd);
}
//-------------------------------------------------------------
bool comprobarTf(MultiBuffer<Tarea, N_CONTROLLERS> mBT){
    int contador = 0;
    for (int i = 0; i < mBT.size(); i++){
        if (mBT.watch(i).tipoTarea == "TF") contador ++;
    }
    if (contador == mBT.size()) return true;
    return false;
}
void master(MultiBuffer<Tarea, N_CONTROLLERS> &mBT, string archivoTareas, ControlBuffer &controlBuffer, bool &final_fichero)
{
    Tarea tarea;
    Tarea tareaFinal = {"TF",0};
    ifstream archivo(archivoTareas);
    string linea;
    bool tf = false;
    // Lee y muestra cada línea del archivo
    while (getline(archivo, linea))
    {
        // Extrea la tarea del archivo
        size_t posicion = linea.find(",");
        tarea.tipoTarea = linea.substr(0, posicion);
        try
        {
            tarea.cargaDeTrabajo = stof(linea.substr(posicion + 1));
        }
        catch (const std::invalid_argument &e)
        {
            cerr << "Error: No se pudo convertir la cadena a un número de punto flotante." << std::endl;
        }

        // Añadir tarea en el multibuffer
        controlBuffer.addTask(tarea, mBT);

    }
    while(!tf){
        // Añadir tareaFinal en el multibuffer
        controlBuffer.addTask(tareaFinal, mBT);

        tf = comprobarTf(mBT);
    }

    final_fichero = true;

    archivo.close();
}
//-------------------------------------------------------------
int main(int argc,char* argv[]) {
    const int N = 4;
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = 3000; //normalmente será un parámetro de invocación. P.e.: argv[1]
    vector<thread> cliente;
    vector<int> client_fds;

    ControlBuffer controlBuffer;
    
    string fichero = "tareas.txt";
    MultiBuffer<Tarea, N_CONTROLLERS> mBT; // multi-buffer de tareas
    bool final_fichero = false;

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
    thread timeControl(&timeOut,60,ref(fin),SERVER_PORT);
    
    thread master_thread(&master , ref(mBT), fichero, ref(controlBuffer), ref(final_fichero));
    master_thread.join();

    int i = 0;
    while (!fin) {
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
        	    cliente.push_back(thread(&servCliente, ref(chan), new_client_fds, ref(mBT), ref(controlBuffer)));
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
