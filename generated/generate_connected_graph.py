import math
import numpy as np
import sys
from scipy.spatial import Delaunay


def main():
    size = len(sys.argv)
    error_message = "Invalid values!\nUsage: generate_connected_graph.py [10 2 [1 1000 [1 [1]]]]"

    if size == 1:
        N = 10
        S = 2
        range_scenario_min = 1
        range_scenario_max = 1000
        part = 1
        edge_probability = 1
    elif size == 3:
        try:
            N = int(sys.argv[1])
            S = int(sys.argv[2])

            if N < 3 or S < 1:
                print(error_message)
                sys.exit()
        except (ValueError, TypeError):
            print(error_message)
            sys.exit()

        range_scenario_min = 1
        range_scenario_max = 1000
        part = 1
        edge_probability = 1
    elif size == 5:
        try:
            N = int(sys.argv[1])
            S = int(sys.argv[2])
            range_scenario_min = int(sys.argv[3])
            range_scenario_max = int(sys.argv[4])

            if N < 3 or S < 1 or range_scenario_min < 1 or range_scenario_max < range_scenario_min:
                print(error_message)
                sys.exit()            
        except (ValueError, TypeError):
            print(error_message)
            sys.exit()

        part = 1
        edge_probability = 1
    elif size == 6:
        try:
            N = int(sys.argv[1])
            S = int(sys.argv[2])
            range_scenario_min = int(sys.argv[3])
            range_scenario_max = int(sys.argv[4])
            part = int(sys.argv[5])

            if N < 3 or S < 1 or range_scenario_min < 1 or range_scenario_max < range_scenario_min or part < 1:
                print(error_message)
                sys.exit()
        except (ValueError, TypeError):
            print(error_message)
            sys.exit()

        edge_probability = 1
    elif size == 7:
        try:
            N = int(sys.argv[1])
            S = int(sys.argv[2])
            range_scenario_min = int(sys.argv[3])
            range_scenario_max = int(sys.argv[4])
            part = int(sys.argv[5])
            edge_probability = float(sys.argv[6])

            if N < 3 or S < 1 or range_scenario_min < 1 or range_scenario_max < range_scenario_min or part < 1 or edge_probability <= 0 or edge_probability > 1:
                print(error_message)
                sys.exit()
        except (ValueError, TypeError):
            print(error_message)
            sys.exit()
    else:
        print(error_message)
        sys.exit()

    connected = False

    while not connected:
        print("Generating graph...")
        connected = generate_normal(N, S, range_scenario_min, range_scenario_max, part, edge_probability)

    connected = False

    while not connected:
        print("Generating graph...")
        connected = generate_euclidean(N, S, range_scenario_min, range_scenario_max, part, edge_probability)    

    print("Generating graph...")
    generate_planar(N, S, range_scenario_min, range_scenario_max, part, edge_probability)

    connected = False

    while not connected:
        print("Generating graph...")
        connected = generate_bipartite(N, S, range_scenario_min, range_scenario_max, part, edge_probability)

    print("Graphs generated successfully")


def generate_normal(N, S, range_scenario_min, range_scenario_max, part, edge_probability):
    filename = "normal/normal_" + str(N) + "_" + str(S) + "_" + str(range_scenario_min) + "_" + str(range_scenario_max) + "_"  + str(edge_probability) + "_" + str(part) + ".txt"
    node = str(N) + " " + str(S) + "\n"

    with open(filename, 'w') as file:
        file.write(node)
        graph = [[] for _ in range(N)]

        for n1 in range(N):
            for n2 in range(n1 + 1, N):
                if np.random.uniform(0, 1) < edge_probability:
                    node = str(n1) + " " + str(n2)

                    for _ in range(S):
                        node += " " + str(np.random.randint(range_scenario_min, range_scenario_max))

                    node += "\n"
                    file.write(node)
                    graph[n1].append(n2)
                    graph[n2].append(n1)

    if not dfs(graph, N):
        print("Graph generated not connected...")
        return False

    return True


