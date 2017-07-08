#include "../lib/MMRMST.hpp"


MMRMST::MMRMST(std::vector <struct edge> *edges,
               std::vector <long> *weights,
               unsigned number_of_nodes,
               unsigned number_of_scenarios,
               double p){

    std::size_t i;

    #ifdef SORT
        std::size_t j;
        std::size_t k;
        long sum;
        double q;
    #endif

    this->number_of_nodes = number_of_nodes;
    this->number_of_scenarios = number_of_scenarios;
    this->p = p;

    this->edges = new std::vector <struct edge> [this->number_of_nodes]();
    this->weights = new std::vector <long> [this->number_of_scenarios]();
    this->visited = new bool[this->number_of_nodes]();
    this->preorder_visited = new int[this->number_of_nodes]();
    this->highest = new int[this->number_of_nodes]();
    this->list_of_edges.reserve(this->number_of_nodes - 1);
    this->actual_st.reserve(this->number_of_nodes - 1);

    for(i = 0; i < this->number_of_scenarios; ++i){
        this->weights[i] = weights[i];
    }

    for(i = 0; i < this->number_of_nodes; ++i){
        this->edges[i] = edges[i];

        #ifdef SORT
            for(j = 0; j < this->edges[i].size(); ++j){
                sum = 0;
                q = p;

                for(k = 0; k < this->number_of_scenarios; ++k){
                    sum += q * this->weights[k][this->edges[i][j].weight_index];
                    q = 1.0 - q;
                }

                this->edges[i][j].sum = sum;
            }

            std::sort(this->edges[i].begin(), this->edges[i].end());
        #endif
    }
}

MMRMST::~MMRMST(){
    delete[] this->edges;
    delete[] this->weights;
    delete[] this->visited;
    delete[] this->preorder_visited;
    delete[] this->highest;
}

void MMRMST::Delete(){
    delete[] this->best_case;
    delete[] this->optimal_scenarios;
}

void MMRMST::Allocate(){
    this->best_case = new long[this->number_of_scenarios]();
    this->optimal_scenarios = new long[this->number_of_scenarios]();

    this->best_weight = LONG_MAX;
}

void MMRMST::setTime(struct timespec start){
    this->start = start;
}

void MMRMST::Optimal(){
    std::size_t i;

    for(i = 0; i < this->number_of_scenarios; ++i){
        this->optimal_scenarios[i] = this->Prim(i);
    }
}

void MMRMST::StartBranch(unsigned node){
    std::size_t i;
    long *actual_best_scenarios;

    #ifdef INITIAL
        for(i = 0; i < this->number_of_nodes; ++i){
            this->preorder_visited[i] = this->highest[i] = i;
        }
    #endif

    actual_best_scenarios = new long[this->number_of_scenarios]();

    this->visited[node] = true;

    for(i = 0; i < this->edges[node].size(); ++i){
        this->list_of_edges.push_back(std::make_pair(node, this->edges[node][i]));
    }

    this->Branch(1, actual_best_scenarios);

    #ifdef RESULT_B_B
        this->ShowResultBranch();
    #endif

    delete[] actual_best_scenarios;
    
    #ifdef THREAD
        this->terminate = true;
    #endif
}

/* 
 * prim algorithm method to obtain the optimal solution
 * for every individual scenario
 * and to posterior use for bound 2
 */
