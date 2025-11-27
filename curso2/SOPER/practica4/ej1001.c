/* ej1001.c
 * Shell elemental con bucle de lectura de comandos con parámetros.
 * Uso: [arre|soo] [comando][lista parametros]
 * arre:  ejecucion asíncrona
 * soo:   ejecucion síncrona
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "error.h"

#define BUFSIZE 1024
#define TRUE     1
#define FALSE    0

int main()
{
    char s[BUFSIZE];		/* vector de BUFSIZE caracteres */
    char *argt[BUFSIZE];	/* vector de BUFSIZE punteros */
    int i, parate, pid;

    while(1) {
        fprintf(stderr, "\n_$ ");
        gets(s);
        // fgets(s, sizeof(s), stdin); s[strlen(s)-1] = 0;

        if (strlen(s) == 0) continue;
        argt[0] = strtok(s, " ");

        if (0 == strcmp(argt[0], "quit")) {
            fprintf(stderr, "logout\n");
            exit(0);
        }

        for (i = 1; 
             (argt[i] = strtok(NULL," \t")) != NULL;
             i++ );

        if (0 == strcmp(argt[0], "soo"))
            parate = TRUE;
        else {
            if (0 == strcmp(argt[0], "arre"))
                parate = FALSE;
            else {
                printf("\n Mande?");
                continue;
            }
        }

        switch(pid = fork()) {
            case -1:    /* error */
                fprintf(stderr, "\nNo se puede crear proceso nuevo\n");
                syserr("fork");
      
            case 0:    /* hijo */
                execvp(argt[1], &argt[1]);
                fprintf(stderr,"\nNo se puede ejecutar %s\n", argt[1]);
                syserr("execvp");
    
            default:    /* padre */
                if (parate)
                    while (pid != wait(NULL));
        } /* switch */
    } /* while */
} /* main */
