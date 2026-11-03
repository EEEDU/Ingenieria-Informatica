/*
* AUTOR: Rafael Tolosana Calasanz
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingeniería Informática
*			Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2021
* FICHERO: ricart-agrawala.go
* DESCRIPCIÓN: Implementación del algoritmo de Ricart-Agrawala Generalizado en Go
 */
package ra

import (
	"encoding/gob"
	"log"
	"net"
	"practica2/ms"
	"sync"
	"time"
)

type Operation int

const (
	Read  Operation = iota // 0
	Write                  // 1
)

var exclude = [2][2]bool{
	{false, true}, // read-read, read-write
	{true, true},  // write-read, write-write
}

type Request struct {
	Clock     int
	Pid       int
	Operation Operation // operacion de entrada o salida

}

type Reply struct {
	Pid int
}

type Stop struct {
	Pid int // gob no codifica structs sin campos exportados, así que no puede ir vacío
}

type Update struct {
	Pid      int
	Fragment string
}

type Ack struct {
	Pid int
}

type Ready struct {
	Pid int
}

type Done struct {
	Pid int
}

type RASharedDB struct {
	OurSeqNum int               // lrd
	HigSeqNum int               // clock
	OutRepCnt int               // Respues que me faltan por recibir
	ReqCS     bool              // Quiere entrar en SC
	RepDefd   []bool            // Procesos que tenia esperando
	ms        *ms.MessageSystem //Sistema de mensajeria
	done      chan bool         // canal para avisar fin
	chrep     chan bool         // canal para avisar que ya puede entrar en sc
	Mutex     sync.Mutex        // mutex para proteger concurrencia sobre las variables
	// TODO: completar
	me        int                   // mi pid, de 1 a N
	N         int                   // número total de procesos
	operation Operation             // operacion de entrada op salida
	OnUpdate  func(fragment string) // callback de la aplicación: escribe en el fichero local
	chack     chan bool             // aviso de "han llegado todos los Ack" (buffer 1)
	peers     []string              // IP:puerto de cada proceso, leído del fichero de usuarios
	DoneFrom  map[int]bool          // pids de los que he recibido Done
	chdone    chan bool             // aviso de "han acabado todos" (buffer 1)
	AckCnt    int                   // Acks que me faltan por recibir (como OutRepCnt)
}

