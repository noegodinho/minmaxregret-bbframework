#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>

/* only uses one scenario */

using namespace boost;

int main(int argc, char **argv){
    unsigned num_nodes;
    unsigned dummy;
    unsigned node1;
    unsigned node2;
    unsigned size;
    std::size_t it = 0; 

    typedef adjacency_list <vecS, vecS, undirectedS, property <vertex_distance_t, int>, property <edge_weight_t, int> > Graph;
    typedef std::pair <unsigned, unsigned> E;

    E *edges;
    unsigned *weights;

    struct timespec start, end;

    if(scanf("%u %u", &num_nodes, &dummy) != 2){
        perror("Error: ");
    }

    size = ((num_nodes * num_nodes) - num_nodes) / 2;

    edges = new E[size];
    weights = new unsigned[size];

    while(scanf("%u %u", &node1, &node2) != EOF){
        edges[it] = std::make_pair(node1, node2);

        if(scanf("%u", &weights[it]) != 1){
            perror("Error: ");
        }

        ++it;
    }

    Graph g(edges, edges + size, weights, num_nodes);
    std::vector <graph_traits <Graph>::vertex_descriptor> p(num_vertices(g));

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    prim_minimum_spanning_tree(g, &p[0]);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    #ifdef RESULT
        unsigned long sum = 0;
        unsigned long weight;
        property_map <Graph, edge_weight_t>::type weightmap = get(edge_weight, g); 

        printf("\nParent[0] = -1, weight = 0\n");

        for (std::size_t i = 1; i != p.size(); ++i){
            weight = get(weightmap, edge(p[i], i, g).first);
            printf("Parent[%zu] = %lu, weight = %lu\n", i, p[i], weight);
            sum += weight;
        }

        printf("\nTotal weight: %lu\n\n", sum);
    #endif

    printf("%lf\n", ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));

    delete[] edges;
    delete[] weights;

    return EXIT_SUCCESS;
}
