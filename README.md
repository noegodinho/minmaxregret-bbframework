# Branch-and-Bound Framework for the Minimum Spanning Tree Min-Max Regret problem

This framework is an implementation in C++ to solve the **Min-Max Regret Problem** with several scenarios in undirected graphs. For each scenario individually, a **Minimum Spanning Tree**, in a custom Prim implementation, is solved to obtain the best value. Then, it is applied the **Branch-and-Bound** technique by enumerating all possible Spanning Trees, with **DFS** in the **branching** and two possible **bounds** to cut possible solutions that will not provably be better than the best found so far.

This is composed by several folders:

* **B&B**: This contains the source, headers and binaries for my custom implementation.
* **boost_prim**: This contains an implementation using the Prim algorithm in the [**Boost Library**](http://www.boost.org/).
* **generated**: In this folder, there are the test cases and the python script to generate them.
* **generic**: This contains an implementation of the Pseudo-polynomial algorithm for the Min-Max Regret by [Aissi et al.](http://www.lamsade.dauphine.fr/~bazgan/Papers/isora05.pdf)
* **prim**: This contains the custom prim implementation alone.
* **Tests**: This folder contains all the generated files for testing.

# B&B

This folder is composed by:

* A makefile
* A bin folder containing the binary
* A lib folder containing the headers
* A src folder containing the source

The makefile has several debug flags (**DEBUG_FLAGS**), a result flag (**RESULT_BB**), a sort of the values flag (**SORT**), a thread flag (**THREAD**), an initial solution for the Branch-and-Bound (**INITIAL**), and a flag for the bounds implemented (**BOUNDS**).

To compile, it is only necessary to choose the intended flags and run the following commands:

    make clean
    make

Then, to execute, from this folder:

    ./bin/main < ../generated/generated_test_file.txt

where **generated_test_file.txt** is the file generated.

# boost_prim

This folder contains the C++ implementation of the Prim algorithm using the [**Boost Library**](http://www.boost.org/).

To compile and execute, it is only necessary to replace the test file in the **run.sh** file.

To execute, just run:

    sh run.sh

# generated

This contains an script in **Python3** of the test cases generator.

To execute, just run:

    python3 generate_connected_graph.py A B [C D E]

where, **A** is the number of nodes in the graph, **B** is the number of scenarios, **C** is the minimum possible value in each edge, **D** is the maximum possible value in each edge, and **E** defines a file part.

This generator uses the **NumPy Library** for the random generator.

To install it, just run:

    sudo -H pip install numpy

# generic

This folder contains the implementation of the Pseudo-polynomial algorithm proposed by [Aissi et al.](http://www.lamsade.dauphine.fr/~bazgan/Papers/isora05.pdf)

To compile, just run:
    
    make clean
    make

And to execute, just run:
    
    ./generic < ../generated/generated_test_file.txt

where **generated_test_file.txt** is the file generated.

This implementation uses the [**GiNaC Library**](https://ginac.de/).

# prim

This folder contains the custom implementation of the Prim algorithm.

To compile, just run:
    
    make clean
    make

And to execute, just run:
    
    ./generic < ../generated/generated_test_file.txt

where **generated_test_file.txt** is the file generated.

# Tests

This folder contains the **R** script to generate plots and information files of the framework runs.

To execute, just run:
    
    Rscript plot_files.r
