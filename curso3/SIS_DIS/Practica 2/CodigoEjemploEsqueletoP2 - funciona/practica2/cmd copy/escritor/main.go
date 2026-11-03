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
	pid, _ := strconv.Atoi(os.Args[1]) // convierte int a str
	usersFile := os.Args[2]
	n, _ := strconv.Atoi(os.Args[3])

	gesFichero := gestor.New(pid) // abre o crea fichero_<pid>.txt
	ricagr := ra.New(pid, usersFile, ra.Write, n, gesFichero.EscribirFichero)

	ricagr.Barrier()

	scanner := bufio.NewScanner(os.Stdin)
	fmt.Println("Escribe un texto y Enter para añadirlo. :q para salir")
	for scanner.Scan() {
		texto := strings.TrimSpace(scanner.Text())
		if texto == ":q" {
			break
		}
		if texto == "" {
			continue // ignora líneas vacías
		}
		frag := fmt.Sprintf("[escritor %d] %s\n", pid, texto)
		ricagr.PreProtocol()
		gesFichero.EscribirFichero(frag)
		ricagr.Update(frag)
		ricagr.PostProtocol()
	}
	ricagr.Finish()
	ricagr.Stop()

}
