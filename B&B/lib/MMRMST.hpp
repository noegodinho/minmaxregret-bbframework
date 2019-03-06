#ifndef MMRMST_HPP
#define MMRMST_HPP

#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <thread>
#include <vector>
#include "structs.hpp"


class MMRMST{
private:
    /* attributes */
    std::vector <struct edge> *edges;
    std::vector <long> *weights;
    static long *optimal_scenarios;
    static long *best_case;
    bool *visited;

    std::vector <std::pair<unsigned, struct edge> > list_of_edges;
    std::vector <std::pair<unsigned, struct edge> > actual_st;
    int *preorder_visited;
    int *highest;

    static long best_weight;
    unsigned number_of_nodes;
    unsigned number_of_scenarios;
    unsigned count;

    static struct timespec start;

    /* methods */
    long Prim(unsigned scenario);
    long Prim2(unsigned scenario);
    void Swap(std::size_t i);
    void InverseSwap(std::size_t index);
    void CalculatePreorder(unsigned w, unsigned actual_node);
    void CalculateWeight(long *actual_best_scenarios,
                         long calculated_weight);
    long CalculateWeight(long *actual_best_scenarios);
    long Bound2();
    void MarkEdges(unsigned actual_node, 
                   unsigned new_node,
                   bool value);
    void Sum(long *actual_best_scenarios);
    void ReplaceBest(long *actual_best_scenarios);
    void RestoreEdges(std::stack <std::pair<unsigned, struct edge> > backup_edges);
    void Branch(unsigned number_of_visited,
                long *actual_best_scenarios);
    void ShowResultInitial();
    void ShowResultBranch();

public:
    MMRMST(std::vector <struct edge> *edges,
           std::vector <long> *weights,
           unsigned number_of_nodes,
           unsigned number_of_scenarios);
    ~MMRMST();

    void Allocate();
    void AllocateMemory();
    void InitializeSum(double p);
    void setTime(struct timespec start);
    void Optimal();
    void InitialSolution(double *w);
    void StartBranch(unsigned node);
    void Delete();
    void WriteSolToFile(char *file);
};


#endif //MMRMST_HPP
