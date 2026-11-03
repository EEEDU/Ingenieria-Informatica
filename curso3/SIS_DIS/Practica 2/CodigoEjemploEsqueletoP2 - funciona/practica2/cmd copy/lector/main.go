package main

import (
	"bufio"
	"fmt"
	"os"
	"practica2/cmd/gestor"
	"practica2/ra"
	"strconv"
	"strings"
)

func main() {
	if len(os.Args) != 4 {
		fmt.Println("Uso: go run main.go <pid> <fichero_usuarios> <n>")
		os.Exit(1)
	}
	pid, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Fprintln(os.Stderr, "pid inválido:", os.Args[1])
		os.Exit(1)
	}
	usersFile := os.Args[2]
	n, err := strconv.Atoi(os.Args[3])
	if err != nil {
		fmt.Fprintln(os.Stderr, "n inválido:", os.Args[3])
		os.Exit(1)
	}

	gf := gestor.New(pid)
	ricagr := ra.New(pid, usersFile, ra.Read, n, gf.EscribirFichero)
	ricagr.Barrier()

	scanner := bufio.NewScanner(os.Stdin)
	fmt.Println("Enter para leer, q para salir")
	for i := 0; scanner.Scan(); i++ {
		if strings.TrimSpace(scanner.Text()) == "q" {
			break
		}
		ricagr.PreProtocol()
		contenido := gf.LeerFichero()
		fmt.Printf("lector %d, lectura %d:\n%s", pid, i, contenido)
		ricagr.PostProtocol()
	}

	ricagr.Finish()
	ricagr.Stop()
}
