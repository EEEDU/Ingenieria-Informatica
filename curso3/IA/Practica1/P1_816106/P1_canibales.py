from searchEina import *  # o los imports concretos que necesites

class Canibals(Problem):
    # states = (C_izq, M_izq, Bote, C_der, M_der)
    MOVIMIENTOS = {'1C': (1, 0), '2C': (2, 0), '1M': (0, 1), '2M': (0, 2), '1C1M': (1, 1)}

    def __init__(self, initial=(3, 3, 0, 0, 0), goal=(0, 0, 1, 3, 3)):
        super().__init__(initial, goal)

    def actions(self, state):
        c_izq, m_izq, bote, c_der, m_der = state
        posibles = []
        if bote == 0:
            c_origen, m_origen = c_izq, m_izq
            c_destino, m_destino = c_der, m_der
        else:
            c_origen, m_origen = c_der, m_der
            c_destino, m_destino = c_izq, m_izq


        for accion in ['1C', '2C', '1C1M', '1M', '2M']:
            # 1. cuántos C y M mueve esta acción
            c, m = self.MOVIMIENTOS[accion]
            # 2. ¿hay suficientes en la orilla de origen?
            if c_origen >= c and m_origen >= m:
                if (c_destino+c <= m_destino+m or m_destino+m == 0) and (c_origen-c <= m_origen-m or m_origen-m == 0):
                    posibles.append(accion)
        return posibles

    def result(self, state, action):
        c_izq, m_izq, bote, c_der, m_der = state
        if bote == 0:
            c_origen, m_origen = c_izq, m_izq
            c_destino, m_destino = c_der, m_der
        else:
            c_origen, m_origen = c_der, m_der
            c_destino, m_destino = c_izq, m_izq

        c, m = self.MOVIMIENTOS[action]

        c_origen -= c
        m_origen -= m
        c_destino += c
        m_destino += m
        if bote == 0:
            return c_origen, m_origen, 1, c_destino, m_destino
        else:
            return c_destino, m_destino, 0, c_origen, m_origen

def print_state(state):
    c_izq, m_izq, bote, c_der, m_der = state
    izq = 'M ' * m_izq + 'C ' * c_izq
    der = 'M ' * m_der + 'C ' * c_der
    if bote == 0:
        rio = 'BOTE --RIO--'
    else:
        rio = '--RIO-- BOTE'
    print(f'RIBERA-IZQ {izq}{rio} {der}RIBERA-DCH')

if __name__ == "__main__":
    problema = Canibals()
    X, _ = breadth_first_graph_search(problema)  # o el algoritmo que quieras
    for nodo in X.path():
        print(f'{nodo.action} |', end='')
        print_state(nodo.state)