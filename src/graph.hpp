//
//  graph.hpp
//  
//
//  Created by Tania Liao on 2021/6/17.
//

#ifndef graph_h
#define graph_h

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

typedef struct Vertex {
    int     name;
    long long int     key;  // in undigraph means the weight of the edge from pi
                            // in digraph means the weight of the longest edge going out
    Vertex* pi;
    
    // for directed graphs
    int     topo_order;
    
    // for heap
    Vertex* parent;
    Vertex* child;
    Vertex* left;
    Vertex* right;
    int degree;
    bool marked;
    
} Vertex;

typedef struct Edge {
    int     from;
    int     to;
    int     w;
    Edge*   next;
    Edge*   pair;   // the corresponding edge for undirected edges
    bool    prt;    // whether it's in the directed graph
    bool    select; // be in the output or not
} Edge;

class Graph {
public:
            Graph(char, int, int);
    void    addVertex(int);
    void    addEdge(int, int, int);
    void    DtoU(Graph&);
    vector<Vertex> V;
    vector<Edge*> adj;
    char    type;
    int     nV;
    int     nE;
};

class FibHeap {
public:
            FibHeap();
    void    insert(Vertex*);
    Vertex* extractMin();
    void    decreaseKey(Vertex*, int);
    Vertex* Min;
    void    uni(Vertex*, Vertex*);
    int     size;
    int     maxDegree;
    bool    ni(Vertex*, Vertex*);
protected:
    void    updateMin(Vertex* = NULL);
    void    link(Vertex*, Vertex*);
    void    consolidate();
    void    cut(Vertex*);
};

void prtSubtree(Vertex*, int = 0);
void prtSubtrees(Vertex*);

#endif /* graph_h */
