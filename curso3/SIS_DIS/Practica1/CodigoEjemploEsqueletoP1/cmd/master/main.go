/*
* AUTOR: Eduardo Guerrero
* ASIGNATURA: 30221/39521 - Sistemas Distribuidos
*	       Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FICHERO: master/main.go
 */
package main

import (
	"bufio"
	"encoding/gob"
	"log"
	"net"
	"os"
	"os/exec"
	"practica1/com"
	"sync"
	"time"
)

const (
	dialAttempts = 20                     // attempts to reach a worker
	retryDelay   = 500 * time.Millisecond // pause between attempts
)

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

// readEndpoints Lee endpoints del fichero
func readEndpoints(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var endpoints []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		if line := scanner.Text(); line != "" {
			endpoints = append(endpoints, line)
		}
	}
	return endpoints, scanner.Err()
}

// startWorker lanza la ejecucion en ssh
func startWorker(endpoint, workerPath string) (*exec.Cmd, error) {
	host, _, err := net.SplitHostPort(endpoint)
	if err != nil {
		return nil, err
	}
	cmd := exec.Command("ssh", "-o", "BatchMode=yes",
		"-o", "StrictHostKeyChecking=no", host, workerPath, endpoint)
	cmd.Stdout, cmd.Stderr = os.Stdout, os.Stderr
	return cmd, cmd.Start()
}

// startWorkers Inicializa los workers y devuelve una listas con los cmds.
func startWorkers(endpoints []string, workerPath string) []*exec.Cmd {
	var cmds []*exec.Cmd
	for _, ep := range endpoints {
		cmd, err := startWorker(ep, workerPath)
		com.CheckError(err)
		cmds = append(cmds, cmd)
	}
	return cmds
}

// dialRetry intenta conectarse varas veces por si el worker todavia no ha iniciado.
func dialRetry(endpoint string) (net.Conn, error) {
	var conn net.Conn
	var err error
	for i := 0; i < dialAttempts; i++ {
		if conn, err = net.Dial("tcp", endpoint); err == nil {
			return conn, nil
		}
		time.Sleep(retryDelay)
	}
	return nil, err
}

// callWorker envia solicitud a worker remote y devuelve su respuesta.
func callWorker(endpoint string, req com.Request) (com.Reply, error) {
	var reply com.Reply
	conn, err := dialRetry(endpoint)
	if err != nil {
		return reply, err
	}
	defer conn.Close()
	err = gob.NewEncoder(conn).Encode(req)
	if err == nil {
		err = gob.NewDecoder(conn).Decode(&reply)
	}
	return reply, err
}

// remoteWorker reenvia cada tarea al worker y el resultado lo manda por el otro canal
func remoteWorker(endpoint string, tasks <-chan task, results chan<- result, wg *sync.WaitGroup) {
	defer wg.Done()
	for t := range tasks {
		reply, err := callWorker(endpoint, t.req)
		if err != nil {
			log.Println("Worker", endpoint, "failed:", err)
			t.conn.Close()
			continue
		}
		log.Println("Worker", endpoint, "answered")
		results <- result{conn: t.conn, reply: reply}
	}
}

// responder envia cada respues a su cliente y cierra la conexion.
// Cuando estan todos cerrados termina
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

// readTask Acepta la conexion y lee la tarea
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

// serve Entrega cada solicitud entratne a los trabajadores
func serve(listener net.Listener, tasks chan<- task) {
	for {
		t, err := readTask(listener)
		log.Println("Task received, id", t.req.Id)
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

// startPool lanza una goroutina por cada worker
func startPool(endpoints []string, tasks chan task, results chan result,
	wg *sync.WaitGroup, done chan bool) {
	for _, ep := range endpoints {
		wg.Add(1)
		go remoteWorker(ep, tasks, results, wg)
	}
	go responder(results, done)
}

// shutdown waits until every pending request has been answered.
func shutdown(tasks chan task, results chan result,
	wg *sync.WaitGroup, done chan bool) {
	close(tasks)
	wg.Wait()
	close(results)
	<-done
}

// sendEnd sends the end request (Id == -1) to a worker.
func sendEnd(endpoint string) error {
	conn, err := net.Dial("tcp", endpoint)
	if err != nil {
		return err
	}
	defer conn.Close()
	return gob.NewEncoder(conn).Encode(com.Request{Id: -1})
}

// stopWorkers tells every worker to finish and waits for its ssh command.
func stopWorkers(endpoints []string, cmds []*exec.Cmd) {
	for i, ep := range endpoints {
		if err := sendEnd(ep); err != nil {
			log.Println("Error stopping worker", ep, ":", err)
			cmds[i].Process.Kill()
		}
		cmds[i].Wait()
	}
}

// setup leer argumentos escuchar de endpoint
func setup() (net.Listener, []string, string) {
	if len(os.Args) != 4 {
		log.Println("Usage: master ip:port workers_file worker_binary")
		os.Exit(1)
	}
	endpoints, err := readEndpoints(os.Args[2])
	com.CheckError(err)
	listener, err := net.Listen("tcp", os.Args[1])
	com.CheckError(err)
	log.SetFlags(log.Lshortfile | log.Lmicroseconds)
	log.Println("***** Master listening in endpoint", os.Args[1])

	return listener, endpoints, os.Args[3]
}

// arg[1] = endpoint
// arg[2] = workers file
// arg[3] = worker binary (abs. path)
func main() {
	listener, endpoints, workerPath := setup()
	// Inicializa workers (ssh)
	cmds := startWorkers(endpoints, workerPath)
	// crea canales de tareas, resutlados y hecho
	tasks, results, done := make(chan task), make(chan result), make(chan bool)
	var wg sync.WaitGroup
	startPool(endpoints, tasks, results, &wg, done)
	serve(listener, tasks)
	shutdown(tasks, results, &wg, done)
	stopWorkers(endpoints, cmds)
	listener.Close()
	log.Println("Master finished")
}
