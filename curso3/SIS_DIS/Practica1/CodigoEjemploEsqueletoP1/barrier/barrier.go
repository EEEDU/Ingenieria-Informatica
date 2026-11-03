package main

import (
	"bufio"
	"errors"
	"fmt"
	"net"
	"os"
	"strconv"
	"sync"
	"time"
)

// Leer fichero de endopoints
// Recibe un fichero, va leyendo linea por linea y conviritiendolo un un array de strings
// Devuelve el array y un error en caso de que algo no haya ido bien
func readEndpoints(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close() // Cierra el fichero cuando termine la funcion

	var endpoints []string // Declara una cadena vacia
	scanner := bufio.NewScanner(file)
	for scanner.Scan() { // Recorre fichero por linea
		line := scanner.Text()
		if line != "" {
			endpoints = append(endpoints, line) // Lo añade a la función
		}
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return endpoints, nil
}

// lee el mensaje (el número de proceso que avisa) y entra en la sección crítica, protegida por el mutex
func handleConnection(conn net.Conn, barrierChan chan<- bool,
	received *map[string]bool, mu *sync.Mutex, n int) {
	defer conn.Close()            // Al terminar funcion cierra conexion
	buf := make([]byte, 1024)     // Reserva hueco de 1024 bytes y espear que el otro proces envie algo
	nBytes, err := conn.Read(buf) // lee dle buffer
	if err != nil {
		fmt.Println("Error reading from connection:", err)
		return
	}
	msg := string(buf[:nBytes]) // convierte bufer en texto

	// Seccion critica
	mu.Lock()
	(*received)[msg] = true //apunta que ese proceso ha avisado
	fmt.Println("Received ", len(*received), " elements")
	if len(*received) == n-1 { // Si ya han avisado el resto de procesos
		barrierChan <- true // anuncia al resto de programa que se levanta la barrear
	}
	mu.Unlock()
}

// Get enpoints (IP adresse:port for each distributed process)
func getEndpoints() ([]string, int, error) {
	endpointsFile := os.Args[1]
	var endpoints []string                      // Por qué esta declaración ?  Se instancia una lista de endpoints vacia
	lineNumber, err := strconv.Atoi(os.Args[2]) // Coge el segundo argument lo convierte en entero
	if err != nil || lineNumber < 1 {
		fmt.Println("Invalid line number")
		if err == nil {
			err = errors.New("line number must be 1 or greater")
		}
	} else if endpoints, err = readEndpoints(endpointsFile); err != nil {
		fmt.Println("Error reading endpoints:", err)
	} else if lineNumber > len(endpoints) {
		fmt.Printf("Line number %d out of range\n", lineNumber)
		err = errors.New("Line number out of range")
	}

	return endpoints, lineNumber, err
}

// Hace un bucle aceptando conexiones y lanza handleConnection en una goroutine por cada una
func acceptAndHandleConnections(listener net.Listener, quitChannel chan bool,
	barrierChan chan bool, receivedMap *map[string]bool, mu *sync.Mutex, n int) {
	for {
		select { // Select pero para canales
		case <-quitChannel: // Hay valor en quitChael
			fmt.Println("Stopping the listener...")
			break
		default: // Sino
			conn, err := listener.Accept() // Espera conexion ya establecida
			if err != nil {
				fmt.Println("Error accepting connection:", err)
				continue
			}
			go handleConnection(conn, barrierChan, receivedMap, mu, n) // Atiende conexion en otra goroutine y bucle se queda libre
		}
	}
}

// cliente. Para cada endpoint que no sea el tuyo lanza una goroutine que intenta conectarse por TCP
func notifyOtherDistributedProcesses(endPoints []string, lineNumber int) {
	for i, ep := range endPoints {
		if i+1 != lineNumber {
			go func(ep string) {
				for {
					conn, err := net.Dial("tcp", ep)
					if err != nil {
						fmt.Println("Error connecting to", ep, ":", err)
						time.Sleep(1 * time.Second)
						continue
					}
					_, err = conn.Write([]byte(strconv.Itoa(lineNumber)))
					if err != nil {
						fmt.Println("Error sending message:", err)
						conn.Close()
						continue
					}
					conn.Close()
					break
				}
			}(ep)
		}
	}
}

// arg[1] = endpointsFile
// arg[2] = que proceso (dentroi de endpointsFile) es el tuyo
func main() {
	var listener net.Listener

	// Comprobar que tiene 3 argumentos
	if len(os.Args) != 3 {
		fmt.Println("Usage: go run main.go <endpoints_file> <line_number>")
		// Si esta bien ocge los endpoinnts
	} else if endPoints, lineNumber, err := getEndpoints(); err == nil {
		// Get the endpoint for current process
		localEndpoint := endPoints[lineNumber-1]

		// Crear un socket, lo lanza y lo pone en escucla
		if listener, err = net.Listen("tcp", localEndpoint); err != nil {
			fmt.Println("Error creating listener:", err)
		} else {
			fmt.Println("Listening on", localEndpoint)

			// Barrier synchronization
			// Las cuatro piezas compartidas
			var mu sync.Mutex                    // cerrojo de exclusión mutua
			quitChannel := make(chan bool)       // Canales para comunicar con otros procesos: Deja de aceptar conexiones (se bloquea hasta que otra goroutine lo recibe)
			receivedMap := make(map[string]bool) // registro de quien le ha avisado, se usa map y no contador por si un proceso avisa dos veces
			barrierChan := make(chan bool)       // Canales para comunicar con otros procesos: Deja de aceptar conexiones (se bloquea hasta que otra goroutine lo recibe)

			go acceptAndHandleConnections(listener, quitChannel, barrierChan,
				&receivedMap, &mu, len(endPoints))

			notifyOtherDistributedProcesses(endPoints, lineNumber)

			fmt.Println("Waiting for all the processes to reach the barrier")

			<-barrierChan // Se queda bloqueado aquí hasta que handleConnection envíe 'true'

			fmt.Println("¡Barrera superada por todos!")
			quitChannel <- true // Avisamos al listener para que pare el bucle

			listener.Close()
		}
	}
}
