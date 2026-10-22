import random
from typing import Optional

from searchEina import (
    EightPuzzle,
    astar_search,
    breadth_first_graph_search,
    iterative_deepening_search,
    astar_h1,
    astar_h2,
)
from bissecion import bissecion

_NUM_CASES = 100
_STEPS = 20
INVERSE = {"UP": "DOWN", "DOWN": "UP", "LEFT": "RIGHT", "RIGHT": "LEFT"}


def random_node() -> tuple:
    """
    Genera un estado aleatorio del 8-puzzle.

    Returns:
        tuple: permutación de los números 0 a 8.
    """
    node_init = [0, 1, 2, 3, 4, 5, 6, 7, 8]
    node_random = []
    for _ in range(9):
        value = random.choice(node_init)
        node_init.remove(value)
        node_random.append(value)
    return tuple(node_random)


def generate_node_final(node_init: tuple, depth: int) -> Optional[tuple]:
    """
    Genera un nodo final a partir de una tupla inicial y un numero de pasos.
    Para evitar bucles, se elimina la inversa a la ultima accion.

    Args:
        node_init: nodo inicial
        steps: numero de pasos que se da hata llegar al estado final

    Returns
        tupla con estado final
    """
    eight_problem = EightPuzzle(node_init)
    node_actual = node_init
    nodes = set(node_init)
    last_action = None
    for _ in range(depth):
        p_actions = eight_problem.actions(node_actual)
        action_inv = INVERSE.get(last_action)
        if action_inv in p_actions:
            p_actions.remove(action_inv)
        last_action = random.choice(p_actions)
        node_actual = eight_problem.result(node_actual, last_action)
        if node_actual in nodes:
            return None
        nodes.add(node_actual)
    return node_actual


def check_steps_ok(node_init: tuple, node_final: tuple, depth: int) -> bool:
    """Comprobar si del nodod final al nodo inicial hay el numero de pasos o existen atajos."""
    problem = EightPuzzle(initial=node_final, goal=node_init)
    node_result, metrics = astar_search(problem, problem.h2)
    if node_result is None or node_result.state != node_init:
        return False
    return metrics.profundidad == depth


def generate_nodes(depth: int) -> list:
    nodes_init = set()
    nodes = []
    while len(nodes_init) < _NUM_CASES:
        node_i = random_node()
        if node_i not in nodes_init:
            node_f = generate_node_final(node_i, depth)
            while node_f is None or not check_steps_ok(node_i, node_f, depth):
                node_f = generate_node_final(node_i, depth)
            nodes_init.add(node_i)
            nodes.append({"init": node_i, "final": node_f})
            # print(len(nodes_init), " init: ", node_i, " final: ", node_f)
    return nodes


def average_generated_nodes(nodes: list, search) -> int:
    average = 0
    for node in nodes:
        problem = EightPuzzle(initial=node["init"], goal=node["final"])
        node_result, metrics = search(problem)
        average += metrics.generados
    return round(average / len(nodes))


def print_header():
    weight = 9
    names = ["BFS", "IDS", "A*h(1)", "A*h(2)"]
    line = "-" * 107
    cols = " | ".join(f"{n:^{weight}}" for n in names)
    print(line)
    print(f"||{'':>4}|| {'Nodos Generados':^45} || {'b*':^45} ||")
    print(line)
    print(f"||{'d':>4}|| {cols} || {cols} ||")
    print(line)


def print_line(d, bfs_gen, ids_gen, h1_gen, h2_gen, bfs_b, ids_b, h1_b, h2_b):
    weight = 9
    params_gen = [bfs_gen, ids_gen, h1_gen, h2_gen]
    params_b = [bfs_b, ids_b, h1_b, h2_b]
    cols_gen = " | ".join(f"{n:^{weight}}" for n in params_gen)
    cols_b = " | ".join(f"{n:^{weight}}" for n in params_b)

    print(f"||{d:>4}|| {cols_gen} || {cols_b} ||")


if __name__ == "__main__":

    # for i in range(2, 24):
    #     print(f"**** PROFUNDIDAD {i} ****")
    #     generate_nodes(i)

    print_header()
    for depth in range(2, 24):
        nodes = generate_nodes(depth)
        bfs_gen = average_generated_nodes(nodes, breadth_first_graph_search)
        if depth <= 10:
            ids_gen = average_generated_nodes(nodes, iterative_deepening_search)
        else:
            ids_gen = "---"
        h1_gen = average_generated_nodes(nodes, astar_h1)
        h2_gen = average_generated_nodes(nodes, astar_h2)
        bfs_b = round(bissecion(1.0001, 10, 0.0001, depth, bfs_gen), 2)
        if depth <= 10:
            ids_b = round(bissecion(1.0001, 10, 0.0001, depth, ids_gen), 2)
        else:
            ids_b = "---"
        h1_b = round(bissecion(1.0001, 10, 0.0001, depth, h1_gen), 2)
        h2_b = round(bissecion(1.0001, 10, 0.0001, depth, h2_gen), 2)

        print_line(depth, bfs_gen, ids_gen, h1_gen, h2_gen, bfs_b, ids_b, h1_b, h2_b)
