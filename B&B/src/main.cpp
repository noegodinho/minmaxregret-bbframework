#include "../lib/MMRMST.hpp"


long MMRMST::best_weight;
long *MMRMST::optimal_scenarios;
long *MMRMST::best_case;
struct timespec MMRMST::start;

#ifdef THREAD
    bool MMRMST::terminate = false;
#endif


int main(){
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

    #ifdef THREAD
        const static unsigned num_threads = 4;
        unsigned i;
        std::thread t[num_threads];
        MMRMST MMRMST_instance[num_threads]{{edges, weights, number_of_nodes, number_of_scenarios, 0.8},
                                            {edges, weights, number_of_nodes, number_of_scenarios, 0.6},
                                            {edges, weights, number_of_nodes, number_of_scenarios, 0.4},
                                            {edges, weights, number_of_nodes, number_of_scenarios, 0.2}};

        MMRMST_instance[0].Allocate();
        clock_gettime(CLOCK_REALTIME, &start);

        MMRMST_instance[0].setTime(start);
        MMRMST_instance[0].Optimal();

        #ifdef INITIAL
            for(i = 0; i < num_threads; ++i){
                MMRMST_instance[i].InitialSolution();
            }
        #endif

        for(i = 0; i < num_threads; ++i){
            t[i] = std::thread(&MMRMST::StartBranch, &MMRMST_instance[i], i);
        }

        for(i = 0; i < num_threads; ++i){
            t[i].join();
        }

        MMRMST_instance[0].Delete();
        clock_gettime(CLOCK_REALTIME, &end);
    #else
        MMRMST MMRMST_instance(edges, weights, number_of_nodes, number_of_scenarios, 0.5);

        MMRMST_instance.Allocate();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        MMRMST_instance.setTime(start);
        MMRMST_instance.Optimal();

        #ifdef INITIAL
            MMRMST_instance.InitialSolution();
        #endif

        MMRMST_instance.StartBranch(0);
        MMRMST_instance.Delete();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    #endif    

    time_value = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);

    if(time_value >= 3600.0){
        printf("3600.000000\n");
    }

    else{
        printf("%lf\n", time_value);
    }

    delete[] edges;
    delete[] weights;

    return 0;
}
