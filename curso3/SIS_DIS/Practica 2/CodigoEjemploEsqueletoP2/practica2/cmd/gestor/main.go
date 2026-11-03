// Package gestor implementa el gestor del fichero de texto local de un proceso.
package gestor

import (
	"log"
	"os"
	"sync"
)

type Gestor struct {
	mu   sync.Mutex // defensa extra: el protocolo ya evita accesos simultáneos
	path string
}

// New crea (o vacía) el fichero en path y devuelve su gestor.
func New(path string) *Gestor {
	if err := os.WriteFile(path, nil, 0o644); err != nil {
		log.Fatalf("no se puede crear %s: %v", path, err)
	}
	return &Gestor{path: path}
}

// LeerFichero devuelve el contenido completo del fichero.
func (g *Gestor) LeerFichero() string {
	g.mu.Lock()
	defer g.mu.Unlock()
	data, err := os.ReadFile(g.path)
	if err != nil {
		log.Fatalf("no se puede leer %s: %v", g.path, err)
	}
	return string(data)
}

// EscribirFichero añade fragmento al final del fichero.
func (g *Gestor) EscribirFichero(fragmento string) {
	g.mu.Lock()
	defer g.mu.Unlock()
	f, err := os.OpenFile(g.path, os.O_APPEND|os.O_WRONLY, 0o644)
	if err != nil {
		log.Fatalf("no se puede abrir %s: %v", g.path, err)
	}
	defer f.Close()
	if _, err := f.WriteString(fragmento); err != nil {
		log.Fatalf("no se puede escribir en %s: %v", g.path, err)
	}
}
