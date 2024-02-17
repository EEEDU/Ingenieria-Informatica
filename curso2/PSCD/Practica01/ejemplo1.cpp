#include <iostream>
#include <thread>
using namespace std;

const int N_VECES = 100000;

float x = 0;

void inc_1(float &x)
{
    for (int i = 1; i <= N_VECES; i++)
    {
        x = x + 1;
    }
}

void inc_2(float &x)
{
    for (int i = 1; i <= N_VECES; i++)
    {
        x = x + 2;
    }
}

int main()
{
    thread P, Q;
    P = thread(&inc_1, ref(x));
    Q = thread(&inc_2, ref(x));
    P.join();
    Q.join();
    cout << "Valor de x: " << x << endl;
    return 0;
}