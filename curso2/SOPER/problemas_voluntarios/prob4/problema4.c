#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // atoi, exit
#include <unistd.h>     // fork, write
#include <sys/wait.h>   // wait
#include <signal.h>
#include <fcntl.h>
#include <string.h>

int modo_out=1;
int modo_err=1;

void cambio_modo_out(int signo){
    modo_out = (modo_out % 3) + 1;
    printf("Cambio de modo salida out a %d\n", modo_out);
}

void cambio_modo_err(int signo){
    modo_err = (modo_err % 3) + 1;
    printf("Cambio de modo salida err a %d\n", modo_err);
}

int main(int argc, char *argv[]){
    char buffer_out[1000], buffer_err[1000];
    int fpipe_out[2], fpipe_err[2], fichero_salida, fichero_error, readbyte_out, readbyte_err;
    pipe(fpipe_out);
    pipe(fpipe_err);
    signal(SIGUSR1, cambio_modo_out);
    signal(SIGUSR2, cambio_modo_err);
    printf ("mi proceso tiene el pid: %d\n", getpid());

    switch (fork())
    {
    case 0:

        switch (fork())
        {
        case 0:
            close(fpipe_out[0]);
            close(fpipe_out[1]);
            close(fpipe_err[1]);
            fichero_error = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            readbyte_err = read(fpipe_err[0], &buffer_err, 1000);
            while (readbyte_err != 0)
            {
                if (modo_err == 1 || modo_err == 3){
                    write(1, &buffer_err, readbyte_err);
                }
                if (modo_err == 2 || modo_err == 3){
                    write(fichero_error, &buffer_err, readbyte_err);
                }
                readbyte_err = read(fpipe_err[0], &buffer_err, 1000);
            }
        
        default:
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
        }
    default:
        close(fpipe_out[1]);
        fichero_salida = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        readbyte_out = read(fpipe_out[0], &buffer_out, 1000);
        while (readbyte_out != 0)
        {
            if (modo_out == 1 || modo_out == 3){
                write(1, &buffer_out, readbyte_out);
                // write(fichero_salida, &buffer, readbyte);

            }
            if (modo_out == 2 || modo_out == 3){
                write(fichero_salida, &buffer_out, readbyte_out);
            }
            readbyte_out = read(fpipe_out[0], &buffer_out, 1000);
        }
        
    }
    exit(0);
}