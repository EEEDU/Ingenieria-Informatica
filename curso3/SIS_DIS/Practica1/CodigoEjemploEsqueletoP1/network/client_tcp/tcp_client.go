package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"
)

func measureTCPDial(serverAddr string) {
	// Measure time for unsuccessful connection (server not running)
	fmt.Println("Attempting to connect to server (expecting failure)...")
	start := time.Now()                      // Empieza cronometro de cuanto tarda en conectar
	conn, err := net.Dial("tcp", serverAddr) //intenta conectarse al servidor. El programa se queda esperando hasta que tenga respuesta (buena o mala)
	duration := time.Since(start)            // Para el cronometro
	if err != nil {
		fmt.Printf("Failed to connect: %v\n", err)
	} else {
		fmt.Println("Connected successfully")
		conn.Close()
	}
	fmt.Printf("Failed Dial took: %v\n", duration)

	// Wait for server to start
	// El programa se detiene hasta que pulses enter
	fmt.Println("\nPlease start the server, then press Enter.")
	fmt.Scanln()

	// Measure time for successful connection (server running)
	fmt.Println("Attempting to connect to server again (expecting success)...")
	start = time.Now()
	conn, err = net.Dial("tcp", serverAddr)
	if err != nil {
		log.Fatalf("Failed to connect to server: %v", err)
	}
	duration = time.Since(start)
	fmt.Printf("Dial (with server running) took: %v\n", duration)
	conn.Close()
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run main.go endpoint")
		return
	}
	serverAddr := os.Args[1]
	measureTCPDial(serverAddr)
}
