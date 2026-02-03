#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // atoi, exit
#include <unistd.h>     // fork, write
#include <sys/wait.h>   // wait
#include <signal.h>
#include <fcntl.h>
#include <string.h>


int max_parametros = 100;
int max_linea = 1000;

int leer_linea_comando(int fildes, char* linea){
    char c;
    int i = 0;
    int readbytes = read(fildes, &c, 1);
    while(readbytes != 0 && c != '\n'){
        linea[i] = c;
        readbytes = read(fildes, &c, 1);
        i++;
    }
    linea[i] = '\0';
    return i;
}

int main(int argc, char *argv[]){
    int fd[2], fichero_control, i;
    pipe(fd);
    char fichero_entrada[max_linea], fichero_salida[max_linea], fichero_errores[max_linea], comando1[max_linea], comando2[max_linea];
    char *argt1[max_parametros], *argt2[max_parametros];

    if (argc < 2){
        printf("Error, tiene que haber al menos un parametros, que sera el nombre de fichero de control\n");
        exit(1);
    }
    fichero_control = open(argv[1], O_RDONLY);

    leer_linea_comando(fichero_control, comando1);
    leer_linea_comando(fichero_control, comando2);
    leer_linea_comando(fichero_control, fichero_entrada);
    leer_linea_comando(fichero_control, fichero_salida);
    leer_linea_comando(fichero_control, fichero_errores);

    argt1[0] = strtok(comando1, " ");
    i = 0;
    while(argt1[i] != NULL){
        i++;
        argt1[i] = strtok(NULL, " ");
    }
    argt1[i] = NULL;

    argt2[0] = strtok(comando2, " ");
    i = 0;
    while(argt2[i] != NULL){
        i++;
        argt2[i] = strtok(NULL, " ");
    }
    argt2[i] = NULL;

    if (fork() == 0){
        close(0);
        dup(fd[0]);

        close(1);
        open(fichero_salida, O_WRONLY | O_CREAT | O_TRUNC, 0666);

        close(2);
        open(fichero_errores, O_WRONLY | O_CREAT | O_TRUNC, 0666);

        close(fd[0]);
        close(fd[1]);
        
        execvp(argt2[0], &argt2[0]);
        exit(1);
    }
    else{
        close(1);
        dup(fd[1]);

        close(0);
        open(fichero_entrada, O_RDONLY);

        close(2);
        open(fichero_errores, O_WRONLY | O_CREAT | O_TRUNC, 0666);

        close(fd[0]);
        close(fd[1]);

        execvp(argt1[0], &argt1[0]);
        exit(1);
    }

}