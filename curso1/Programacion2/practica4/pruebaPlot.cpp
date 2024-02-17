#include <iostream>

using namespace std;

int main(){

    system("gnuplot -e \"set terminal gif; set style data lines; plot 'tfb.txt'\" > tfb.gif");
  
    return 0;

} 
