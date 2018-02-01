import numpy as np
import sys


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
        connected = generate(N, S, range_scenario_min, range_scenario_max, part, edge_probability)

    print("Graph generated successfully")


def generate(N, S, range_scenario_min, range_scenario_max, part, edge_probability):
    filename = str(N) + "_" + str(S) + "_" + str(range_scenario_min) + "_" + str(range_scenario_max) + "_"  + str(edge_probability) + "_" + str(part) + ".txt"
    node = str(N) + " " + str(S) + "\n"

    with open(filename, 'w') as file:
        file.write(node)
        graph = [[] for i in range(N)]

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