long MMRMST::Prim(unsigned scenario){
    std::size_t i;
    unsigned num_nodes = 0;
    unsigned temp_node_connected;
    unsigned temp_weight;
    unsigned node_value;
    long sum = 0;

    struct mst mst[this->number_of_nodes];
    bool visited[this->number_of_nodes];

    std::priority_queue <struct scenario_node> nodes;
    struct scenario_node node;

    struct mst fill_mst;

    fill_mst.parent = -1;
    fill_mst.weight = LONG_MAX;

    std::fill(mst, mst + this->number_of_nodes, fill_mst);
    std::copy(this->visited, this->visited + this->number_of_nodes, visited);

    node.node = 0;
    node.key = 0;
    nodes.push(node);

    while(num_nodes < this->number_of_nodes){
        node = nodes.top();
        nodes.pop();
        node_value = node.node;

        if(visited[node_value]){
            continue;
        }

        visited[node_value] = true;
        ++num_nodes; 

        for(i = 0; i < this->edges[node_value].size(); ++i){
            temp_node_connected = this->edges[node_value][i].node_connected;
            temp_weight = this->edges[node_value][i].weight_index;

            if(!visited[temp_node_connected]
                && this->weights[scenario][temp_weight] < mst[temp_node_connected].weight){
                
                mst[temp_node_connected].weight = node.key = this->weights[scenario][temp_weight];
                mst[temp_node_connected].parent = node_value;

                node.node = temp_node_connected;
                nodes.push(node);
            }
        }
    }

    for(i = 1; i < this->number_of_nodes; ++i){
        sum += mst[i].weight;
    }

    return sum;
}

inline long MMRMST::Prim2(unsigned scenario){
    std::size_t i;
    unsigned num_nodes = 0;
    unsigned temp_node_connected;
    unsigned temp_weight;
    unsigned node_value;
    long sum = 0;

    struct mst mst[this->number_of_nodes];
    bool visited[this->number_of_nodes];

    std::priority_queue <struct scenario_node> nodes;
    struct scenario_node node;

    struct mst fill_mst;

    fill_mst.parent = -1;
    fill_mst.weight = LONG_MAX;

    std::fill(mst, mst + this->number_of_nodes, fill_mst);
    std::fill(visited, visited + this->number_of_nodes, false);

    for(i = 0; i < this->actual_st.size(); ++i){
        node_value = this->actual_st[i].first;
        temp_node_connected = this->actual_st[i].second.node_connected;
        temp_weight = this->actual_st[i].second.weight_index;
        
        mst[temp_node_connected].weight = this->weights[scenario][temp_weight];
        mst[temp_node_connected].parent = node_value;
        node.node = temp_node_connected;
        node.key = 0;
        nodes.push(node);
    }

    node.node = 0;
    node.key = 0;
    nodes.push(node);

    while(num_nodes < this->number_of_nodes){
        node = nodes.top();
        nodes.pop();
        node_value = node.node;

        if(visited[node_value]){
            continue;
        }

        visited[node_value] = true;
        ++num_nodes;

        for(i = 0; i < this->edges[node_value].size(); ++i){
            temp_node_connected = this->edges[node_value][i].node_connected;
            temp_weight = this->edges[node_value][i].weight_index;

            if(this->visited[temp_node_connected]
                && !visited[temp_node_connected]){

                node.node = temp_node_connected;
                nodes.push(node);
            }         

            else if(!visited[temp_node_connected]
                && this->weights[scenario][temp_weight] < mst[temp_node_connected].weight){
                
                mst[temp_node_connected].weight = node.key = this->weights[scenario][temp_weight];
                mst[temp_node_connected].parent = node_value;

                node.node = temp_node_connected;
                nodes.push(node);
            }            
        }
    }

    for(i = 0; i < this->number_of_nodes; ++i){
        if(mst[i].weight != LONG_MAX){
            sum += mst[i].weight;            
        }
    }

    return sum;
}

