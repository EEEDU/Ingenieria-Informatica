/*
* AUTOR: Rafael Tolosana Calasanz y Unai Arronategui
* ASIGNATURA: 30221/39521 - Sistemas Distribuidos
*	       Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2022
* FICHERO: server-draft.go
* DESCRIPCIÓN: contiene la funcionalidad esencial para realizar los servidores
*				correspondientes a la práctica 1
 */
package main

import (
	"encoding/gob"
	"log"
	"net"
	"os"
	"practica1/com"
)

// PRE: verdad = !foundDivisor
// POST: IsPrime devuelve verdad si n es primo y falso en caso contrario
func isPrime(n int) (foundDivisor bool) {
	foundDivisor = false
	for i := 2; (i < n) && !foundDivisor; i++ {
		foundDivisor = (n%i == 0)
	}
	return !foundDivisor
}

// PRE: interval.A < interval.B
// POST: FindPrimes devuelve todos los números primos comprendidos en el
//
//	intervalo [interval.A, interval.B]
func findPrimes(interval com.TPInterval) (primes []int) {
	for i := interval.Min; i <= interval.Max; i++ {
		if isPrime(i) {
			primes = append(primes, i)
		}
	}
	return primes
}

func processRequest(conn net.Conn) {
	var request com.Request
	decoder := gob.NewDecoder(conn) // Decodifica mensaaje
	err := decoder.Decode(&request)
	com.CheckError(err)
	primes := findPrimes(request.Interval)             // funcion de encontrar primos
	reply := com.Reply{Id: request.Id, Primes: primes} // manteniendo el mismo Id de la petición
	encoder := gob.NewEncoder(conn)
	encoder.Encode(&reply)
}

// arg[1] = endpoint
func main() {
	args := os.Args
	if len(args) != 2 {
		log.Println("Error: endpoint missing: go run server.go ip:port")
		os.Exit(1)
	}
	endpoint := args[1]
	listener, err := net.Listen("tcp", endpoint) //escuchar endpoint por tcp
	com.CheckError(err)

	log.SetFlags(log.Lshortfile | log.Lmicroseconds) // Para que todos los mensajes saquen por pantalla shortfile y microseconds.

	log.Println("***** Listening for new connection in endpoint ", endpoint)
	for {
		conn, err := listener.Accept() //Acpeta conexion
		defer conn.Close()             // Al terminar funcion cierra conexion
		com.CheckError(err)            // comprobar error de comunicacion
		processRequest(conn)           // Procesar petici
	}
}
