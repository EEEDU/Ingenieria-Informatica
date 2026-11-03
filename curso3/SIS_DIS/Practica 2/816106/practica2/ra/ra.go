/*
* AUTOR: Rafael Tolosana Calasanz
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingenieria Informatica
*			Escuela de Ingenieria y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2021
* FICHERO: ricart-agrawala.go
* DESCRIPCIoN: Implementacion del algoritmo de Ricart-Agrawala Generalizado en Go
 */

// Package ra implementa el algoritmo de Ricart-Agrawala generalizado para el
// problema de lectores y escritores distribuido, sobre el modulo de
// mensajeria ms.
package ra

import (
	"bufio"
	"encoding/gob"
	"fmt"
	"log"
	"net"
	"os"
	"sync"
	"time"

	"practica2/ms"
)

// Operation es el tipo de acceso que un proceso hace a la seccion critica.
// Se usa tambien como indice de la matriz de exclusion.
type Operation int

const (
	Read  Operation = iota // 0
	Write                  // 1
)

// exclude[a][b] vale true si las operaciones a y b NO pueden ejecutarse a la
// vez. Es simetrica: solo dos lecturas son compatibles entre si.
var exclude = [2][2]bool{
	{false, true}, // read-read, read-write
	{true, true},  // write-read, write-write
}

// ---------------------------------------------------------------------------
// Mensajes. Todos los campos son exportados porque gob solo serializa campos
// exportados (uno en minuscula llegaria siempre a cero, sin avisar).
// ---------------------------------------------------------------------------

// Request: peticion de entrada a la seccion critica (REQUEST(k, j) de la Figura 1).
type Request struct {
	Clock     int       // numero de secuencia de la peticion
	Pid       int       // quien pide
	Operation Operation // que quiere hacer (leer o escribir)
}

// Reply: permiso concedido (PERMISSION de la Figura 1).
type Reply struct {
	Pid int
}

// Update: un escritor pide a los demas que añadan fragment a su copia.
type Update struct {
	Pid      int
	Fragment string
}

// Ack: confirma que se ha aplicado un Update.
type Ack struct {
	Pid int
}

// Ready: mensaje de la barrera de arranque. Solo sirve para comprobar que el
// destinatario escucha; receiveLoop lo ignora.
type Ready struct {
	Pid int
}

// Done: el emisor ha terminado de usar la seccion critica (ver Finish).
type Done struct {
	Pid int
}

// Stop: mensaje que un proceso se envia a si mismo en Stop() para sacar al
// listener de ms de su Accept(). receiveLoop lo ignora.
type Stop struct {
	Pid int // gob no codifica structs con campos pero ninguno exportado
}

// RASharedDB es el estado de un proceso en el algoritmo.
type RASharedDB struct {
	// Variables del algoritmo (Figura 1). Las tocan a la vez el hilo de la
	// aplicacion (Pre/PostProtocol) y receiveLoop: siempre bajo Mutex.
	OurSeqNum int        // numero de secuencia de mi peticion actual (lrd)
	HigSeqNum int        // mayor numero de secuencia visto (clock)
	OutRepCnt int        // respuestas que me faltan por recibir
	ReqCS     bool       // quiero entrar en la seccion critica, o estoy dentro
	RepDefd   []bool     // RepDefd[j]: le debo una respuesta aplazada al proceso j
	Mutex     sync.Mutex // protege las variables compartidas

	ms    *ms.MessageSystem // sistema de mensajeria
	chrep chan bool         // receiveLoop avisa: ya han llegado todas las respuestas

	// Datos fijos desde New: no cambian, no necesitan mutex.
	me        int       // mi pid, de 1 a N
	N         int       // numero total de procesos
	operation Operation // mi rol: Read o Write
	peers     []string  // IP:puerto de cada proceso; la linea i es el pid i

	// Replicacion de escrituras (Update/Ack).
	onUpdate func(fragment string) // callback de la aplicacion: escribe en el fichero local
	ackCnt   int                   // Acks que me faltan tras un Update (bajo Mutex)
	chack    chan bool             // receiveLoop avisa: han llegado todos los Ack

	// Finalizacion coordinada (Finish).
	doneFrom map[int]bool // pids de los que he recibido Done (bajo Mutex)
	chdone   chan bool    // receiveLoop avisa: todos han terminado
}