void MMRMST::InitialSolution(){
    std::size_t i;
    std::size_t j;
    unsigned num_nodes = 0;
    unsigned temp_node_connected;
    unsigned temp_weight;
    unsigned node_value;
    long actual_weight;
    
    struct mst mst[this->number_of_nodes];
    bool visited[this->number_of_nodes];
    long best_case[this->number_of_scenarios];
    long best_weight;
    double q;

    std::priority_queue <struct scenario_node> nodes;
    struct scenario_node node;

    struct mst fill_mst;

    fill_mst.parent = -1;
    fill_mst.weight = LONG_MAX;

    std::fill(mst, mst + this->number_of_nodes, fill_mst);
    std::fill(best_case, best_case + this->number_of_scenarios, 0);
    std::copy(this->visited, this->visited + this->number_of_nodes, visited);

    node.node = 0;
    node.key = 0;
    nodes.push(node);

    while(num_nodes < this->number_of_nodes){
        node = nodes.top();
        nodes.pop();
        node_value = node.node;

        if(visited[node_value]){
            continue;
        }

        visited[node_value] = true;
        ++num_nodes; 

        for(i = 0; i < this->edges[node_value].size(); ++i){
            temp_node_connected = this->edges[node_value][i].node_connected;
            temp_weight = this->edges[node_value][i].weight_index;
            actual_weight = 0;
            q = this->p;

            for(j = 0; j < this->number_of_scenarios; ++j){                
                actual_weight += q * this->weights[j][temp_weight];
                q = 1.0 - this->p;
            }

            if(!visited[temp_node_connected]
                && actual_weight < mst[temp_node_connected].weight){
                
                mst[temp_node_connected].weight = node.key = actual_weight;
                mst[temp_node_connected].parent = node_value;
                mst[temp_node_connected].weight_index = temp_weight;

                node.node = temp_node_connected;
                nodes.push(node);
            }
        }
    }

    for(i = 1; i < this->number_of_nodes; ++i){        
        for(j = 0; j < this->number_of_scenarios; ++j){
            best_case[j] += this->weights[j][mst[i].weight_index];
        }
    }

    best_weight = LONG_MIN;

    for(i = 0; i < this->number_of_scenarios; ++i){
        best_weight = std::max(best_weight,
                               best_case[i]
                               - this->optimal_scenarios[i]);
    }

    if(best_weight < this->best_weight){
        this->best_weight = best_weight;

        for(i = 0; i < this->number_of_scenarios; ++i){
            this->best_case[i] = best_case[i];
        }
    }

    #ifdef RESULT_B_B
        this->ShowResultInitial();
    #endif 
}

inline void MMRMST::Swap(std::size_t i){    
    std::pair <unsigned, struct edge> temp_pair;
    std::size_t j;

    for(j = i; j < this->list_of_edges.size() - 1; ++j){
        temp_pair = this->list_of_edges[j + 1];
        this->list_of_edges[j + 1] = this->list_of_edges[j];
        this->list_of_edges[j] = temp_pair;
    }

    this->list_of_edges.pop_back();
}

inline void MMRMST::InverseSwap(std::size_t index){
    std::pair <unsigned, struct edge> temp_pair;
    std::size_t j;

    for(j = this->list_of_edges.size(); j --> (index + 1);){
        temp_pair = this->list_of_edges[j - 1];
        this->list_of_edges[j - 1] = this->list_of_edges[j];
        this->list_of_edges[j] = temp_pair;
    }
}

void MMRMST::CalculatePreorder(unsigned w, unsigned actual_node){
    std::size_t i;

    this->preorder_visited[actual_node] = this->count++;
    this->highest[actual_node] = this->preorder_visited[actual_node];

    if(this->count > this->number_of_nodes){
        return;
    }

    for(i = 0; i < this->actual_st.size(); ++i){
        if(this->preorder_visited[this->actual_st[i].first] == 0
           && this->actual_st[i].second.node_connected == actual_node){

            this->CalculatePreorder(actual_node, this->actual_st[i].first);
            this->highest[actual_node] = std::max(this->highest[actual_node], 
                                                  this->highest[this->actual_st[i].first]);

            break;
        }

        else if(w != this->actual_st[i].first){
            this->highest[actual_node] = std::max(this->highest[actual_node],
                                                  this->preorder_visited[this->actual_st[i].first]);
        }

        if(this->preorder_visited[this->actual_st[i].second.node_connected] == 0
                && this->actual_st[i].first == actual_node){

            this->CalculatePreorder(actual_node, this->actual_st[i].second.node_connected);
            this->highest[actual_node] = std::max(this->highest[actual_node], 
                                                  this->highest[this->actual_st[i].second.node_connected]);

            break;
        }

        else if(w != this->actual_st[i].second.node_connected){
            this->highest[actual_node] = std::max(this->highest[actual_node],
                                                  this->preorder_visited[this->actual_st[i].second.node_connected]);
        }
    }
}

