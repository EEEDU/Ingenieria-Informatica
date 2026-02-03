#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // atoi, exit
#include <unistd.h>     // fork, write
#include <sys/wait.h>   // wait

int main(int argc, char* argv[]){

    if (argc != 3){
        fprintf(stderr, "ERROR, se tienen que utilizar dos parametros\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    char c[] = "abcdefghijklmnopqrstuvwxyz";
    
    for(int i = 0; i<n; i++){
        if (fork()==0){
            for(int j = 0; j<m; j++){
                write(1, &c[i%26], 1);
            }
            exit(1);
        }
        
    }
    for(int i = 0; i<n; i++){
        wait(NULL);
    }        
    write(1, "\n", 1); // salto de línea opcional
    exit(0);

}