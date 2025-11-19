/******************************************************************************
 * Fichero: main.cpp
 * Autores: Daniel Ubalde (875396)
 *          Eduardo Guerrero (816106)
 *
 * Asignatura: Estructuras de Datos y Algoritmos
 ********************************************************************************/

#include <fstream>
#include <iostream>
#include "evento.hpp"
#include "colecInterdep.hpp"

using namespace std;

struct eventData {
  string id;
  string desc;
  string prio;
  string type;
  string id_super;
};

void LD(ostream &out, colecInterdep<string, Evento> &c, const string &id) {

  string i_id, i_id_super;
  Evento e;
  unsigned int num_depend;
  unsigned int cont = 1;

  out << "****DEPENDIENTES: " << id << "\n";

  if(obtenerElemento(id, c, e, i_id_super, num_depend)) {
    if (i_id_super != "") {
      out << "[ " << id << " -de-> " << i_id_super << " ;;; " << num_depend << " ] --- "
          << descripcion(e) << " --- ( " << suPrioridad(e) << " ) ****\n";
    } else {
      out << "[ " << id << " --- " << num_depend << " ] --- "
          << descripcion(e) << " --- ( " << suPrioridad(e) << " ) ****\n";
    }

    iniciarIterador(c);

    while (existeSiguiente(c)) {
      siguienteIdent(c, i_id);
      siguienteVal(c, e);

      if (siguienteNumDependientes(c, num_depend) && siguienteSuperior(c, i_id_super) && id == i_id_super) {
        out << "[" << cont << " -> " << i_id << " -de-> " << id
            << " ;;; " << num_depend << " ] --- " << descripcion(e)
            << " --- ( " << suPrioridad(e) << " ) ;;;;\n";
        cont++;
      }

      avanza(c);
    }

    out << "****FINAL dependientes -de-> " << id << "\n";

  } else {
    out << "****DESCONOCIDO\n";
  }
}

void LT(ostream &out, colecInterdep<string, Evento> &c) {

  string id;
  Evento e;
  string id_super;
  unsigned int num_depend;
  unsigned int c_tamanio = tamanio(c);

  out << "-----LISTADO: " << c_tamanio << "\n";

  iniciarIterador(c);

  while (existeSiguiente(c)) {
    siguienteIdent(c, id);
    siguienteVal(c, e);
    if (siguienteNumDependientes(c, num_depend) && siguienteSuperior(c, id_super)) {
      out << "[ " << id << " -de-> " << id_super << " ;;; " << num_depend << " ] --- "
          << descripcion(e) << " --- ( " << suPrioridad(e) << " )\n";
    } else {
      out << "[ " << id << " --- " << num_depend << " ] --- "
          << descripcion(e) << " --- ( " << suPrioridad(e) << " )\n";
    }

    avanza(c);
  }
  out << "-----\n";
}

void clearData(struct eventData &data) {
  data.id = "";
  data.desc = "";
  data.prio = "";
  data.type = "";
  data.id_super = "";
}