// Leer fichero de endopoints
// Recibe un fichero, va leyendo linea por linea y conviritiendolo un un array de strings
// Devuelve el array y un error en caso de que algo no haya ido bien
func readPeers(filename string) ([]string, error) {
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

// Crear nuevo RA
// parametros:
//
//		me: pid
//		usersFile: nombre del fichoer de usuarios
//		operation: operación que realiza este proceso (read, write)
//	 numero de procesos
func New(me int, usersFile string, operation Operation, onUpdate func(fragment string)) *RASharedDB {
	messageTypes := []ms.Message{Request{}, Reply{}, Stop{}, Update{}, Ack{}, Ready{}, Done{}}
	msgs := ms.New(me, usersFile, messageTypes)
	peers, err := readPeers(usersFile)
	if err != nil {
		log.Fatalf("no se puede leer %s: %v", usersFile, err)
	}
	n := len(peers)
	ra := RASharedDB{
		me:        me,
		N:         n,
		operation: operation,
		RepDefd:   make([]bool, n+1),
		ms:        &msgs,
		chrep:     make(chan bool, 1),
		chack:     make(chan bool, 1),
		chdone:    make(chan bool, 1),
		OnUpdate:  onUpdate,
		peers:     peers,
		DoneFrom:  make(map[int]bool),
	}
	go receiveLoop(&ra)
	return &ra
}

func receiveLoop(ra *RASharedDB) {
	for {
		msg := ra.ms.Receive()
		if msg == nil {
			return // el buzón se cerró: ms se ha parado
		}
		switch m := msg.(type) {
		case Request:
			// líneas (10)-(14) de la Figura 1
			ra.Mutex.Lock()
			if ra.HigSeqNum < m.Clock {
				ra.HigSeqNum = m.Clock
			}

			responder := !exclude[ra.operation][m.Operation] ||
				!ra.ReqCS ||
				m.Clock < ra.OurSeqNum ||
				(m.Clock == ra.OurSeqNum && m.Pid < ra.me)
			if !responder {
				ra.RepDefd[m.Pid] = true
			}
			ra.Mutex.Unlock()
			if responder {
				// send PERMISSION(i) to pj
				ra.ms.Send(m.Pid, Reply{ra.me})
			}
		case Reply:
			// línea (15)
			ra.Mutex.Lock()
			ra.OutRepCnt--
			todasRecibidas := ra.OutRepCnt == 0
			ra.Mutex.Unlock()
			if todasRecibidas {
				ra.chrep <- true
			}
		case Stop:

		case Update:
			// aplicar el cambio
			if ra.OnUpdate != nil {
				ra.OnUpdate(m.Fragment)
			}
			ra.ms.Send(m.Pid, Ack{ra.me}) // enviar confirmacion
		case Ack:
			ra.Mutex.Lock()
			ra.AckCnt--
			todos := ra.AckCnt == 0
			ra.Mutex.Unlock()
			if todos {
				ra.chack <- true
			}
		case Ready:
		case Done:
			ra.Mutex.Lock()
			todos := false
			if !ra.DoneFrom[m.Pid] {
				ra.DoneFrom[m.Pid] = true
				todos = len(ra.DoneFrom) == ra.N-1
			}
			ra.Mutex.Unlock()
			if todos {
				ra.chdone <- true
			}
		}
	}
}

// Pre: Verdad
// Post: Realiza  el  PreProtocol  para el  algoritmo de
//
//	Ricart-Agrawala Generalizado
func (ra *RASharedDB) PreProtocol() {
	// lineas (1)-(6)
	ra.Mutex.Lock()
	ra.ReqCS = true
	ra.OurSeqNum = ra.HigSeqNum + 1
	ra.OutRepCnt = ra.N - 1
	seq := ra.OurSeqNum
	ra.Mutex.Unlock()
	if ra.N > 1 {
		for i := 1; i <= ra.N; i++ {
			if i != ra.me {
				ra.ms.Send(i, Request{Clock: seq, Pid: ra.me, Operation: ra.operation})
			}
		}
		<-ra.chrep
	}
}

// Pre: Verdad
// Post: Realiza  el  PostProtocol  para el  algoritmo de
//
//	Ricart-Agrawala Generalizado
func (ra *RASharedDB) PostProtocol() {
	// lineas (7)-(9)
	ra.Mutex.Lock()
	ra.ReqCS = false
	var pendientes []int
	for i, defd := range ra.RepDefd {
		if defd {
			pendientes = append(pendientes, i)
			ra.RepDefd[i] = false
		}
	}
	ra.Mutex.Unlock()
	for _, i := range pendientes {
		ra.ms.Send(i, Reply{ra.me})
	}
}

func (ra *RASharedDB) Stop() {
	stopped := make(chan struct{})

	// Pedir a ms que pare, sin esperar a que termine
	go func() {
		ra.ms.Stop()
		close(stopped)
	}()

	// Despertar al listener hasta que haya leído la orden de parar
	for {
		select {
		case <-stopped:
			return
		default:
			ra.ms.Send(ra.me, Stop{Pid: ra.me})
			time.Sleep(10 * time.Millisecond)
		}
	}
}

// Pre: el llamante está en la sección crítica como escritor
// Post: todos los demás procesos han aplicado fragment
func (ra *RASharedDB) Update(fragment string) {
	if ra.N <= 1 {
		return
	}
	ra.Mutex.Lock()
	ra.AckCnt = ra.N - 1 // antes de enviar, o un Ack rápido podría adelantarse al contador
	ra.Mutex.Unlock()
	for i := 1; i <= ra.N; i++ {
		if i != ra.me {
			ra.ms.Send(i, Update{Pid: ra.me, Fragment: fragment})
		}
	}
	<-ra.chack
}

// Post: todos los demás procesos están escuchando
func (ra *RASharedDB) Barrier() {
	var wg sync.WaitGroup
	for i := 1; i <= ra.N; i++ {
		if i == ra.me {
			continue
		}
		wg.Add(1)
		go func(addr string) {
			defer wg.Done()
			var msg ms.Message = Ready{Pid: ra.me}
			for {
				conn, err := net.Dial("tcp", addr)
				if err != nil {
					time.Sleep(100 * time.Millisecond) // aún no escucha
					continue
				}
				err = gob.NewEncoder(conn).Encode(&msg)
				conn.Close()
				if err == nil {
					return
				}
			}
		}(ra.peers[i-1])
	}
	wg.Wait()
}

// Pre: este proceso ha terminado de usar la sección crítica
// Post: todos los demás procesos también han terminado
func (ra *RASharedDB) Finish() {
	if ra.N <= 1 {
		return
	}
	for i := 1; i <= ra.N; i++ {
		if i != ra.me {
			ra.ms.Send(i, Done{Pid: ra.me})
		}
	}
	<-ra.chdone
}
