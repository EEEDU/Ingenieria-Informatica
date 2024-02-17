//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2022
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//         Invocación
//                  ./practica_5_COORDINADOR <PUERTO>
//*****************************************************************

#include "Socket.hpp"
#include "ControlParejas.hpp"
#include <iostream>
#include <thread>
#include <sstream>
#include <vector>

using namespace std;

//-------------------------------------------------------------
// Pre : <s> contiene un string con 1 o mas campos, separados por el caracter en <sep>
// Post : devuelve un vector cuyas componentes son los valores que estaban separados por <sep > en <s>, por orden
vector<string> split(const string &s, char sep) 
{
    vector<string> elems;
    string el;
    istringstream seqEls(s);
    while(getline(seqEls, el, sep)) 
    {
        elems.push_back(el);
    }
    return elems;
}

//-------------------------------------------------------------
void Representante(Socket& chan, int client_fd, ControlParejas& monitor, bool& fin, int puerto)
{
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer, what;
    int rcv_bytes, snd_bytes;
    int miNip, nipPareja, fila;
    vector<string> bufferTroceado;

    //recivimos el nip del estudiante al que representamos
    rcv_bytes = chan.Receive(client_fd,buffer,length);

    if (rcv_bytes == -1)
    {
        cerr << chan.error("Error al recibir nip del Estudiante",errno);
        // Cerramos los sockets
        chan.Close(client_fd);
    }

    //una vez sabemos que el buffer es valido lo troceamos para tener el "what,valor"
    bufferTroceado = split(buffer, ',');

    if(bufferTroceado[0] == "fin")
    {
        //salir del while de accepts
        fin = true;
        Socket chan("localhost", puerto);
        int sfd = chan.Connect(); //sacará al servidor del "Accept"
        chan.Close();
    }
    else if (bufferTroceado[0] == "sentar" && bufferTroceado.size() == 2)
    {
        miNip = stoi(bufferTroceado[1]);

        // esperar por una silla libre
        //< await n_sentados < 2
        //    sentado[n_sentados] = nip; 
        //    sentados++;
        //>
        monitor.sillaLibre(miNip);

        monitor.getDatosCompanyero(miNip,nipPareja,fila);

        //enviamos el nip del compañero y la fila al estudiante que representamos
        buffer = to_string(nipPareja) + "," + to_string(fila); 
        snd_bytes = chan.Send(client_fd, buffer);

        if(miNip < nipPareja)
        {
            //recivimos el maximo del estudiante al que representamos
            rcv_bytes = chan.Receive(client_fd,buffer,length);

            if (rcv_bytes == -1)
            {
                cerr << chan.error("Error al recibir el maximo del Estudiante",errno);
                // Cerramos los sockets
                chan.Close(client_fd);
            }

            //una vez sabemos que el buffer es valido lo troceamos para tener el "what,valor"
            bufferTroceado = split(buffer, ',');

            if(bufferTroceado[0] == "max" && bufferTroceado.size() == 2)
            {
                int max = stoi(bufferTroceado[1]);

                // calcular el maximo e indicar a mi pareja que ya lo tengo
                //< 
                // maximos[mi_fila] = max;
                // encontrado[mi_fila] = true; 
                //>
                monitor.pasarMaximo(fila,max);
            }
            else
            {
                cerr << chan.error("tipo de mensaje no esperado",errno);
                // Cerramos los sockets
                chan.Close(client_fd);
                exit(1);
            }
        }
        else
        {
            //recivimos la suma del estudiante al que representamos
            rcv_bytes = chan.Receive(client_fd,buffer,length);

            if (rcv_bytes == -1)
            {
                cerr << chan.error("Error al recibir la suma del Estudiante",errno);
                // Cerramos los sockets
                chan.Close(client_fd);
            }

            //una vez sabemos que el buffer es valido lo troceamos para tener el "what,valor"
            bufferTroceado = split(buffer, ',');

            if(bufferTroceado[0] == "sum" && bufferTroceado.size() == 2)
            {
                string suma = bufferTroceado[1];

                // coger info de max (de mi pareja)
                //< await encontrado[mi_fila] == true
                //    int max = maximos[mi_fila];
                //>
                int max;
                monitor.getMaximo(fila,max);

                cout << to_string(fila) + "\t| " + to_string(nipPareja) + "-" + to_string(miNip) + "\t| " + to_string(max) + "\t| " + suma + "\n";

                // comunicar finalizacion
                //< termine++; >
                monitor.parejaHaTerminado();
            }
            else
            {
                cerr << chan.error("tipo de mensaje no esperado",errno);
                // Cerramos los sockets
                chan.Close(client_fd);
                exit(1);
            }
        }
    }
    else
    {
        cerr << chan.error("tipo de mensaje no esperado",errno);
        // Cerramos los sockets
        chan.Close(client_fd);
        exit(1);
    }

    //cerramos el canal
    chan.Close(client_fd);
}
//-------------------------------------------------------------
/*
 * La sbr profesor se encarga de indicar los nips de las personas que se sientan en las sillas.
 * luego espera a que terminen todas las parejas para poder dar por finalizado el examen. 
*/
void Profesor (ControlParejas& monitor) 
{
    for(int i=0; i< N_FIL; i++)
    {
        // esperar a que haya dos
        //< await n_sentados == 2
        // comunicar a cada uno su pareja , y la fila que les toca
        //con esto l que estamos haciendo es, meter en el vectord e compañeros el nip de su otro compañero.
        //    companyeros[sentados[0]] = sentados[1];
        //    companyeros[sentados[1]] = sentados[0];
        //    //aqui lo que estamos haciendo es meter en las posiciones correspondeientes a los nips de la pareja el numero de fila a leer.
        //    filasPareja[sentados[0]] = i;
        //    filasPareja[sentados[1]] = i;
        //    n_sentados = 0;
        //>
        monitor.hay2sentados(i);
    }

    // esperar que todos hayan terminado
    //< await termine == N_FIL 
    //    mostrar mensaje de fin
    //>
    monitor.todosHanTermninado();

    cout << "\nPrueba finalizada\n";
}
//-------------------------------------------------------------
int main(int argc,char* argv[]) 
{
    const int N = 4;
    // Puerto donde escucha el proceso servidor
    if(argc != 2)
    {
        cerr << "./practica_5_COORDINADOR <PUERTO>" << endl;
        return 0;
    }

    int SERVER_PORT = atoi(argv[1]);
    vector<thread> cliente;
    vector<int> client_fds;

    //Lanzar el proceso del profesor
    ControlParejas monitor;

    thread th_prof(&Profesor,ref(monitor));

    bool fin = false;

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1)
    {
        cerr << chan.error("Error en el bind",errno);
        exit(1);
    }

    //lisen
    int error_code = chan.Listen();
    if (error_code == -1)
    {
        cerr << chan.error("Error en el listen",errno);
        // Cerramos el socket
        chan.Close();
        exit(1);
    }
    //para desbloquear servidor y terminar

    int conectados = 0;
    while (!fin) 
    {
        conectados++;
        // Accept
        int new_client_fds = chan.Accept();

        if(new_client_fds == -1) {
            cerr << chan.error("Error en el accept",errno);
            // Cerramos el socket
            chan.Close();
            exit(1);
        } else {
            if(!fin)
            {
                //introducir en el vector el cliente y arrancar el thread
                client_fds.push_back(new_client_fds);
                cliente.push_back(thread(&Representante, ref(chan), new_client_fds, ref(monitor), ref(fin), SERVER_PORT));
            }
        }
    }

    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i=0; i<cliente.size(); i++)
    {
        cliente[i].join();
    }
    th_prof.join();

    // Cerramos el socket del servidor
    error_code = chan.Close();
    if (error_code == -1) {
        cerr << chan.error("Error cerrando el socket del servidor",errno);
    }

    // Despedida
    cout << "\nFinal de la ejecucion. Mensaje de fin recivido\n" << endl;

    return error_code;
}
//-------------------------------------------------------------
