#include "practica3.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	// Productos estandar

	Contenedor<Carga>* contenedor_estandar = new Contenedor<Carga>(4);

	Producto* longaniza_graus = new Producto("Longaniza de Graus",0.5,0.8);
	contenedor_estandar->guardar(longaniza_graus);

	Producto* adoquines_zgz = new Producto("Adoquines de Zaragoza",0.1,5.0);
	contenedor_estandar->guardar(adoquines_zgz);

	// Productos especiales
	Toxico* discos_melendi = new Toxico("Discos de Melendi",1,10);
	Contenedor<Toxico>*	contenedor_toxico = new Contenedor<Toxico>(3);
	contenedor_toxico->guardar(discos_melendi);

	// Esto no deberia compilar
	// contenedor_estandar->guardar(discos_melendi);

	SerVivo* elvis_presley = new SerVivo("Elvis Presley",0.1,100);
	Contenedor<SerVivo>* contenedor_elvis = new Contenedor<SerVivo>(1);
	contenedor_elvis->guardar(elvis_presley);

	SerVivo* frogosaurio = new SerVivo("Frogosaurio",5,1000);
	Contenedor<SerVivo>* contenedor_frogo = new Contenedor<SerVivo>(8);
	contenedor_frogo->guardar(frogosaurio);

	// Contenedores especiales dentro de contenedor estándar
	Contenedor<Carga>* contenedor_bichos = new Contenedor<Carga>(10);
	contenedor_bichos->guardar(contenedor_elvis);
	contenedor_bichos->guardar(contenedor_frogo);

	Camion camion(20);

	// Esto no deberia compilar
	// camion.guardar(elvis_presley);

	if (!camion.guardar(contenedor_estandar))
		cout << "Camion lleno con contenedor estandar" << endl;
	if (!camion.guardar(contenedor_toxico))
		cout << "Camion lleno con contenedor toxico" << endl;
	if (!camion.guardar(contenedor_bichos))
		cout << "Camion lleno con contenedor de seres vivos" << endl;

	Producto* apuntes_tepro = new Producto("Apuntes de TePro",0.5,0.1);
	if (!camion.guardar(apuntes_tepro))
		cout << "Camion lleno con "  << apuntes_tepro->nombre()<< endl;

	Producto* trenzas_almudevar = new Producto("Trenzas de Almudevar",3.5,50);
	if (!camion.guardar(trenzas_almudevar))
		cout << "Camion lleno con " << trenzas_almudevar->nombre() << endl;

	cout << endl;
	cout << camion << endl;

/*
La salida del programa deberia ser parecida a la siguiente:

> ./main
Camion lleno con Trenzas de Almudevar

Camion [20.0 m3] [1115.9 kg]
  Contenedor de Carga Estandar [4.0 m3] [5.8 kg]
    Longaniza de Graus [0.5 m3] [0.8 kg]
    Adoquines de Zaragoza [0.1 m3] [5.0 kg]
  Contenedor de Productos Toxicos [3.0 m3] [10.0 kg]
    Discos de Melendi [1.0 m3] [10.0 kg]
  Contenedor de Carga Estandar [10.0 m3] [1100.0 kg]
    Contenedor de Seres Vivos [1.0 m3] [100.0 kg]
      Elvis Presley [0.1 m3] [100.0 kg]
    Contenedor de Seres Vivos [8.0 m3] [1000.0 kg]
      Frogosaurio [5.0 m3] [1000.0 kg]
  Apuntes de TePro [0.5 m3] [0.1 kg]
*/

	return 0;
}

