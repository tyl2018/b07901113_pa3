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
    /*
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
    */
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
    while(Q.Min != NULL) {
        //cout << "loop" << endl;
        Vertex* u = Q.extractMin();
        //cout << u->name << endl;
        Edge* e = G.adj[u->name];
        while(e) {
            //cout << e->from << ' ' << e->to << ' ' << e->w << endl;
            Vertex* v = &G.V[e->to];
            //cout << v->name << ' ' << v->key << endl;
            if(Q.ni(v, Q.Min) && -(e->w) < v->key) {
                //cout << "update" << endl;
                v->pi = &G.V[e->from];
                Q.decreaseKey(v, -e->w);
            }
            //prtSubtrees(Q.Min);
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
    Graph G_r('u', G.nV, G.nE); // undigraph transformed from G
    G.DtoU(G_r);
    
    // Graph G_r('d', G.nV, G.nE); // reduced digraph by cycleBreaking_u
    
    // do cb on G_r and copy the answer
    long long int T=0;
    T = cycleBreaking_u(G_r);
    G_r.type = 'd'; // change G_r into the reduced directed graph
    for(int i=0; i<G_r.nV; i++) {
        Edge* e = G_r.adj[i];
        if(e==NULL) {continue;}
        if(!e->prt) { // delete e
            G_r.adj[i] = e->next;
            e = e->next;
        }
        if(e==NULL) {continue;}
        while(e->next != NULL) {
            Edge* f = e->next;
            if(!f->prt) { // delete f
                e->next = f->next;
            } else {
                e = e->next;
            }
        }
    }
    //G.adj = move(G_r.adj); // C++11 only
    G.adj = G_r.adj;
    
    // prepare for topological sort
    vector<int> ref(G.nV, 0); // The number of incoming edges
    for(int i=0; i<G.nV; i++) {
        Edge* e = G.adj[i];
        G.V[i].key = 0;
        while(e != NULL) {
            cout << e->from << ' ' << e->to << endl;
            if(!e->select) { // only the remaining edges counts
                ref[e->to] += 1;
                G.V[e->from].key -= e->w;
            }
            e = e->next;
        }
    }
    for(int i=0; i<G.nV; i++) {
        cout << i << ' ' <<  ref[i] << ' ' << G.V[i].key << endl;
    }
    /*
    // find a vertex with no incoming edges and do topological sort
    //cout << "topo order" << endl;
    FibHeap Q;
    cout << "init" << endl;
    for(int i=0; i<G.nV; i++) {
        if(ref[i]==0) {
            Q.insert(&G.V[i]);
            cout << "insert " << i << endl;
        }
    }
    cout << "for" << endl;
    for(int i=0; i<G.nV; i++) {
        cout << "popMin" << endl;
        Vertex* s = Q.extractMin();
        if(s==NULL) {
            cout << "error" << endl;
            return 0;
        }
        cout << "set order" << endl;
        ref[s->name] = -1; // mark as sorted
        s->topo_order = i;
        cout << i << ' ' << s->name << endl;
        
        cout << "update" << endl;
        Edge* e = G.adj[s->name];
        while(e != NULL) {
            if(!e->select) {
                ref[e->to] -= 1;
            }
            if(ref[e->to]==0) {
                Q.insert(&G.V[e->to]);
            }
            e = e->next;
        }
    }
    for(int i=0; i<G.nV; i++) {
        cout << G.V[i].topo_order << endl;
    }
    // if the rest of the edges didn't break the sort then add it back
     */
    return T;
}