// New crea el proceso me (1..N) con el rol operation y arranca su receptor.
// N se deduce del fichero usersFile (una linea IP:puerto por proceso).
// onUpdate se invoca cada vez que otro proceso escribe, para que la
// aplicacion actualice su copia local; se pasa aqui y no con un setter porque
// receiveLoop ya esta activo al volver y podria recibir un Update antes.
func New(me int, usersFile string, operation Operation, onUpdate func(fragment string)) *RASharedDB {
	peers, err := readPeers(usersFile)
	if err != nil {
		log.Fatalf("no se puede leer %s: %v", usersFile, err)
	}
	n := len(peers)
	messageTypes := []ms.Message{Request{}, Reply{}, Update{}, Ack{}, Ready{}, Done{}, Stop{}}
	msgs := ms.New(me, usersFile, messageTypes)
	ra := RASharedDB{
		me:        me,
		N:         n,
		operation: operation,
		peers:     peers,
		RepDefd:   make([]bool, n+1), // se indexa por pid (1..N); la casilla 0 no se usa
		ms:        &msgs,
		// Con buffer 1, quien avisa no se bloquea si el otro aun no esta esperando.
		chrep:    make(chan bool, 1),
		chack:    make(chan bool, 1),
		chdone:   make(chan bool, 1),
		onUpdate: onUpdate,
		doneFrom: make(map[int]bool),
	}
	go receiveLoop(&ra)
	return &ra
}

// receiveLoop atiende los mensajes entrantes mientras el proceso hace otras
// cosas (los "procesos concurrentes" de las lineas (10)-(15) de la Figura 1).
func receiveLoop(ra *RASharedDB) {
	for {
		msg := ra.ms.Receive()
		if msg == nil {
			return // el buzon se cerro: ms se ha parado
		}
		switch m := msg.(type) {
		case Request:
			// Lineas (10)-(14). Se decide bajo el mutex, pero se envia fuera
			// para no tener bloqueado el estado compartido durante la red.
			ra.Mutex.Lock()
			if ra.HigSeqNum < m.Clock {
				ra.HigSeqNum = m.Clock
			}
			// Respondo ya salvo que: me interese entrar, nuestras operaciones
			// se excluyan y mi peticion sea anterior (menor (reloj, pid)).
			responder := !exclude[ra.operation][m.Operation] ||
				!ra.ReqCS ||
				m.Clock < ra.OurSeqNum ||
				(m.Clock == ra.OurSeqNum && m.Pid < ra.me)
			if !responder {
				ra.RepDefd[m.Pid] = true
			}
			ra.Mutex.Unlock()
			if responder {
				ra.ms.Send(m.Pid, Reply{Pid: ra.me})
			}
		case Reply:
			// Linea (15).
			ra.Mutex.Lock()
			ra.OutRepCnt--
			todasRecibidas := ra.OutRepCnt == 0
			ra.Mutex.Unlock()
			if todasRecibidas {
				ra.chrep <- true
			}
		case Update:
			// Primero se aplica el cambio y solo despues se confirma.
			if ra.onUpdate != nil {
				ra.onUpdate(m.Fragment)
			}
			ra.ms.Send(m.Pid, Ack{Pid: ra.me})
		case Ack:
			ra.Mutex.Lock()
			ra.ackCnt--
			todos := ra.ackCnt == 0
			ra.Mutex.Unlock()
			if todos {
				ra.chack <- true
			}
		case Done:
			ra.Mutex.Lock()
			todos := false
			if !ra.doneFrom[m.Pid] {
				ra.doneFrom[m.Pid] = true
				todos = len(ra.doneFrom) == ra.N-1
			}
			ra.Mutex.Unlock()
			if todos {
				ra.chdone <- true
			}
		case Ready:
			// Solo comprobaba que escuchamos (ver Barrier): nada que hacer.
		case Stop:
			// Solo sirve para despertar al listener (ver Stop): nada que hacer.
		}
	}
}

// Pre: Verdad
// Post: Realiza el PreProtocol para el algoritmo de
// Ricart-Agrawala Generalizado: al volver, el proceso puede entrar en la
// seccion critica con su operacion.
func (ra *RASharedDB) PreProtocol() {
	// Lineas (1)-(2): estado y numero de secuencia, juntos bajo un mismo Lock
	// para que un Request ajeno no vea un estado a medias.
	ra.Mutex.Lock()
	ra.ReqCS = true
	ra.OurSeqNum = ra.HigSeqNum + 1
	ra.OutRepCnt = ra.N - 1
	seq := ra.OurSeqNum
	ra.Mutex.Unlock()
	// Con N == 1 no hay nadie a quien esperar: nadie escribiria en chrep.
	if ra.N > 1 {
		// Lineas (3)-(4): pedir permiso a todos los demas.
		for i := 1; i <= ra.N; i++ {
			if i != ra.me {
				ra.ms.Send(i, Request{Clock: seq, Pid: ra.me, Operation: ra.operation})
			}
		}
		// Linea (5): esperar a las N-1 respuestas.
		<-ra.chrep
	}
}

