from searchEina import *  # o los imports concretos que necesites

threeSteps = (1, 2, 5, 3, 4, 0, 6, 7, 8)
nineSteps = (1, 4, 2, 7, 5, 8, 3, 0, 6)
thirtySteps = (0, 8, 7, 6, 5, 4, 3, 2, 1)

def main():

    problemaThreeSteps = EightPuzzle(threeSteps)
    problemaNineSteps = EightPuzzle(nineSteps)
    problemaThirtySteps = EightPuzzle(thirtySteps)

    # resultG = breadth_first_graph_search(problema)
    _, metrics_BFS_G_3 = breadth_first_graph_search(problemaThreeSteps)
    _, metrics_BFS_T_3 = breadth_first_tree_search(problemaThreeSteps)
    _, metrics_DFS_G_3 = depth_first_graph_search(problemaThreeSteps)
    _, metrics_DFS_T_3 = depth_first_tree_search(problemaThreeSteps)
    _, metrics_DLS_3_3 = depth_limited_search(problemaThreeSteps, 3)
    _, metrics_DLS_9_3 = depth_limited_search(problemaThreeSteps, 9)
    _, metrics_IDS_3 = iterative_deepening_search(problemaThreeSteps)
    _, metrics_UCS_3 = uniform_cost_search(problemaThreeSteps)


    _, metrics_BFS_G_9 = breadth_first_graph_search(problemaNineSteps)
    _, metrics_BFS_T_9 = breadth_first_tree_search(problemaNineSteps)
    _, metrics_DFS_G_9 = depth_first_graph_search(problemaNineSteps)
    _, metrics_DFS_T_9 = depth_first_tree_search(problemaNineSteps)
    _, metrics_DLS_3_9 = depth_limited_search(problemaNineSteps, 3)
    _, metrics_DLS_9_9 = depth_limited_search(problemaNineSteps, 9)
    _, metrics_IDS_9 = iterative_deepening_search(problemaNineSteps)
    _, metrics_UCS_9 = uniform_cost_search(problemaNineSteps)


    _, metrics_BFS_G_30 = breadth_first_graph_search(problemaThirtySteps)
    _, metrics_BFS_T_30 = breadth_first_tree_search(problemaThirtySteps)
    _, metrics_DFS_G_30 = depth_first_graph_search(problemaThirtySteps)
    _, metrics_DFS_T_30 = depth_first_tree_search(problemaThirtySteps)
    _, metrics_DLS_3_30 = depth_limited_search(problemaThirtySteps, 3)
    _, metrics_DLS_9_30 = depth_limited_search(problemaThirtySteps, 9)
    _, metrics_IDS_30 = iterative_deepening_search(problemaThirtySteps)
    _, metrics_UCS_30 = uniform_cost_search(problemaThirtySteps)

    print(f" {'Problema':>9} | {'Profundidad':>11} | {'Expand':>8} | {'Q.Size':>8} | {'MaxQS':>8} | {'Tiempo(s)':>10} | {'Generados':>9}")
    print_metrics("BFS-G-3", metrics_BFS_G_3)
    print_metrics("BFS-T-3", metrics_BFS_T_3)
    print_metrics("DFS-G-3", metrics_DFS_G_3)
    print_metrics("DFS-T-3", metrics_DFS_T_3)
    print_metrics("DLS_3_3", metrics_DLS_3_3)
    print_metrics("DLS_9_3", metrics_DLS_9_3)
    print_metrics("IDS-3", metrics_IDS_3)
    print_metrics("UCS-3", metrics_UCS_3)

    print_metrics("BFS-G-9", metrics_BFS_G_9)
    print_metrics("BFS-T-9", metrics_BFS_T_9)
    print_metrics("DFS-G-9", metrics_DFS_G_9)
    print_metrics("DFS-T-9", metrics_DFS_T_9)
    print_metrics("DLS_3_9", metrics_DLS_3_9)
    print_metrics("DLS_9_9", metrics_DLS_9_9)
    print_metrics("IDS-9", metrics_IDS_9)
    print_metrics("UCS-9", metrics_UCS_9)

    print_metrics("BFS-G-30", metrics_BFS_G_30)
    print_metrics("BFS-T-30", metrics_BFS_T_30)
    print_metrics("DFS-G-30", metrics_DFS_G_30)
    print_metrics("DFS-T-30", metrics_DFS_T_30)
    print_metrics("DLS_3_30", metrics_DLS_3_30)
    print_metrics("DLS_9_30", metrics_DLS_9_30)
    print_metrics("IDS-30", metrics_IDS_30)
    print_metrics("UCS-30", metrics_UCS_30)

def print_metrics(name: str, metric: Metrics):
    if metric.error:
        print(f" {name:>9} | "
              "        --- | "
              "     --- | "
              "     --- | "
              "     --- | "
              "         --- | "
              f"      ({metric.error}) ")
    else:
        print(f" {name:>9} | "
            f"{metric.profundidad:>11} | "
            f"{metric.expand:>8} | "
            f"{metric.q_size:>8} | "
            f"{metric.max_qs:>8} | "
            f"{metric.tiempo:>11} | "
            f"{metric.generados:>9}")
    
if __name__ == "__main__":
    main()