def generate_euclidean(N, S, range_scenario_min, range_scenario_max, part, edge_probability):
    filename = "euclidean/euclidean_" + str(N) + "_" + str(S) + "_" + str(range_scenario_min) + "_" + str(range_scenario_max) + "_"  + str(edge_probability) + "_" + str(part) + ".txt"
    node = str(N) + " " + str(S) + "\n"

    with open(filename, 'w') as file:
        file.write(node)
        graph = [[] for _ in range(N)]
        points = [[] for _ in range(S)]

        for s in range(S):
            for _ in range(N):
                d1 = np.random.randint(range_scenario_min, range_scenario_max)
                d2 = np.random.randint(range_scenario_min, range_scenario_max)
                points[s].append([d1, d2])

        for n1 in range(N):
            for n2 in range(n1 + 1, N):
                if np.random.uniform(0, 1) < edge_probability:
                    node = str(n1) + " " + str(n2)                    

                    for s in range(S):
                        distance = round(math.sqrt((points[s][n1][0] - points[s][n2][0]) ** 2 + (points[s][n1][1] - points[s][n2][1]) ** 2))
                        node += " " + str(distance)

                    node += "\n"
                    file.write(node)
                    graph[n1].append(n2)
                    graph[n2].append(n1)

    if not dfs(graph, N):
        print("Graph generated not connected...")
        return False

    return True


def generate_planar(N, S, range_scenario_min, range_scenario_max, part, edge_probability):
    filename = "planar/planar_" + str(N) + "_" + str(S) + "_" + str(range_scenario_min) + "_" + str(range_scenario_max) + "_" + str(part) + ".txt"
    node = str(N) + " " + str(S) + "\n"    

    with open(filename, 'w') as file:
        file.write(node)
        graph = [[] for _ in range(N)]
        points = [[] for _ in range(S)]
        pointsnp = []
        tri = []

        for s in range(S):
            for _ in range(N):
                d1 = np.random.randint(range_scenario_min, range_scenario_max)
                d2 = np.random.randint(range_scenario_min, range_scenario_max)
                points[s].append([d1, d2])

            pointsnp.append(np.array(points[s]))
            tri.append(Delaunay(pointsnp[s]))

        for t in tri[0].simplices:
            if t[1] not in graph[t[0]]:
                graph[t[0]].append(t[1])
                graph[t[1]].append(t[0])

            if t[2] not in graph[t[0]]:
                graph[t[0]].append(t[2])
                graph[t[2]].append(t[0])

            if t[2] not in graph[t[1]]:
                graph[t[1]].append(t[2])
                graph[t[2]].append(t[1])

        visited = [[] for _ in range(N)]

        for i in range(N):
            visited[i] = [False] * N

        for n1 in range(N):
            for n2 in graph[n1]:             
                if visited[n1][n2]:
                    continue

                node = str(n1) + " " + str(n2)

                for s in range(S):
                    distance = round(math.sqrt((points[s][n1][0] - points[s][n2][0]) ** 2 + (points[s][n1][1] - points[s][n2][1]) ** 2))    
                    node += " " + str(distance)

                node += "\n"
                file.write(node)
                visited[n1][n2] = True
                visited[n2][n1] = True                


def generate_bipartite(N, S, range_scenario_min, range_scenario_max, part, edge_probability):
    filename = "bipartite/bipartite_" + str(N) + "_" + str(S) + "_" + str(range_scenario_min) + "_" + str(range_scenario_max) + "_"  + str(edge_probability) + "_" + str(part) + ".txt"
    node = str(N) + " " + str(S) + "\n"

    graph = [[] for _ in range(N)]

    graph_1 = {}
    graph_2 = {}

    for n1 in range(N):
        if np.random.uniform(0, 1) < 0.5:
            graph_1[n1] = []
        else:
            graph_2[n1] = []

    with open(filename, 'w') as file:
        file.write(node)

        for key1, value1 in graph_1.items():
            for key2, value2 in graph_2.items():
                if np.random.uniform(0, 1) < edge_probability:
                    node = str(key1) + " " + str(key2)

                    for _ in range(S):
                        node += " " + str(np.random.randint(range_scenario_min, range_scenario_max))

                    node += "\n"
                    file.write(node)

                    value1.append(key2)
                    value2.append(key1)

    g = {**graph_1, **graph_2}

    for key, value in g.items():
        graph[key] = value

    if not dfs(graph, N):
        print("Graph generated not connected...")
        return False

    return True


def dfs(graph, N):
    visited = [False] * N
    stack = []

    stack.append(0)

    while stack:
        node = stack.pop()

        if not visited[node]:
            visited[node] = True

            for n in graph[node]:
                stack.append(n)

    if False in visited:
        return False

    return True


if __name__ == '__main__':
    main()
