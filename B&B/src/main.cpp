#include "../lib/MMRMST.hpp"


long MMRMST::best_weight;
long *MMRMST::optimal_scenarios;
long *MMRMST::best_case;
struct timespec MMRMST::start;


int main(int argc, char **argv){
    struct timespec start, end;
    unsigned number_of_nodes;
    unsigned number_of_scenarios;
    long value;
    std::vector <struct edge> *edges;
    std::vector <long> *weights;
    std::size_t k;
    std::size_t it = 0;
    unsigned node1;
    unsigned node2;
    double time_value;

    #ifdef INITIAL// || defined(INITIAL_W)
        double *w;
    #endif

    #ifdef INITIAL_W
        double **w;
    #endif

    struct edge temp_edge;

    if(scanf("%u %u", &number_of_nodes, &number_of_scenarios) != 2){
        perror("Error: ");
    }

    value = ((number_of_nodes * number_of_nodes)
            - number_of_nodes) / 2;

    edges = new std::vector <struct edge> [number_of_nodes]();
    weights = new std::vector <long> [number_of_scenarios]();

    for(k = 0; k < number_of_nodes; ++k){
        edges[k].reserve(number_of_nodes - 1);
    }

    for(k = 0; k < number_of_scenarios; ++k){
        weights[k].resize(value);
    }

    while(scanf("%u %u", &node1, &node2) != EOF){
        for(k = 0; k < number_of_scenarios; ++k){
            if(scanf("%ld", &weights[k][it]) != 1){
                perror("Error: ");
            }
        }

        temp_edge.visited = false;
        temp_edge.node_connected = node2;
        temp_edge.weight_index = it;
        edges[node1].push_back(temp_edge);
        temp_edge.node_connected = node1;
        edges[node2].push_back(temp_edge);

        ++it;
    }

    #ifdef INITIAL
        w = new double[number_of_scenarios];

        for(it = 0; it < number_of_scenarios; ++it){
            w[it] = 1.0 / number_of_scenarios;
        }
    #endif

    #ifdef INITIAL_W
        w = new double *[number_of_scenarios + 1];

        for(it = 0; it < number_of_scenarios + 1; ++it){
            w[it] = new double[number_of_scenarios];
        }

        for(it = 0; it < number_of_scenarios; ++it){
            for(k = 0; k < number_of_scenarios; ++k){
                if(it == k){
                    w[it][k] = (2.0*number_of_scenarios - number_of_scenarios + 1.0) / (2.0*number_of_scenarios);
                }

                else{
                    w[it][k] = 1.0 / (2.0*number_of_scenarios);
                }
            }
        }

        for(it = 0; it < number_of_scenarios; ++it){
            w[number_of_scenarios][it] = 1.0 / number_of_scenarios;
        }
    #endif

    MMRMST MMRMST_instance(edges, weights, number_of_nodes, number_of_scenarios);

    MMRMST_instance.Allocate();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    MMRMST_instance.setTime(start);
    MMRMST_instance.Optimal();

    #ifdef INITIAL
        MMRMST_instance.InitialSolution(w);
    #endif

    #ifdef INITIAL_W
        for(it = 0; it < number_of_scenarios + 1; ++it){
            MMRMST_instance.InitialSolution(w[it]);
        }
    #endif

    MMRMST_instance.StartBranch(0);
    MMRMST_instance.WriteSolToFile(argv[1]);
    MMRMST_instance.Delete();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    time_value = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);

    if(time_value >= 1800.0){
        printf("1800.000000\n");
    }

    else{
        printf("%lf\n", time_value);
    }

    delete[] edges;
    delete[] weights;

    #ifdef INITIAL
        delete[] w;
    #endif

    #ifdef INITIAL_W
        for(it = 0; it < number_of_scenarios + 1; ++it){
            delete[] w[it];
        }
    #endif

    return 0;
}
