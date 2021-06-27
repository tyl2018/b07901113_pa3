//
//  graph_test.cpp
//  
//
//  Created by Tania Liao on 2021/6/21.
//

#include <stdio.h>
#include "graph.hpp"

int main() {
    Graph G = Graph('u', 16, 0);
    int keys[16] = {4, 9, 0, 1, 9, 2, 1, 6, 1, 9, 7, 5, 2, 8, 3, 8};
    for(int i=0; i<16; i++) {
        G.addVertex(i);
        G.V[i].key = keys[i];
    }
    /*
    for(int i=0; i<8; i++) {
        cout << G.V[i].name << ' '<< G.V[i].key << endl;
    }*/
    /*
    //G.addEdge(0, 1, 31);
    //G.addEdge(1, 2, 41);
    for(int i=0; i<9; i++) {
        cout << i << endl;
        Edge* p = G.adj[i];
        while(p != NULL) {
            cout << p->w << " ";
            p = p->next;
        }
        cout << endl;
    }
    */
    FibHeap Q = FibHeap();
    cout << "Insert" << endl;
    for(int i=0; i<16; i++) {
        Q.insert(&G.V[i]);
    }
    Vertex* p = Q.Min;
    do {
        cout << p->name << endl;
        p = p->right;
    } while(p != Q.Min);
    cout << endl;
    /*
    cout << "link 1 to 2" << endl;
    Q.link(&G.V[1], &G.V[2]);
    p = Q.Min;
    for(int i=0; i<9; i++) {
        prtSubtree(p, 0);
        p = p->right;
    } */
    
    cout << endl;
    vector<Vertex*> x;
    /*
    x.push_back(Q.extractMin());
    x.push_back(Q.extractMin());
    x.push_back(Q.extractMin());
    p = Q.Min;
    for(int i=0; i<3; i++) {
        cout << p->name << ':' << endl;
        if(p->child != NULL) {
            p = p->child;
            for(int i=0; i<3; i++) {
                cout << p->name << ' ';
                p = p->right;
            }
            p = p->parent;
        }
        cout << endl;
        p = p->right;
    } */
    x.push_back(Q.extractMin());
    Vertex* v = Q.Min;
    do {
        prtSubtree(v, 0);
        v = v->right;
    } while(v != Q.Min);
    cout << endl;
    
    int a[3] = {12, 7, 10};
    int b[3] = {-1, 0, -2};
    for(int i=0; i<3; i++) {
        Q.decreaseKey(&G.V[a[i]], b[i]);
        v = Q.Min;
        do {
            prtSubtree(v, 0);
            v = v->right;
        } while(v != Q.Min);
        char buffer;
        cin >> buffer;
    }
    v = Q.Min;
    /*
    for(int i=0; i<8; i++) {
        x.push_back(Q.extractMin());
    }
    for(int i=0; i<8; i++) {
        cout << x[i]->name << ' ' << x[i]->key << endl;
    }
    cout << endl;
    cout << (Q.Min==NULL) << endl;
    */
    return 0;
}
