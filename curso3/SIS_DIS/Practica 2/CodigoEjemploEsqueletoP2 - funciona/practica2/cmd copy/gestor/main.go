package gestor

import (
	"fmt"
	"log"
	"os"
	"sync"
)

type Gestor struct {
	path  string
	mutex sync.Mutex
}

func New(pid int) *Gestor {
	path := fmt.Sprintf("fichero_%d.txt", pid)
	f, err := os.OpenFile(path, os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatalf("no se ha podido crear %s: %v", path, err)
	}
	f.Close()
	return &Gestor{path: path}
}
func (g *Gestor) LeerFichero() string {
	g.mutex.Lock()
	defer g.mutex.Unlock()

	data, err := os.ReadFile(g.path)
	if err != nil {
		log.Fatalf("no puedo leer %s: %v", g.path, err)
	}
	return string(data)
}
func (g *Gestor) EscribirFichero(fragmento string) {
	g.mutex.Lock()
	defer g.mutex.Unlock()

	f, err := os.OpenFile(g.path, os.O_APPEND|os.O_WRONLY|os.O_CREATE, 0644)
	if err != nil {
		log.Fatalf("no puedo abrir %s: %v", g.path, err)
	}
	defer f.Close()

	if _, err := f.WriteString(fragmento); err != nil {
		log.Fatalf("no puedo escribir en %s: %v", g.path, err)
	}
}
