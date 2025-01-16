//*****************************************************************
// File:   Alumno.cpp
// Author: 816106   
// Date:   noviembre 2024
// Coms:   Alumnos que se conectan al servidor de la practica 5
//                  ./Alumno
//*****************************************************************
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>   
#include <fstream>
#include <sstream>

#include "Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

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
    while (getline(sequenciaElementos, elemento, separador)) {
        elementos.push_back(elemento);
    }
    return elementos;
}

// Pre : <fila > es un indice de fila de <D>
// Post : devuelve el maximo de la fila <fila >
int calcularMax(int D[N_FIL][N_COL], int fila)
{
    int max = 0;
    for (int i = 0; i < N_COL; i++)
    {
        if (D[fila][i] > max) { max = D[fila][i]; }
    }
    return max;
}

// Pre : <fila > es un ´ı ndice de fila de <D>
// Post : devuelve la suma de los els . de la fila <fila >
int calcularSuma(int D[N_FIL][N_COL], int fila)
{
    int sum = 0;
    for (int i = 0; i < N_COL; i++)
    {
        sum += D[fila][i];
    }
    return sum;
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

// ----------------------------------------------------
// Pre : Escucha el canal
// Post : Devuelve el mensaje de vuelta que deberia ser nipPareja,Fila
string recibirInfoParejaFila(Socket &chan, int socket_fd)
{
    int read_bytes;  //num de bytes recibidos en un mensaje
    string buffer;  // Buffer para almacenar la respuesta

    // Recibimos la respuesta del servidor
    read_bytes = chan.Receive(socket_fd, buffer, MESSAGE_SIZE);

    // Comprobamos que se ha recibido bien el mensaje
    if (read_bytes == -1) {
        cerr << chan.error("Error al recibir datos");
        // Cerramos los sockets
        chan.Close(socket_fd);
    }

    cout << "Mensaje recibido: " << buffer << endl;
    
    return buffer;
}

void enviarValor(Socket &chan, int socket_fd, string nipPareja_Fila, int nip, int D[N_FIL][N_COL]){
    int send_bytes;  //num de bytes enviados en un mensaje
    string mensajeConValor;  //mensaje de vuelta

    // Separar el mensaje recibido por nip de la pareja y la fila
    vector<string> mensajePorComas = split(nipPareja_Fila, ',');
    int nipPareja = stoi(mensajePorComas[0]);
    int fila = stoi(mensajePorComas[1]);

    if (nip < nipPareja) {
        // Obtener el maximo de la fila estudiante 
        int numMax = calcularMax(D, fila);

        // Concatenar el mensaje
        mensajeConValor = "max," + to_string(numMax);
    }

    else {
        // Obtener la suma de la fila estudiante
        int suma = calcularSuma(D, fila);

        // Concatenar el mensaje
        mensajeConValor = "suma," + to_string(suma);
    }

    // Enviar mensaje con valor
    send_bytes = chan.Send(socket_fd, mensajeConValor);
    // Comprobar que se envia el mensaje
    if(send_bytes == -1) {
        cerr << chan.error("Error al enviar datos");
        // Cerramos los sockets
        chan.Close(socket_fd);
        exit(1);
    }
    cout << "Mensaje enviado: " << mensajeConValor << endl;
}


int alumnos(int nip) {
    int D[N_FIL][N_COL]; // para almacenar los datos
    string nombreArchivo = "datos.txt";

    // Leemos los datos del archivo de texto en la matriz D[][]
    leerDatos(nombreArchivo, D);    

    const string MENS_FIN = "END OF SERVICE";
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = "localhost"; //alternativamente "127.0.0.1"
    int SERVER_PORT = 3106; // mi nip es 816106

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_fd;
    do {
        // Conexión con el servidor
        socket_fd = chan.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1) {
        return socket_fd;
    }

    
    string mensaje;  // Mensaje inicial, tiene que ser sentar,nip para que avance
    vector<string> mensajePorComas;  // Vector con el mensaje separado por comas
    int send_bytes;  //num de bytes enviados en un mensaje
    int read_bytes;  //num de bytes recibidos en un mensaje
    // bucle de envío de peticiones
    do {
        // Leer mensaje de la entrada estandar
        // do {
        //     cout << "Mensaje para enviar: ";
        //     getline(cin, mensaje);
        // } while (mensaje == ""); //no se puede enviar mensaje vacío
        mensaje = "sentar," + to_string(nip);
        
        // Enviamos el mensaje
        send_bytes = chan.Send(socket_fd, mensaje);
        // Lo separamos por comas para saber si hemos enviado mensaje para sentarse
        mensajePorComas = split(mensaje, ',');

        // Comprueba que se ha enviado bien el mensaje
        if(send_bytes == -1) {
            cerr << chan.error("Error al enviar datos");
            // Cerramos el socket
            chan.Close();
            exit(1);
        }
        
        // Si no se ha enviado mensaje de sentar, ni menaje final, recibimos mensaje porque no era lo que esperaba
        if(mensajePorComas[0] != "sentar" && mensaje != MENS_FIN) {
            // Buffer para almacenar la respuesta
            string buffer;

            // Recibimos la respuesta del servidor
            read_bytes = chan.Receive(socket_fd, buffer, MESSAGE_SIZE);

            // Mostramos la respuesta
            cout << "Mensaje que habiamos enviado: '" << mensaje << "'" << endl;
            cout << "*** " << buffer << "***" << endl;
        }
    // El bucle sigue hasta que se envie mensaje de sentar o mensaje final
    } while(mensajePorComas[0] != "sentar" && mensaje != MENS_FIN);

    // Una vez que envia el mensaje de sentar ya pasa a recivir y enviar informacion especifica
    if (mensajePorComas[0] == "sentar"){
        // Obtener el nip del estudiante
        int nip = stoi(mensajePorComas[1]);
        // Recibe informacion del nip de la pareja y la fila
        string nipPareja_Fila = recibirInfoParejaFila(chan, socket_fd);
        // Envia la informacion segun sea el caso (maximo o suma)
        enviarValor(chan, socket_fd, nipPareja_Fila, nip, D);
    }

    // Cerramos el socket
    int error_code = chan.Close();
    if(error_code == -1) {
        cerr << chan.error("Error cerrando el socket");
    }

    // Despedida
    cout << "Bye bye" << endl;

    return error_code;
}

int main(int argc,char* argv[]) {
    thread pEstudiante[N_EST];

    for (int i=0; i<N_EST; i++) { 
        pEstudiante[i] = thread(&alumnos, i);
    }

    for (int i = 0; i < N_EST; i++)
    {
        pEstudiante[i].join(); //me bloqueo hasta que "P[i]" termine
    }
    cout << " Prueba finalizada \n";
    return 0;
    
}