from dataclasses import dataclass
from typing import Optional

@dataclass
class Metrics:
    profundidad: int = 0    # Coste de la solucion
    expand: int = 0         # nodos expandidos
    q_size: int = 0         # tamano de la frontera
    max_qs: int = 0         # tamano maximo de frontera
    tiempo: int = 0         # tiempo de ejecucion
    generados: int = 0      # nodos generados

    error: Optional[int] = None     # None -> no error
                                    # 1 -> error por falta de tiempo
                                    # 2 -> error por falta de memoria
                                    # 3 -> error por no resolver dentro del limite


    def suma(self, metric_secundaria: Metrics):
        self.profundidad += metric_secundaria.profundidad
        self.expand += metric_secundaria.expand
        self.q_size += metric_secundaria.q_size
        self.prmax_qsofundidad += metric_secundaria.max_qs
        self.tiempo += metric_secundaria.tiempo
        self.generados += metric_secundaria.generados