int main(int argc, char const *argv[]) {

  ifstream f_in;
  ofstream f_out;
  string instruccion;
  string salto;
  Evento ev;
  eventData data;
  colecInterdep<string, Evento> eventsColl;
  unsigned int num_depend, tamanio_a, tamanio_d;
  bool event_type;

  if (argc == 2) {
    f_out.open(argv[1]);
  } else {
    f_out.open("salida.txt");
  }

  f_in.open("entrada.txt");
  crear(eventsColl);

  while (f_in >> instruccion) {
    getline(f_in, salto);
    if (instruccion == "A") {

      clearData(data);
      getline(f_in, data.id);
      getline(f_in, data.desc);
      getline(f_in, data.prio);
      getline(f_in, data.type);
      getline(f_in, data.id_super);

      if ((data.type == "INDependiente" && data.id_super != "-.-.-.-.-") ||
          (data.type == "DEPendiente" && data.id_super == "-.-.-.-.-")) {
        cerr << "Entrada mal formada 'type & id_super'\n";
        exit(1);
      }

      crearEvento(data.desc, stoi(data.prio), ev);

      if (data.type == "INDependiente") {

        tamanio_a = tamanio(eventsColl);
        aniadirIndependiente(eventsColl, data.id, ev);
        tamanio_d = tamanio(eventsColl);

        if (tamanio_a != tamanio_d) {
          f_out << "INTRODUCIDO: [ " << data.id << " ] --- " << data.desc << " --- ( " << data.prio << " )\n";
        } else {
          f_out << "NO INTRODUCIDO: [ " << data.id << " ] --- " << data.desc << " --- ( " << data.prio << " )\n";
        }
      } else if (data.type == "DEPendiente") {

        tamanio_a = tamanio(eventsColl);
        aniadirDependiente(eventsColl, data.id, ev, data.id_super);
        tamanio_d = tamanio(eventsColl);

        if (tamanio_a != tamanio_d) {
          f_out << "INTRODUCIDO: [ " << data.id << " -de-> " << data.id_super << " ] --- "
                << data.desc << " --- ( " << data.prio << " )\n";
        } else {
          f_out << "NO INTRODUCIDO: [ " << data.id << " -de-> " << data.id_super << " ] --- "
                << data.desc << " --- ( " << data.prio << " )\n";
        }
      } else {
        cerr << "Entrada mal formada 'type' no reconocido\n";
        exit(1);
      }

    } else if (instruccion == "C") {

      clearData(data);
      getline(f_in, data.id);
      getline(f_in, data.desc);
      getline(f_in, data.prio);

      crearEvento(data.desc, stoi(data.prio), ev);

      if (actualizarVal(eventsColl, data.id, ev)) {
        if (obtenerElemento(data.id, eventsColl, ev, data.id_super, num_depend)) {
          if (data.id_super != "") {
            f_out << "CAMBIADO: [ " << data.id << " -de-> " << data.id_super << " ;;; "
                  << num_depend << " ] --- " << data.desc << " --- ( " << data.prio << " )\n";
          } else {
            f_out << "CAMBIADO: [ " << data.id << " --- " << num_depend << " ] --- "
                  << data.desc << " --- ( " << data.prio << " )\n";
          }
        } else {
          cerr << "ERROR: obtenerElemento - debería existir\n";
          exit(1);
        }
      } else {
  f_out << "NO CAMBIADO: " << data.id << "\n";
      }

    } else if (instruccion == "D") {

      clearData(data);
      getline(f_in, data.id);
      getline(f_in, data.id_super);

      // TODO: Mirar si se puede optimizar
      if (existe(data.id, eventsColl) && existe(data.id_super, eventsColl)) {
        f_out << "INTENTANDO hacer depend.: " << data.id << " -de-> " << data.id_super << "\n";
      } else {
        f_out << "IMPOSIBLE hacer depend.: " << data.id << " -de-> " << data.id_super << "\n";
      }
      hacerDependiente(eventsColl, data.id, data.id_super);

    } else if (instruccion == "O") {
      clearData(data);
      getline(f_in, data.id);

      if (obtenerElemento(data.id, eventsColl, ev, data.id_super, num_depend)) {
        if (data.id_super != "") {
          f_out << "LOCALIZADO: [ " << data.id << " -de-> " << data.id_super << " ;;; "
                << num_depend << " ] --- " << descripcion(ev) << " --- ( " << suPrioridad(ev) << " )\n";
        } else {
          f_out << "LOCALIZADO: [ " << data.id << " --- " << num_depend << " ] --- "
                << descripcion(ev) << " --- ( " << suPrioridad(ev) << " )\n";
        }
      } else {
        f_out << "NO LOCALIZADO: " << data.id << "\n";
      }
    } else if (instruccion == "E") {

      clearData(data);
      getline(f_in, data.id);

      if (existeDependencia(data.id, eventsColl, event_type)) {
        if (event_type) {
          f_out << "DEPendiente: " << data.id << "\n";
        } else {
          f_out << "INDEPendiente: " << data.id << "\n";
        }
      } else {
        f_out << "DESCONOCIDO: " << data.id << "\n";
      }

    } else if (instruccion == "I") {

      clearData(data);
      getline(f_in, data.id);

      if (existeDependencia(data.id, eventsColl, event_type)) {
        if (event_type) {
          f_out << "INDEPENDIZADO: " << data.id << "\n";
        } else {
          f_out << "YA ERA INDepend.: " << data.id << "\n";
        }
      } else {
        f_out << "NO INDEPENDIZADO: " << data.id << "\n";
      }
      hacerIndependiente(eventsColl, data.id);

    } else if (instruccion == "B") {

      clearData(data);
      getline(f_in, data.id);

      tamanio_a = tamanio(eventsColl);
      borrar(data.id, eventsColl);
      tamanio_d = tamanio(eventsColl);

      if (tamanio_a != tamanio_d) {
        f_out << "BORRADO: " << data.id << "\n";
      } else {
        f_out << "NO BORRADO: " << data.id << "\n";
      }

    } else if (instruccion == "LD") {

      clearData(data);
      getline(f_in, data.id_super);

      LD(f_out, eventsColl, data.id_super);
    } else if (instruccion == "LT") {
      LT(f_out, eventsColl);
    }
  }

  return 0;
}
