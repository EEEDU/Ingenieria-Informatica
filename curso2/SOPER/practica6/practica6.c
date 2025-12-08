#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(){
    int to_p2[2], to_p1[2], pelota = 0;
    pipe(to_p2);
    pipe(to_p1);
    char buf[256];
    switch (fork()) {
    case 0:
        printf("Iniciar el hijo: %d del padre: %d\n", getpid(), getppid());
        close(0);
        dup(to_p2[0]);
        close(to_p2[0]);
        close(to_p2[1]);
        close(1);
        dup(to_p1[1]);
        close(to_p1[0]);
        close(to_p1[1]);
        while(pelota<100){
            pelota = read(0, buf, sizeof(buf));
            printf("Soy el hijo: %d, leo la pelota con el contador %d\n", getpid(), pelota);
            sleep(1);
            pelota++;
            write(1, buf, pelota);
            printf("Soy el hijo: %d, escribo la pelota con el contador %d\n", getpid(), pelota);
        }
    default:
        printf("Iniciar el padre: %d \n", getpid());
        close(0);
        dup(to_p2[1]);
        close(to_p2[0]);
        close(to_p2[1]);
        close(1);
        dup(to_p1[0]);
        close(to_p1[0]);
        close(to_p1[1]);
        write(1, buf, pelota);
        while (pelota<100)
        {
            pelota = read(0, buf, sizeof(buf));
            printf("Soy el padre: %d, leo la pelota con el contador %d\n", getpid(), pelota);
            sleep(1);
            pelota++;
            write(1, buf, pelota);
            printf("Soy el padre: %d, escribo la pelota con el contador %d\n", getpid(), pelota);

        }
    }

}