package main

import (
	"fmt"
	"log"
	"math/rand"
	"os"
	"strconv"
	"time"

	"practica2/cmd/gestor"
	"practica2/ra"
)

const (
	esperaPeers = 120 * time.Second // máximo para que todos los procesos estén escuchando
)

func main() {
	if len(os.Args) != 4 {
		fmt.Fprintf(os.Stderr, "Uso: %s <pid> <fichero_peers> <iteraciones>\n", os.Args[0])
		os.Exit(1)
	}
	pid, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Fprintln(os.Stderr, "el pid debe ser un número")
		os.Exit(1)
	}
	iteraciones, err := strconv.Atoi(os.Args[3])
	if err != nil {
		fmt.Fprintln(os.Stderr, "las iteraciones deben ser un número")
		os.Exit(1)
	}
	log.SetFlags(log.Lshortfile | log.Lmicroseconds)

	g := gestor.New(fmt.Sprintf("fichero_%d.txt", pid))
	r := ra.New(pid, os.Args[2], ra.Write, g.EscribirFichero)

	if err := r.Barrier(esperaPeers); err != nil {
		log.Fatalf("barrera de arranque: %v", err)
	}
	for i := 1; i <= iteraciones; i++ {
		r.PreProtocol()
		log.Printf("ENTRA escritor %d %d", pid, i)

		fragmento := fmt.Sprintf("escritor %d: línea %d\n", pid, i)
		g.EscribirFichero(fragmento)
		r.Update(fragmento)
		log.Printf("escritor %d (iteración %d) escribe %q", pid, i, fragmento)
		log.Printf("SALE escritor %d %d", pid, i)
		r.PostProtocol()
		time.Sleep(time.Duration(rand.Intn(500)) * time.Millisecond)
	}
	r.Finish()
	r.Stop()
}
