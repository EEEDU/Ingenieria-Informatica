package ra_test

// Pruebas del algoritmo de Ricart-Agrawala generalizado.
//
// Cada "proceso" es una goroutine con su propia instancia de ra y su propio
// fichero, y se comunican por TCP a través de ms (como en la ejecución real).
// La exclusión mutua se comprueba con contadores atómicos, sin depender de
// relojes físicos:
//   - un escritor no puede entrar si hay otro escritor o algún lector dentro;
//   - un lector no puede entrar si hay un escritor dentro.
//
// Ejecutar con: go test -race -count=1 -timeout 120s ./ra/

import (
	"fmt"
	"math/rand"
	"net"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"sync"
	"sync/atomic"
	"testing"
	"time"

	"practica2/cmd/gestor"
	"practica2/ra"
)

const iteraciones = 5

// Fichero con las direcciones IP:puerto, una por línea (la línea i es el
// proceso con pid i). Igual que en ms_test.go, se busca en este directorio.
const usersFile = "./users.txt"

const saltoPuertos = 100

var ejecuciones int // n de veces que se ha llamado a ejecutar en este proceso

// usersDesplazado lee usersFile, suma desplazamiento a cada puerto, escribe el
// resultado en dir/users.txt y devuelve esa ruta.
func usersDesplazado(t *testing.T, dir string, desplazamiento int) string {
	t.Helper()
	datos, err := os.ReadFile(usersFile)
	if err != nil {
		t.Fatalf("no se puede leer %s: %v", usersFile, err)
	}
	lineas := strings.Split(strings.TrimRight(string(datos), "\n"), "\n")
	for i, l := range lineas {
		host, puerto, err := net.SplitHostPort(strings.TrimSpace(l))
		if err != nil {
			t.Fatalf("%s, línea %d (%q): %v", usersFile, i+1, l, err)
		}
		p, err := strconv.Atoi(puerto)
		if err != nil {
			t.Fatalf("%s, línea %d: puerto no numérico %q", usersFile, i+1, puerto)
		}
		lineas[i] = net.JoinHostPort(host, strconv.Itoa(p+desplazamiento))
	}
	ruta := filepath.Join(dir, "users.txt")
	if err := os.WriteFile(ruta, []byte(strings.Join(lineas, "\n")), 0o644); err != nil {
		t.Fatal(err)
	}
	return ruta
}

// numProcesos devuelve cuántos procesos define users.txt (una línea cada uno).
func numProcesos(t *testing.T) int {
	t.Helper()
	datos, err := os.ReadFile(usersFile)
	if err != nil {
		t.Fatalf("no se puede leer %s: %v", usersFile, err)
	}
	return len(strings.Split(strings.TrimRight(string(datos), "\n"), "\n"))
}

// comprobarContenido verifica el formato del fichero final: cada escritor
// aparece con sus linea 1..., en orden y sin repetirse.
func comprobarContenido(t *testing.T, contenido string, escritores int) {
	t.Helper()
	var lineas []string
	if contenido != "" {
		lineas = strings.Split(strings.TrimSuffix(contenido, "\n"), "\n")
	}
	siguiente := map[int]int{}
	for _, l := range lineas {
		var pid, it int
		if _, err := fmt.Sscanf(l, "escritor %d: línea %d", &pid, &it); err != nil {
			t.Errorf("línea con formato inesperado: %q", l)
			continue
		}
		if it != siguiente[pid]+1 {
			t.Errorf("escritor %d: línea %d fuera de orden (se esperaba %d)", pid, it, siguiente[pid]+1)
		}
		siguiente[pid] = it
	}
	if len(siguiente) != escritores {
		t.Errorf("hay %d escritores en el fichero, se esperaban %d", len(siguiente), escritores)
	}
	for pid, n := range siguiente {
		if n != iteraciones {
			t.Errorf("escritor %d: %d líneas, se esperaban %d", pid, n, iteraciones)
		}
	}
}

