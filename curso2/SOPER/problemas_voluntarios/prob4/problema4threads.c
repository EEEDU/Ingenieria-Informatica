#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // atoi, exit
#include <unistd.h>     // fork, write
#include <sys/wait.h>   // wait
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>


int modo_out=1;
int modo_err=1;
int fpipe_out[2], fpipe_err[2];
int fichero_salida, fichero_error;

void cambio_modo_out(int signo){
    modo_out = (modo_out % 3) + 1;
    printf("Cambio de modo salida out a %d\n", modo_out);
}

void cambio_modo_err(int signo){
    modo_err = (modo_err % 3) + 1;
    printf("Cambio de modo salida err a %d\n", modo_err);
}

void *hilo_stdout(void *arg) {
    char buf[1024];
    int n;

    while ((n = read(fpipe_out[0], buf, sizeof(buf))) > 0) {
        if (modo_out == 1 || modo_out == 3)
            write(1, buf, n);
        if (modo_out == 2 || modo_out == 3)
            write(fichero_salida, buf, n);
    }
    return NULL;
}

void *hilo_stderr(void *arg) {
    char buf[1024];
    int n;

    while ((n = read(fpipe_err[0], buf, sizeof(buf))) > 0) {
        if (modo_err == 1 || modo_err == 3)
            write(1, buf, n);
        if (modo_err == 2 || modo_err == 3)
            write(fichero_error, buf, n);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    
    printf ("mi proceso tiene el pid: %d\n", getpid());

    pthread_t th_out, th_err;
    int pid;
    
    pipe(fpipe_out);
    pipe(fpipe_err);

    signal(SIGUSR1, cambio_modo_out);
    signal(SIGUSR2, cambio_modo_err);
    
    fichero_salida = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    fichero_error = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);


    switch (pid = fork())
    {
    case 0:
        close(1);
        dup(fpipe_out[1]);
        
        close(2);
        dup(fpipe_err[1]);

        close(fpipe_out[0]);
        close(fpipe_out[1]);
        close(fpipe_err[0]);
        close(fpipe_err[1]);
        
        execvp(argv[3], &argv[3]);
        exit(1);
    default:
        close(fpipe_out[1]);
        close(fpipe_err[1]);

        pthread_create(&th_out, NULL, hilo_stdout, NULL);
        pthread_create(&th_err, NULL, hilo_stderr, NULL);

        waitpid(pid, NULL, 0);
        
        close(fpipe_out[0]);
        close(fpipe_err[0]);
        close(fichero_salida);
        close(fichero_error);
        pthread_cancel(th_out);
        pthread_cancel(th_err);

        return 0;
    }
}