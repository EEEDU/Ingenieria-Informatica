#include <iostream>
#include "maxsolape.hpp"
using namespace std;
int main(){
    tpInter indinters[N];
    double inters[N][2] = {{1.5, 8.0},{0.0, 4.5},{2.0, 4.0},{1.0, 6.0},{3.5, 7.0}};
    tpSolape res = maxSolFBruta(inters,5);
    cout << res.interA << " " << res.interB << " " << res.solape << endl;
    crearvind(inters, indinters, 5);
    mergesortIndInters(indinters, 0, 4);
    tpSolape res1 = maxSolDyV(indinters, 0, 4);
    cout << res1.interA << " " << res1.interB << " " << res1.solape << endl;
}