// Pre: Verdad
// Post: Realiza el PostProtocol para el algoritmo de
// Ricart-Agrawala Generalizado: se contesta a todos los que se habian
// aplazado.
func (ra *RASharedDB) PostProtocol() {
	// Lineas (7)-(9). Bajo el mutex se anota a quien contestar; los envios se
	// hacen despues de soltarlo.
	ra.Mutex.Lock()
	ra.ReqCS = false
	var pendientes []int
	for i, aplazado := range ra.RepDefd {
		if aplazado {
			pendientes = append(pendientes, i)
			ra.RepDefd[i] = false
		}
	}
	ra.Mutex.Unlock()
	for _, i := range pendientes {
		ra.ms.Send(i, Reply{Pid: ra.me})
	}
}

// Pre: el llamante esta dentro de la seccion critica como escritor.
// Post: todos los demas procesos han aplicado fragment en su copia local.
// Sin la espera a los Ack, el Reply aplazado de PostProtocol podria llegar a
// un lector antes que el Update (ms no ordena entre mensajes distintos).
func (ra *RASharedDB) Update(fragment string) {
	if ra.N <= 1 {
		return
	}
	// El contador se fija antes de enviar, o un Ack rapido podria adelantarse.
	ra.Mutex.Lock()
	ra.ackCnt = ra.N - 1
	ra.Mutex.Unlock()
	for i := 1; i <= ra.N; i++ {
		if i != ra.me {
			ra.ms.Send(i, Update{Pid: ra.me, Fragment: fragment})
		}
	}
	<-ra.chack
}

// Pre: ra.New ya se ha ejecutado (mi listener esta abierto).
// Post: devuelve nil cuando todos los demas procesos escuchan, o un error si
// alguno no lo hace antes de timeout. Evita el os.Exit(1) de ms.Send cuando
// se escribe a un proceso que aun no ha arrancado.
// Se envia un Ready real (y no una conexion vacia) porque ms convierte una
// conexion vacia en un nil dentro del buzon, que receiveLoop confundiria con
// un buzon cerrado.
func (ra *RASharedDB) Barrier(timeout time.Duration) error {
	deadline := time.Now().Add(timeout)
	errs := make(chan error, ra.N) // como mucho un error por proceso
	var wg sync.WaitGroup
	for i := 1; i <= ra.N; i++ {
		if i == ra.me {
			continue
		}
		wg.Add(1)
		go func(pid int, addr string) {
			defer wg.Done()
			// Misma codificacion que ms.Send: un puntero a Message.
			var msg ms.Message = Ready{Pid: ra.me}
			for {
				conn, err := net.DialTimeout("tcp", addr, 500*time.Millisecond)
				if err == nil {
					err = gob.NewEncoder(conn).Encode(&msg)
					conn.Close()
					if err == nil {
						return
					}
				}
				if time.Now().After(deadline) {
					errs <- fmt.Errorf("el proceso %d (%s) no escucha tras %v", pid, addr, timeout)
					return
				}
				time.Sleep(100 * time.Millisecond) // aun no escucha
			}
		}(i, ra.peers[i-1])
	}
	wg.Wait()
	close(errs)
	return <-errs // el primer error, o nil si el canal esta vacio
}

// Pre: este proceso ha terminado de usar la seccion critica.
// Post: todos los demas procesos tambien han terminado. Hasta entonces
// receiveLoop sigue activo y este proceso sigue contestando a los demas; si se
// parase antes, los que aun le envian mensajes moririan en el os.Exit de ms.
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

// Pre: Finish ya ha vuelto (nadie mas va a escribir a este proceso).
// Post: ms y receiveLoop terminan.
// ms.Stop() bloquea hasta que el listener lee su canal done, y este solo lo
// mira entre conexiones: si esta parado en Accept() no se entera. Por eso
// ms.Stop() se lanza en una goroutine y se despierta al listener enviandonos
// mensajes hasta que termine.
func (ra *RASharedDB) Stop() {
	stopped := make(chan struct{})
	go func() {
		ra.ms.Stop()
		close(stopped)
	}()
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

// readPeers lee las direcciones IP:puerto de path, una por linea. Igual que
// ms, no se saltan lineas vacias: la linea i corresponde al pid i. Se
// duplica aqui porque parsePeers de ms es privada.
func readPeers(path string) ([]string, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var peers []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		peers = append(peers, scanner.Text())
	}
	return peers, scanner.Err()
}