// ejecutar lanza len(roles) procesos; roles[i] == 'E' => el pid i+1 es
// escritor, cualquier otra letra (p. ej. 'L') => lector.
func ejecutar(t *testing.T, roles string) {
	t.Helper() // esta funcion es auxiliar de test
	n := len(roles)
	if n != numProcesos(t) {
		t.Fatalf("roles %q define %d procesos pero %s tiene %d líneas", roles, n, usersFile, numProcesos(t))
	}
	// crear una carpeta temporal unica y vacio para el test
	// aqui van los fichero_N.txt
	dir := t.TempDir()
	users := usersDesplazado(t, dir, ejecuciones*saltoPuertos)
	ejecuciones++

	var enEscritura, enLectura atomic.Int32
	var wg sync.WaitGroup // para esperar que se terminen todos
	for pid := 1; pid <= n; pid++ {

		wg.Add(1) // antes de lanzar la goroutine: "hay una más por esperar"
		go func(pid int) {
			defer wg.Done() // al terminar marco que se acabo
			g := gestor.New(filepath.Join(dir, fmt.Sprintf("fichero_%d.txt", pid)))

			// asignar una operacion en funcion de su rol
			escritor := roles[pid-1] == 'E'
			op := ra.Read
			if escritor {
				op = ra.Write
			}

			r := ra.New(pid, users, op, g.EscribirFichero)
			// barrera
			if err := r.Barrier(20 * time.Second); err != nil {
				t.Errorf("proceso %d: %v", pid, err)
				return // los demás se quedarán esperando: el -timeout lo cortará
			}
			for it := 1; it <= iteraciones; it++ {
				r.PreProtocol()
				if escritor {
					if enEscritura.Add(1) != 1 || enLectura.Load() != 0 {
						t.Errorf("escritor %d (it %d) entra con otros procesos dentro", pid, it)
					}
					frag := fmt.Sprintf("escritor %d: línea %d\n", pid, it)
					g.EscribirFichero(frag)
					r.Update(frag)
					enEscritura.Add(-1)
				} else {
					enLectura.Add(1)
					if enEscritura.Load() != 0 {
						t.Errorf("lector %d (it %d) entra con un escritor dentro", pid, it)
					}
					c := g.LeerFichero()
					if c != "" && !strings.HasSuffix(c, "\n") {
						t.Errorf("lector %d (it %d) ve una escritura a medias", pid, it)
					}
					enLectura.Add(-1)
				}
				r.PostProtocol()
				time.Sleep(time.Duration(rand.Intn(50)) * time.Millisecond)
			}
			r.Finish()
			r.Stop()
		}(pid)
	}
	wg.Wait()

	// Tras la escritura, todas las copias deben ser iguales.
	ref, err := os.ReadFile(filepath.Join(dir, "fichero_1.txt"))
	if err != nil {
		t.Fatal(err)
	}
	for pid := 2; pid <= n; pid++ {
		b, err := os.ReadFile(filepath.Join(dir, fmt.Sprintf("fichero_%d.txt", pid)))
		if err != nil {
			t.Fatal(err)
		}
		if string(b) != string(ref) {
			t.Errorf("fichero_%d.txt difiere de fichero_1.txt (%d vs %d bytes)", pid, len(b), len(ref))
		}
	}
	comprobarContenido(t, string(ref), strings.Count(roles, "E"))
}

// Todos escritores: se excluyen entre sí, todas las copias deben quedar iguales.
func TestTodosEscritores(t *testing.T) {
	ejecutar(t, strings.Repeat("E", numProcesos(t)))
}

// Todos lectores: ninguno se excluye, no se escribe nada (ficheros vacíos).
func TestTodosLectores(t *testing.T) {
	ejecutar(t, strings.Repeat("L", numProcesos(t)))
}

// Un solo lector, el resto escritores.
func TestUnLectorRestoEscritores(t *testing.T) {
	n := numProcesos(t)
	ejecutar(t, strings.Repeat("E", n-1)+"L")
}

// Un solo escritor, el resto lectores.
func TestUnEscritorRestoLectores(t *testing.T) {
	n := numProcesos(t)
	ejecutar(t, "E"+strings.Repeat("L", n-1))
}

// Mitad escritores, mitad lectores (si N es impar, un lector más).
func TestMitadYMitad(t *testing.T) {
	n := numProcesos(t)
	ejecutar(t, strings.Repeat("E", n/2)+strings.Repeat("L", n-n/2))
}
