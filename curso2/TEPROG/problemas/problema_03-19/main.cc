#include "Anagrama.h"
#include "ConjuntoAnagrama.h"

using namespace std;

int main() {
    Anagrama anagrama;
    Anagrama anagrama2("amor");

    anagrama.push("monja");
    anagrama.push("jamon");

    anagrama2.push("roma");
    anagrama2.push("asdf");

    cout << "Cantidad de anagrama 1: " << anagrama.getCantidad() << endl;
    cout << "Cantidad de anagrama 2: " << anagrama2.getCantidad() << endl;

    anagrama2.push("mora");
    cout << "Cantidad de anagrama 2: " << anagrama2.getCantidad() << endl;
    anagrama.push("jmaon");
    anagrama.push("majon");

    vector<Anagrama> conjunto_anagramas = {anagrama, anagrama2};
    ConjuntoAnagrama conjunto(conjunto_anagramas);
    conjunto.listarAnagramas();
}