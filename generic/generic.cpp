#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include <ginac/ginac.h>
#include "../B&B/lib/structs.hpp"


using namespace GiNaC;


long prim(std::vector <struct edge> *edges,
          std::vector <long> *weights,
          unsigned scenario,
          unsigned number_of_nodes);
ex calculate_determinant(std::vector <struct edge> *edges,
                         std::vector <long> weights,
                         unsigned number_of_nodes,
                         symbol x);
void generic_algorithm(long *optimal_scenarios,
                       long *pow_calc,
                       unsigned number_of_scenarios,
                       long max_v,
                       ex poly,
                       symbol x);
long calculate_value(long *pow_calc,
                     int alpha[],
                     unsigned k);
unsigned upow(unsigned base, unsigned num);


int main(){
    unsigned number_of_nodes;
    unsigned number_of_scenarios;
    unsigned node1;
    unsigned node2;
    long M = 0;
    long m;
    long max_f = 0;
    long const_calc;
    long it = 0;
    std::vector <struct edge> *edges;
    std::vector <long> *weights;
    std::vector <long> new_weights;
    unsigned k;
    long *optimal_scenarios;
    long *pow_calc;

    struct edge temp_edge;
    struct timespec start, end;
    ex poly;
    symbol x("x");

    if(scanf("%u %u", &number_of_nodes, &number_of_scenarios) != 2){
        perror("Error: ");
    }

    m = ((number_of_nodes * number_of_nodes)
        - number_of_nodes) / 2;

    edges = new std::vector <struct edge> [number_of_nodes]();
    weights = new std::vector <long> [number_of_scenarios]();
    optimal_scenarios = new long[number_of_scenarios]();
    pow_calc = new long[number_of_scenarios]();

    for(k = 0; k < number_of_nodes; ++k){
        edges[k].reserve(number_of_nodes - 1);
    }

    for(k = 0; k < number_of_scenarios; ++k){
        weights[k].reserve(m);
    }

    new_weights.resize(m);

    while(scanf("%u %u", &node1, &node2) != EOF){
        temp_edge.visited = false;
        temp_edge.node_connected = node2;
        temp_edge.weight_index = it;
        edges[node1].push_back(temp_edge);
        temp_edge.node_connected = node1;
        edges[node2].push_back(temp_edge);

        for(k = 0; k < number_of_scenarios; ++k){
            if(scanf("%ld", &weights[k][it]) != 1){
                perror("Error: ");
            }

            M = std::max(M, weights[k][it]);
        }

        ++it;
    }

    const_calc = m * M + 1;    

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    for(k = 0; k < number_of_scenarios; ++k){
        optimal_scenarios[k] = prim(edges, weights, k, number_of_nodes);
        max_f = std::max(max_f, optimal_scenarios[k]);
        pow_calc[k] = upow(const_calc, k);
    }

    for(it = 0; it < m; ++it){
        for(k = 0; k < number_of_scenarios; ++k){
            new_weights[it] += weights[k][it] * pow_calc[k];
        }
    }

    poly = calculate_determinant(edges, new_weights, number_of_nodes, x);
    generic_algorithm(optimal_scenarios, pow_calc, number_of_scenarios, m * M - max_f, poly, x);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    printf("%lf\n", ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));

    delete[] edges;
    delete[] weights;
    delete[] optimal_scenarios;
    delete[] pow_calc;

    return 0;
}

long prim(std::vector <struct edge> *edges,
          std::vector <long> *weights,
          unsigned scenario,
          unsigned number_of_nodes){

    std::size_t i;
    unsigned num_nodes = 0;
    unsigned temp_node_connected;
    unsigned temp_weight;
    unsigned node_value;
    long sum = 0;

    struct mst mst[number_of_nodes];
    bool visited[number_of_nodes];

    std::priority_queue <struct scenario_node> nodes;
    struct scenario_node node;

    struct mst fill_mst;

    fill_mst.parent = -1;
    fill_mst.weight = LONG_MAX;

    std::fill(mst, mst + number_of_nodes, fill_mst);
    std::fill(visited, visited + number_of_nodes, false);

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
                && weights[scenario][temp_weight] < mst[temp_node_connected].weight){
                
                mst[temp_node_connected].weight = node.key = weights[scenario][temp_weight];
                mst[temp_node_connected].parent = node_value;

                node.node = temp_node_connected;
                nodes.push(node);
            }
        }
    }

    for(i = 1; i < number_of_nodes; ++i){
        sum += mst[i].weight;
    }

    return sum;
}

ex calculate_determinant(std::vector <struct edge> *edges,
                         std::vector <long> weights,
                         unsigned number_of_nodes,
                         symbol x){

    matrix N(number_of_nodes - 1, number_of_nodes - 1);

    std::size_t i;
    std::size_t j;
    unsigned node;

    for(i = 0; i < number_of_nodes - 1; ++i){
        for(j = i + 1; j < edges[i].size(); ++j){
            node = edges[i + 1][j].node_connected - 1;

            N(i, node) = N(node, i) = -pow(x, weights[edges[i + 1][j].weight_index]);
            N(i, i) -= N(i, node);
        }

        for(j = 1; j < i + 1; ++j){
            N(i, i) -= N(i, edges[i + 1][j].node_connected - 1);
        }

        N(i, i) -= -pow(x, weights[edges[0][i].weight_index]);
    }

    return N.determinant();
}

void generic_algorithm(long *optimal_scenarios,
                       long *pow_calc,
                       unsigned number_of_scenarios,
                       long max_v,
                       ex poly,
                       symbol x){

    int i;
    int j;
    long value;
    long v;
    long val;
    bool test = false;

    int alpha[number_of_scenarios];    

    for(v = 0; !test && v <= max_v; ++v){
        for(i = optimal_scenarios[0]; !test && i - optimal_scenarios[0] <= v; ++i){
            alpha[0] = i;

            val = optimal_scenarios[1];

            if(i - optimal_scenarios[0] < v){
                val += v;
            }

            for(j = val; j - optimal_scenarios[1] <= v; ++j){
                alpha[1] = j;
                value = calculate_value(pow_calc, alpha, number_of_scenarios);

                if(poly.coeff(x, value) > 0){
                    test = true;
                    break;
                }
            }
        }
    }

    #ifdef RESULT
        printf("\nBest Weight: %ld\n\n", v - 1);
    #endif
}

inline long calculate_value(long *pow_calc,
                            int alpha[],
                            unsigned k){

    long value = 0;    

    while(k--){
        value += alpha[k] * pow_calc[k];        
    }

    return value;
}

unsigned upow(unsigned base, unsigned num){
    unsigned result = 1;

    while(num--){
        result *= base;
    }

    return result;
}
