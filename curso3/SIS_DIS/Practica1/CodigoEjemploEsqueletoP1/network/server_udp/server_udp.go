package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run server.go <listen-address>")
		return
	}
	listenAddr := os.Args[1]

	udpAddr, err := net.ResolveUDPAddr("udp", listenAddr) //Convierte el argumento en algo que Go entienda
	if err != nil {
		fmt.Println("Error resolving address:", err)
		return
	}

	// Listen on the specified UDP address
	conn, err := net.ListenUDP("udp", udpAddr) // Crea el socket de UDP y asocia el puerto (aqui no hay que aceptar)
	if err != nil {
		fmt.Println("Error starting UDP server:", err)
		return
	}
	defer conn.Close()

	fmt.Println("UDP server is running and waiting for messages...")
	buffer := make([]byte, 1024)
	for {
		n, addr, err := conn.ReadFromUDP(buffer) // Se bloquea hasta que llega un datagrama
		if err != nil {
			fmt.Println("Error receiving message:", err)
			continue
		}

		fmt.Printf("Received '%s' from %s\n", string(buffer[:n]), addr)

		// Echo the message back to the client
		_, err = conn.WriteToUDP(buffer[:n], addr) // Devuelve el mismo contenido que recibio
		if err != nil {
			fmt.Println("Error sending message:", err)
			return
		}
	}
}
