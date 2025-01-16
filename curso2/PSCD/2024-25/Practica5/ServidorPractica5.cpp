//******************************************************************
// File:   ServidorPractica5.cpp
// Author: 816106
// Date:   Noviembre 2024
// Coms:   Servidor para la practica 5
//                  ./ServidorPractica5
//*****************************************************************

#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>

#include "Socket.hpp"

#include "MonitorSilla.hpp"
#include "MonitorDatos.hpp"
#include "MonitorFinal.hpp"

using namespace std;

// ----------------------------------------------------
const int N_EST = 60;        // # de estudiantes
const int N_FIL = N_EST / 2; // # de filas en la matriz
const int N_COL = 1000;      // # de columnas

//------------------------------------------------------------
//Pre: <s> contiene un string con 1 o m´as campos, separados por el caracter en <sep>
//Post: devuelve un vector cuyas componentes son los valores que estaban separados por <sep> en <s>, por orden
//Com: Mirar la documentacion sobre la clase ‘‘vector’’
vector<string> split(const string& frase, char separador) {
    vector<string> elementos;
    string elemento;
    istringstream sequenciaElementos(frase);

    // Verificar si la frase termina con el separador
    if (!frase.empty() && frase.back() == separador) {
        elementos.push_back(elemento); // Remover el último elemento vacío
        return elementos;

    }
    while (getline(sequenciaElementos, elemento, separador)) {
        elementos.push_back(elemento);
    }
    return elementos;
}

// ----------------------------------------------------
// Pre : archivo donde se encuntran los datos
// Post : los datos en formato array[][]
void leerDatos(string nombreArchivo, int D[N_FIL][N_COL])
{
    // Intentamos abrir el archivo de datos
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo datos.txt" << endl;
    }

    // Vector temporal para almacenar los números del archivo
    vector<int> numeros;
    int numero;

    // Leemos todos los números desde el archivo
    while (archivo >> numero) {
        numeros.push_back(numero);
    }

    // Cerramos el archivo
    archivo.close();

    // Llenamos la matriz con los números leídos
    for (int i = 0; i < N_FIL; ++i) {
        for (int j = 0; j < N_COL; ++j) {
            D[i][j] = numeros[i * N_COL + j];
        }
    }
}

bool substringExists(string s, string subs){
    return (s.find(subs) != string::npos);
}

void estudiante(MonitorSilla& mSilla, MonitorDatos& mDatos, MonitorFinal& mFinal, Socket& chan, int client_fd)
{
    string MENS_FIN = "END OF SERVICE";
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;
    vector<string> bufferPorComas;
    
    int nip, filaEstudiante, parejaEstudiante, suma, numMax;

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
        bufferPorComas = split(buffer, ',');

        // Si recibimos "   " --> Fin de la comunicación
        if (buffer == MENS_FIN) {
            out = true; // Salir del bucle
        } 
        // En caso de recibir mensaje de sentarse envia nipPareja,fila
        else if(bufferPorComas[0] == "sentar"){
            
            // Coge el nip como el parametro despues de la coma
            nip = stoi(bufferPorComas[1]);

            // Utilizar el monitor para sentarse en la silla
            mSilla.sentarse(nip);

            // Utilizar el monitor para levantarse de la silla recogiendo informacion <fila, pareja>
            tuple<int, int> datosEstudiante = mSilla.levantarse(nip);
            filaEstudiante = get<0>(datosEstudiante);
            parejaEstudiante = get<1>(datosEstudiante);
            // Concatenar y enviar la respuesta con nipPareja,fila
            string respuesta = to_string(parejaEstudiante) + "," + to_string(filaEstudiante);
            int send_bytes = chan.Send(client_fd, respuesta);

            // Comprobar que el mensaje se envia sin erro
            if(send_bytes == -1) {
                cerr << chan.error("Error al enviar datos");
                // Cerramos los sockets
                chan.Close(client_fd);
                exit(1);
            }
        }
        // En caso de recibir mensaje de maximo envia el maximo y cierra el canal
        else if(bufferPorComas[0] == "max")
        {
            // Obtener el maximo de la fila estudiante y enviarlo al monitor de datos
            numMax = stoi(bufferPorComas[1]);
            mDatos.enviarMax(filaEstudiante, numMax);

            // Salir del bucle
            out = true;
        }
        // En caso de recibir mensaje de suma coge maximo y cierra el canal   
        else if(bufferPorComas[0] == "suma") 
        {
            // Obtener la suma de la fila estudiante y recoger informacion de la pareja
            suma = stoi(bufferPorComas[1]);
            numMax = mDatos.cogerMax(filaEstudiante);
            cout << filaEstudiante << " | " <<  parejaEstudiante << "-" << nip << " | " << numMax << " | " << suma << endl;
            mFinal.avisarFin();

            // Salir del bucle
            out = true;
        }
        // En caso de recibir un mensaje no deseado lo comunica con el cliente
        else {
            string s = "Mensaje no esperado";
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

void profesor(MonitorSilla& mSilla, MonitorFinal& mFinal)
{
    for (int fila = 0; fila < N_FIL; fila++)
    {
        // Asigna cada fila a una pareja
        mSilla.asignarInfo(fila);
    }        

    // Esperar a que todos los estudiantes hayan terminado
    mFinal.esperarFin();
}
//-------------------------------------------------------------
int main(int argc,char* argv[]) {
    const int N = 4;
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = 3106; //normalmente será un parámetro de invocación. P.e.: argv[1]

    // Variables de practica 4    
    MonitorSilla mSilla(N_EST);
    MonitorDatos mDatos(N_FIL);
    MonitorFinal mFinal(N_EST);

    thread pProfesor = thread(&profesor, ref(mSilla), ref(mFinal));
    thread pEstudiante[N_EST];

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1) {
        cerr << chan.error("Error en el bind");
        exit(1);
    }

    //listen
    int error_code = chan.Listen();
    if (error_code == -1) {
        cerr << chan.error("Error en el listen");
        // Cerramos el socket
        chan.Close();
        exit(1);
    }

    for (int i=0; i<N_EST; i++) {
        // Accept
        int new_client_fds = chan.Accept();
         
        if(new_client_fds == -1) {
            cerr << chan.error("Error en el accept");
            // Cerramos el socket
            chan.Close();
            exit(1);
        } else {
            //introducir en el vector el cliente y arrancar el thread
            pEstudiante[i] = thread(&estudiante, ref(mSilla), ref(mDatos), ref(mFinal), ref(chan), new_client_fds);
            cout << "Nuevo alumno (" + to_string(i) + ") aceptado" + "\n";
        }
        
    }
    cout << "He acabado" << endl;

    //¿Qué pasa si algún thread acaba inesperadamente?
    pProfesor.join();
    for (int i = 0; i < N_EST; i++)
    {
        pEstudiante[i].join(); //me bloqueo hasta que "P[i]" termine
    }
    // timeControl.join();

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
