/*
* AUTOR: Eduardo Guerrero
* ASIGNATURA: 30221/39521 - Sistemas Distribuidos
*	       Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FICHERO: worker/main.go
 */
package main

import (
	"encoding/gob"
	"log"
	"net"
	"os"
	"practica1/com"
)

func isPrime(n int) (foundDivisor bool) {
	foundDivisor = false
	for i := 2; (i < n) && !foundDivisor; i++ {
		foundDivisor = (n%i == 0)
	}
	return !foundDivisor
}

func findPrimes(interval com.TPInterval) (primes []int) {
	for i := interval.Min; i <= interval.Max; i++ {
		if isPrime(i) {
			primes = append(primes, i)
		}
	}
	return primes
}

// sendReply codifica la respuesta en la conexion.
func sendReply(conn net.Conn, reply com.Reply) {
	err := gob.NewEncoder(conn).Encode(&reply)
	if err != nil {
		log.Println("Error sending reply:", err)
	}
}

// handle atiende una solicitud y cierra la conexión.
func handle(conn net.Conn) bool {
	defer conn.Close()
	var req com.Request
	if err := gob.NewDecoder(conn).Decode(&req); err != nil {
		log.Println("Error decoding request:", err)
		return true
	}
	if req.Id == -1 {
		return false
	}
	reply := com.Reply{Id: req.Id, Primes: findPrimes(req.Interval)}
	sendReply(conn, reply)
	return true
}

// listen comprueba arguemtnos de entrada
func listen() net.Listener {
	if len(os.Args) != 2 {
		log.Println("Error: endpoint missing: go run worker.go ip:port")
		os.Exit(1)
	}
	listener, err := net.Listen("tcp", os.Args[1])
	com.CheckError(err)
	log.SetFlags(log.Lshortfile | log.Lmicroseconds)
	log.Println("***** Worker listening in endpoint", os.Args[1])
	return listener
}

// run atiende las solicitudes una a una hasta la ultima
func run(listener net.Listener) {
	for serving := true; serving; {
		conn, err := listener.Accept()
		com.CheckError(err)
		serving = handle(conn)
	}
}

// arg[1] = endpoint
func main() {
	listener := listen()
	run(listener)
	listener.Close()
}
