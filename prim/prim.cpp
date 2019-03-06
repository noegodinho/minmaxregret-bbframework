#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include "../B&B/lib/structs.hpp"


/* only uses one scenario */

void prim(std::vector <struct edge> *edges,
          std::vector <long> weights,
          unsigned number_of_nodes);
void show_result(struct mst mst[], unsigned number_of_nodes);


int main(){
    unsigned number_of_nodes;
    unsigned number_of_scenarios;
    unsigned node1;
    unsigned node2;
    long it = 0;
    long value;
    std::vector <struct edge> *edges;
    std::vector <long> weights;
    std::size_t k;

    struct edge temp_edge;
    struct timespec start, end;

    if(scanf("%u %u", &number_of_nodes, &number_of_scenarios) != 2){
        perror("Error: ");
    }

    value = ((number_of_nodes * number_of_nodes)
            - number_of_nodes) / 2;

    edges = new std::vector <struct edge> [number_of_nodes]();

    for(k = 0; k < number_of_nodes; ++k){
        edges[k].reserve(number_of_nodes - 1);
    }

    weights.resize(value);

    while(scanf("%u %u", &node1, &node2) != EOF){
        temp_edge.visited = false;
        temp_edge.node_connected = node2;
        temp_edge.weight_index = it;
        edges[node1].push_back(temp_edge);
        temp_edge.node_connected = node1;
        edges[node2].push_back(temp_edge);

        if(scanf("%ld", &weights[it]) != 1){
            perror("Error: ");
        }

        ++it;
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    prim(edges, weights, number_of_nodes);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    printf("%lf\n", ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));

    delete[] edges;

    return 0;
}

/*
 * prim algorithm method for individual MST
 * used to compare performance with boost prim algorithm 
 */
void prim(std::vector <struct edge> *edges,
          std::vector <long> weights,
          unsigned number_of_nodes){

    std::size_t i;
    unsigned num_nodes = 0;
    unsigned temp_node_connected;
    unsigned temp_weight;
    unsigned node_value;

    struct mst mst[number_of_nodes];
    bool visited[number_of_nodes];
    std::priority_queue <struct scenario_node> nodes;
    struct scenario_node node;

    struct mst fill_mst;

    fill_mst.parent = -1;
    fill_mst.weight = LONG_MAX;

    std::fill(mst, mst + number_of_nodes, fill_mst);
    std::fill(visited, visited + number_of_nodes, false);

    mst[0].weight = 0;

    node.node = 0;
    node.key = 0;
    nodes.push(node);

    while(num_nodes < number_of_nodes){
        node = nodes.top();
        nodes.pop();
        node_value = node.node;

        if(visited[node_value]){
            continue;
        }

        visited[node_value] = true;
        ++num_nodes;

        for(i = 0; i < edges[node_value].size(); ++i){
            temp_node_connected = edges[node_value][i].node_connected;
            temp_weight = edges[node_value][i].weight_index;

            if(!visited[temp_node_connected]
                && weights[temp_weight] < mst[temp_node_connected].weight){

                mst[temp_node_connected].weight = node.key = weights[temp_weight];
                mst[temp_node_connected].parent = node_value;

                node.node = temp_node_connected;
                nodes.push(node);
            }
        }
    }

    #ifdef RESULT_PRIM
        show_result(mst, number_of_nodes);
    #endif
}

void show_result(struct mst mst[], unsigned number_of_nodes){
    long sum = 0;

    printf("\n");

    for(std::size_t i = 0; i < number_of_nodes; ++i){
        printf("Parent[%zu] = %d, weight = %ld\n", i, mst[i].parent, mst[i].weight);
        sum += mst[i].weight;
    }

    printf("\nTotal weight: %ld\n\n", sum);
}
