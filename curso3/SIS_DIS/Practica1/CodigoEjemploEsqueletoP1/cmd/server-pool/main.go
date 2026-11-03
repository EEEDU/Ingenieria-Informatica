/*
* AUTOR: Eduardo Guerrero
* ASIGNATURA: 30221/39521 - Sistemas Distribuidos
*	       Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2026
* FICHERO: server-pool.go
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
	"sync"
)

var numWorkers = 4

// task, tarea que envia el servidor al worker
type task struct {
	conn net.Conn
	req  com.Request
}

// result, resultado que el worker devuelve al servidor
type result struct {
	conn  net.Conn
	reply com.Reply
}

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

// worker, donde se hace el trabajo.
// Coge la tarea, ejecuta la funcion encontrar primos para ese intervalo y lanza el resultado
func worker(tasks <-chan task, results chan<- result, wg *sync.WaitGroup) {
	defer wg.Done()
	for t := range tasks {
		primes := findPrimes(t.req.Interval)
		reply := com.Reply{Id: t.req.Id, Primes: primes}
		results <- result{conn: t.conn, reply: reply}
	}
}

// responder, coge los resultados de la tuberia de resultaos y se los manda a los clientes
// Si ha terminado activa el canal de done
func responder(results <-chan result, done chan<- bool) {
	for r := range results {
		err := gob.NewEncoder(r.conn).Encode(&r.reply)
		if err != nil {
			log.Println("Error sending reply:", err)
		}
		r.conn.Close()
	}
	done <- true
}

// readTask, acepta conexion y lee la peticion del clietne
func readTask(listener net.Listener) (task, error) {
	conn, err := listener.Accept()
	if err != nil {
		return task{}, err
	}
	var req com.Request
	err = gob.NewDecoder(conn).Decode(&req)
	if err != nil {
		conn.Close()
	}
	return task{conn: conn, req: req}, err
}

// serve, bucle que recbie peticiones de los clientes y se las pasa a los workers
func serve(listener net.Listener, tasks chan<- task) {
	for {
		t, err := readTask(listener)
		if err != nil {
			log.Println("Error reading request:", err)
			continue
		}
		if t.req.Id == -1 {
			t.conn.Close()
			return
		}
		tasks <- t
	}
}

// startPool, lanza el numero de workers.
func startPool(tasks chan task, results chan result, wg *sync.WaitGroup, done chan bool) {
	for i := 0; i < numWorkers; i++ {
		wg.Add(1)
		go worker(tasks, results, wg)
	}
	go responder(results, done)
}

// shutdown cierra canales, espera que todos los workers terminen
func shutdown(tasks chan task, results chan result, wg *sync.WaitGroup, done chan bool) {
	close(tasks)
	wg.Wait()
	close(results)
	<-done
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

	// 3 canales
	tasks := make(chan task)     // mandar tareas a workers
	results := make(chan result) //que el woker mande resultado
	done := make(chan bool)      // marcar que el trabajo esta hecho

	var wg sync.WaitGroup // Contador de Goroutinas para saber cuando han termiando todas

	startPool(tasks, results, &wg, done) // inicar workers
	serve(listener, tasks)
	shutdown(tasks, results, &wg, done)
	listener.Close()

}
