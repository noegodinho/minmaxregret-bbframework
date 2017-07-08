#ifndef STRUCTS_HPP
#define STRUCTS_HPP


struct scenario_node{
    unsigned node;
    long key;

    bool operator<(const scenario_node &a) const{
        return key > a.key;
    }
};

struct edge{
    unsigned node_connected;
    unsigned weight_index;
    long sum;
    bool visited;

    bool operator<(const edge &a) const{
        return sum < a.sum;
    }
};

struct mst{
    int parent;
    long weight;
    unsigned weight_index;
};

#endif //STRUCTS_HPP