inline void MMRMST::CalculateWeight(long *actual_best_scenarios,
                                    long calculated_weight){
    if(calculated_weight < this->best_weight){
        this->best_weight = calculated_weight;
        this->ReplaceBest(actual_best_scenarios);
    }

    else if(calculated_weight == this->best_weight){
        this->Sum(actual_best_scenarios);
    }
}

inline long MMRMST::CalculateWeight(long *actual_best_scenarios){
    std::size_t i;
    long calculated_weight = LONG_MIN;

    for(i = 0; i < this->number_of_scenarios; ++i){
        calculated_weight = std::max(calculated_weight,
                                     actual_best_scenarios[i]
                                     - this->optimal_scenarios[i]);
    }

    return calculated_weight;
}

inline long MMRMST::Bound2(){
    std::size_t i;
    long calculated_weight_2 = LONG_MIN;
    long weight;

    for(i = 0; i < this->number_of_scenarios; ++i){
        weight = this->Prim2(i)
                 - this->optimal_scenarios[i];

        calculated_weight_2 = std::max(calculated_weight_2, weight);
    }

    return calculated_weight_2;
}

inline void MMRMST::MarkEdges(unsigned actual_node,
                              unsigned new_node,
                              bool value){

    #ifdef SORT
        for(std::size_t i = 0; i < this->edges[actual_node].size(); ++i){
            if(this->edges[actual_node][i].node_connected == new_node){
                this->edges[actual_node][i].visited = value;
                break;
            }
        }

        for(std::size_t i = 0; i < this->edges[new_node].size(); ++i){
            if(this->edges[new_node][i].node_connected == actual_node){
                this->edges[new_node][i].visited = value;
                break;
            }
        }
    #else
        if(actual_node > new_node){
            this->edges[actual_node][new_node].visited = value;
            this->edges[new_node][actual_node - 1].visited = value;
        }

        else{
            this->edges[actual_node][new_node - 1].visited = value;
            this->edges[new_node][actual_node].visited = value;
        }
    #endif
}

/* 
 * in case there is more than one best solution
 * is going to be saved the minimal for all scenarios
 * 
 * example: 
 * optimal S1: 3
 * optimal S2: 13
 * bests: 8, 14 - 8, 15
 * saved: 8, 14
 */
inline void MMRMST::Sum(long *actual_best_scenarios){
    unsigned long sum = 0;
    unsigned long sum2 = 0;

    for(std::size_t i = 0; i < this->number_of_scenarios; ++i){
        sum += this->best_case[i];
        sum2 += actual_best_scenarios[i];
    }

    if(sum == 0 || sum2 < sum){
        this->ReplaceBest(actual_best_scenarios);
    }
}

inline void MMRMST::ReplaceBest(long *actual_best_scenarios){
    for(std::size_t i = 0; i < this->number_of_scenarios; ++i){
        this->best_case[i] = actual_best_scenarios[i];
    }
}

inline void MMRMST::RestoreEdges(std::stack <std::pair<unsigned, struct edge> > backup_edges){
    unsigned actual_node;
    unsigned new_node;

    while(!backup_edges.empty()){
        actual_node = backup_edges.top().first;
        new_node = backup_edges.top().second.node_connected;

        this->MarkEdges(actual_node, new_node, false);
        
        this->list_of_edges.push_back(backup_edges.top());
        backup_edges.pop();
    }
}

