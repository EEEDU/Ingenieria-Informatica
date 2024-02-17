#include <iostream>
using namespace std;
int main()
{
    system("gnuplot -e \"set terminal gif; set style data lines; plot 'tiempoDyV.txt'\" > tiempoDyV.gif");
    system("gnuplot -e \"set terminal gif; set style data lines; plot 'tiempoFB.txt'\" > tiempoFB.gif");
    return 0;
} // Cuidado con las comillas del nombre del archivo!! Tienen que ser las comillas de la ?