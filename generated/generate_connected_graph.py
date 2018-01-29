import numpy as np
import sys


def main():
    size = len(sys.argv)

    if size == 1:
        N = 10
        S = 2
        range_scenario_min = 1
        range_scenario_max = 10
        part = 1
    elif size == 3:
        try:
            N = int(sys.argv[1])
            S = int(sys.argv[2])

            if N < 3 or S < 1:
                print("Invalid values!\nUsage: generate_connected_graph.py [10 1 [1 10]]")
                sys.exit()
        except (ValueError, TypeError):
            print("Invalid values!\nUsage: generate_connected_graph.py [10 1 [1 10]]")
            sys.exit()

        range_scenario_min = 1
        range_scenario_max = 10
        part = 1
    elif size == 5 or size == 6:
        try:
            N = int(sys.argv[1])
            S = int(sys.argv[2])
            range_scenario_min = int(sys.argv[3])
            range_scenario_max = int(sys.argv[4])
            part = int(sys.argv[5])

            if N < 3 or S < 1 or range_scenario_min < 1 or range_scenario_max < range_scenario_min:
                print("Invalid values!\nUsage: generate_connected_graph.py [10 1 [1 10]]")
                sys.exit()
        except (ValueError, TypeError):
            print("Invalid values!\nUsage: generate_connected_graph.py [10 1 [1 10]]")
            sys.exit()
    else:
        print("Usage: generate_connected_graph.py [10 1 [1 10]]")
        sys.exit()

    generate(N, S, range_scenario_min, range_scenario_max, part)

def generate(N, S, range_scenario_min, range_scenario_max, part):
    filename = str(N) + "_" + str(S) + "_" + str(range_scenario_min) + "_" + str(range_scenario_max) + "_" + str(part) + ".txt"
    node = str(N) + " " + str(S) + "\n"

    with open(filename, 'w') as file:
        file.write(node)

        for n1 in range(N):
            for n2 in range(n1 + 1, N):
                node = str(n1) + " " + str(n2)

                for _ in range(S):
                    node += " " + str(np.random.randint(range_scenario_min, range_scenario_max))

                node += "\n"
                file.write(node)


if __name__ == '__main__':
    main()