void MMRMST::Branch(unsigned number_of_visited,
                    long *actual_best_scenarios){

    std::stack <std::pair<unsigned, struct edge> > backup_edges;
    std::stack <std::pair<std::size_t, std::pair<unsigned, struct edge> > > restore_edges;
    struct edge edge;
    bool b = false;
    unsigned actual_node;
    unsigned new_node;
    unsigned index;
    long calculated_weight = this->CalculateWeight(actual_best_scenarios);
    struct timespec end;
    double time_value;
    std::size_t i;

    #ifdef THREAD
        clock_gettime(CLOCK_REALTIME, &end);
    #else
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    #endif

    time_value = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - 
                 ((double)this->start.tv_sec + 1.0e-9*this->start.tv_nsec);

    if(time_value >= 3600.0){
        return;
    }

    if(number_of_visited >= this->number_of_nodes){
        this->count = 1;
        std::fill(this->preorder_visited, this->preorder_visited + this->number_of_nodes, 0);
        std::fill(this->highest, this->highest + this->number_of_nodes, 0);

        for(i = 0; i < this->number_of_nodes; ++i){
            if(this->preorder_visited[i] == 0){
                this->CalculatePreorder(i, i); 
            }            
        }        

        this->CalculateWeight(actual_best_scenarios, calculated_weight);
        return;
    }

    #ifdef THREAD
        if(this->terminate){
            return;
        }
    #endif    

    #ifdef BOUND1
        if(calculated_weight >= this->best_weight){
            return;
        }
    #endif

    #ifdef BOUND2
        if(this->number_of_nodes - number_of_visited > 1){
            if(this->Bound2() >= this->best_weight){
                return;
            }
        }
    #endif

    while(!b){
        if(this->list_of_edges.empty()){
            this->RestoreEdges(backup_edges);
            return;
        }

        actual_node = this->list_of_edges.back().first;
        edge = this->list_of_edges.back().second;
        new_node = edge.node_connected;
        index = edge.weight_index;
        this->actual_st.push_back(this->list_of_edges.back());
        this->list_of_edges.pop_back();

        this->visited[new_node] = true;

        for(i = 0; i < this->edges[new_node].size(); ++i){
            if(!this->visited[this->edges[new_node][i].node_connected]
               && !this->edges[new_node][i].visited){

                edge.node_connected = this->edges[new_node][i].node_connected;
                edge.weight_index = this->edges[new_node][i].weight_index;
                this->list_of_edges.push_back(std::make_pair(new_node, edge));
            }
        }

        for(i = 0; i < this->list_of_edges.size();){
            if(this->list_of_edges[i].second.node_connected == new_node
               && this->visited[this->list_of_edges[i].first]){
                
                restore_edges.push(std::make_pair(i, this->list_of_edges[i]));
                this->Swap(i);
            }

            else{
                ++i;
            }
        }
        
        for(i = 0; i < this->number_of_scenarios; ++i){
            actual_best_scenarios[i] += this->weights[i][index];
        }

        this->Branch(number_of_visited + 1,
                     actual_best_scenarios);

        for(i = 0; i < this->number_of_scenarios; ++i){
            actual_best_scenarios[i] -= this->weights[i][index];
        }

        for(i = 0; i < this->list_of_edges.size();){
            if(this->list_of_edges[i].first == new_node
               && !this->visited[this->list_of_edges[i].second.node_connected]){

                this->Swap(i);
            }

            else{
                ++i;
            }
        }

        while(!restore_edges.empty()){
            this->list_of_edges.push_back(restore_edges.top().second);
            this->InverseSwap(restore_edges.top().first);

            restore_edges.pop();
        }

        backup_edges.push(this->actual_st.back());
        this->actual_st.pop_back();
        this->visited[new_node] = false;

        this->MarkEdges(actual_node, new_node, true);

        b = true;

        for(i = 0; i < this->edges[new_node].size(); ++i){
            if(!this->edges[new_node][i].visited){
                if(this->preorder_visited[new_node] 
                    > this->preorder_visited[this->edges[new_node][i].node_connected]
                   
                    || this->preorder_visited[this->edges[new_node][i].node_connected] 
                    > this->highest[new_node]){

                    b = false;
                    break;
                }
            }
        }
    }

    this->RestoreEdges(backup_edges);
}

void MMRMST::ShowResultInitial(){    
    printf("\nInitial best: ");

    for(std::size_t i = 0; i < this->number_of_scenarios; ++i){
        printf("%ld ", this->best_case[i]);
    }

    printf("\nInitial best weight: %ld\n\n", this->best_weight);    
}

void MMRMST::ShowResultBranch(){
    std::size_t i;

    for(i = 0; i < this->number_of_scenarios; ++i){
        printf("Optimal %zu: %ld\n", i + 1, this->optimal_scenarios[i]);
    }

    printf("\nBest: ");

    for(i = 0; i < this->number_of_scenarios; ++i){
        printf("%ld ", this->best_case[i]);
    }

    printf("\nBest weight: %ld\n\n", this->best_weight);
}
