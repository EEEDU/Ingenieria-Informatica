#include <signal.h>

int main(){
    signal(SIGALRM, SIG_DFL);
    alarm(5);
    pause();
}