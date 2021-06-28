//
//  main.cpp
//  
//
//  Created by Tania Liao on 2021/6/8.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "graph.hpp"

using namespace std;

long long int cycleBreaking_u(Graph&);
long long int cycleBreaking_d(Graph&);
void help_message() {
    cout << "usage: ./cb <input_file> <output_file>" << endl;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        help_message();
        return 0;
    }
    
    //////////// read the input file /////////////
    char type;
    int n, m, from, to, w;
    
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    fin >> type;
    fin >> n;
    fin >> m;
    Graph G(type, n, m);
    for(int i=0; i<n; i++) {
        G.addVertex(i);
    }
    for(int i=0; i<m; i++) {
        fin >> from >> to >> w;
        G.addEdge(from, to, w);
    }
    
    // for testing
    
    cout << "(test: copy)" << endl;
    for(int i=0; i<n; i++) {
        Edge* p = G.adj[i];
        while(p != NULL) {
            if(p->prt) { // if(p->prt)
                cout << p->from << ' ' << p->to << ' ' << p->w << endl;
            }
            p = p->next;
        }
    }
    cout << endl;
    
    //////////// cycle breaking //////////////////
    long long int ttl;
    if(G.type == 'u') {
        ttl = cycleBreaking_u(G);
        // test
        /*
        for(int i=0; i<G.nV; i++) {
            Edge* e = G.adj[i];
            while(e != NULL) {
                //cout << e->from << ' ' << e->to << ' ' << e->w << '\t';
                //cout << e->select << ' ' << e->prt << endl;
                if(e->select && e->prt) {
                    cout << e->from << ' ' << e->to << ' ' << e->w << endl;
                }
                e = e->next;
            }
        }*/
    } else if(G.type == 'd') {
        ttl = cycleBreaking_d(G);
    } else {
        cout << "檔案格式不正確" << endl;
        fin.close();
        fout.close();
        return 0;
    }
    //////////// write the output file ///////////
    fout << ttl << endl;
    for(int i=0; i<G.nV; i++) {
        Edge* e = G.adj[i];
        while(e != NULL) {
            //cout << e->from << ' ' << e->to << ' ' << e->w << '\t';
            //cout << e->select << ' ' << e->prt << endl;
            if(e->select && e->prt) {
                fout << e->from << ' ' << e->to << ' ' << e->w << endl;
            }
            e = e->next;
        }
    }
    fin.close();
    fout.close();
    return 0;
}

long long int cycleBreaking_u(Graph& G) {
    //cout << "cb:" << endl;
    G.V[0].key = 0;
    FibHeap Q;
    //vector<Vertex*> Q_vec(G.nV);
    //cout << "insert" << endl;
    for(int i=0; i<G.nV; i++) {
        Q.insert(&G.V[i]);
    }
    //cout << "-mps" << endl;
    while(Q.Min) {
        Vertex* u = Q.extractMin();
        Edge* e = G.adj[u->name];
        while(e) {
            //cout << e->from << ' ' << e->to << ' ' << e->w << endl;
            Vertex* v = &G.V[e->to];
            //cout << v->name << ' ' << v->key << endl;
            if(Q.ni(v, Q.Min) && -(e->w) < v->key) {
                v->pi = &G.V[e->from];
                Q.decreaseKey(v, -e->w);
            }            //Vertex* i=Q.Min;
            //do {
            //    prtSubtree(i, 0);
            //    i = i->right;
            //} while(i!=Q.Min);
            e = e->next;
        }
    }
    // reset: select=true
    //cout << "reset" << endl;
    long long int T = 0;
    for(int i=0; i<G.nV; i++) {
        Edge* e = G.adj[i];
        while(e != NULL) {
            if(e->prt) {
                e->select = true;
                T += e->w;
            }
            e = e->next;
        }
    }
    // find the unused edges
    //cout << "pick out" << endl;
    for(int i=1; i<G.nV; i++) { // 0 is the starting point with no information
        int j = G.V[i].pi->name;
        Edge* e = G.adj[j];
        while(e != NULL) {
            if(i==e->to && e->prt==true) {
                e->select = false;
                T -= e->w;
                break;
            } else if(i==e->to && e->prt==false) {
                e->pair->select = false;
                T -= e->pair->w;
                break;
            }
            e = e->next;
        }
    }
    return T;
}

long long int cycleBreaking_d(Graph& G) {
    // transform to undigraph G_u
    Graph G_u('u', G.nV, G.nE);
    G.DtoU(G_u);
    
    cout << "(test: DtoU)" << endl;
    for(int i=0; i<G_u.nV; i++) {
        Edge* p = G_u.adj[i];
        while(p != NULL) {
            if(p->prt) { // if(p->prt)
                cout << p->from << ' ' << p->to << ' ' << p->w << endl;
            }
            p = p->next;
        }
    }
    // cycleBreaking_d(G_u) and copy the edges
    
    // find a vertex with no incoming edges and do dfs
    // if the rest of the edges didn't break the dag then add it back
    return 0;
}
