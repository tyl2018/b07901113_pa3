//
//  graph.cpp
//  
//
//  Created by Tania Liao on 2021/6/17.
//

#include "graph.hpp"

// Graph
Graph::Graph(char t, int n, int m) {
    type = t;
    nV = n;
    nE = m;
    V.reserve(n);
    adj.assign(n, NULL);
}

void Graph::addVertex(int x) {
    Vertex v;
    v.name = x;
    v.key = 101;
    v.pi = NULL;
    V.push_back(v);
}

void Graph::addEdge(int i, int j, int w) {
    Edge* e = new Edge();
    e->from = i;
    e->to = j;
    e->w = w;
    e->next = adj[i];
    e->prt = true;
    //e->del = false;
    adj[i] = e;
    //cout << adj[i]->del << endl;
    if(type=='u') {
        Edge* er = new Edge();
        er->from = j;
        er->to = i;
        er->w = w;
        er->next = adj[j];
        er->prt = false;
        //e->select = true;
        adj[j] = er;
        adj[i]->pair = adj[j];
        adj[j]->pair = adj[i];
    }
}

void Graph::DtoU(Graph& G_u) {
    G_u.V = V;
    for(int i=0; i<nV; i++) {
        Edge* e = adj[i];
        while(e!=NULL) {
            if(e->prt) {
                G_u.addEdge(e->from, e->to, e->w);
            }
            e = e->next;
        }
    }
}

FibHeap::FibHeap() {
    size = 0;
    Min = NULL;
    maxDegree = 0;
}

void FibHeap::insert(Vertex* v) {
    //cout << "insert " << v->name << endl;
    v->parent = NULL;
    v->child = NULL;
    v->left = v;
    v->right = v;
    v->degree = 0;
    v->marked = false;
    size++;
    if(Min != NULL) {
        // insert p into root list
        uni(v, Min);
        updateMin(v);
    } else {
        Min = v;
    }
}

Vertex* FibHeap::extractMin() {
    if(Min==NULL) {
        cout << "This heap is empty." << endl;
        return NULL;
    }
    Vertex* i = Min->child;
    if(i != NULL) { // has some children to move
        // cut down all the children
        while(i->parent != NULL) {
            i->parent = NULL;
            i->marked = false;
            //cout << i->name << " false a" << endl;
            i = i->right;
        }
        // Add subtrees in the root list
        uni(Min->left,Min->child);
    }
    // delete Min from the root list
    
    Min->left->right = Min->right;
    Min->right->left = Min->left;
    Vertex* x = Min; // ready to dump
    size--;
    // Set new Min
    updateMin(NULL);
    //consolidate(x->name==71);
    consolidate(false);
    // Delete the min node
    x->left = x;
    x->right = x;
    x->child = NULL;
    return x;
}

void FibHeap::decreaseKey(Vertex* v, int newkey) {
    //cout << "decreaseKey" << endl;
    v->key = newkey;
    if(v->parent == NULL) { // v in the root list. Just update Min
        //cout << "in root list" << endl;
        updateMin(v);
    } else if(v->key < v->parent->key) { // v is not in the root list min tree not violated
        while(true) {
            Vertex* p = v->parent;
            if(p == NULL){break;}
            // cut the link with the parent
            if(v->right==v) {
                p->child = NULL;
            } else {
                p->child = v->right;
            }
            p->degree -= 1;
            v->parent = NULL;
            v->marked = false;
            //cout << v->name << " false b" << endl;
            v->left->right = v->right;
            v->right->left = v->left;
            v->left = v;
            v->right = v;
            // move the subtree to the root list
            uni(v, Min);
            updateMin(v); // update Min
            if(!(p->marked)) {
                p->marked = true;
                //cout << p->name << " true c" << endl;
                break;
            } else {
                v = p;
            }
        }
    }
    
    //prtSubtrees(Min);
}

void FibHeap::uni(Vertex* x, Vertex* y) {
    if(x!=NULL && y!=NULL) {
        x->right->left = y->left;
        y->left->right = x->right;
        x->right = y;
        y->left = x;
    }
    //updateMin(v);
}

bool FibHeap::ni(Vertex* v, Vertex* list) {
    //cout << "ni" << endl;
    if(list==NULL) {
        return false;
    }
    bool b = false;
    Vertex* i = list;
    do {
        if(v==i) {
            return true;
        } else if(v->key >= i->key && i->child != NULL) {
            //prtSubtree(i);
            b = ni(v, i->child);
            if(b) {return true;}
        }
        i = i->right;
    } while(i!=list);
    return false;
}

void FibHeap::updateMin(Vertex* v) {
    //cout << "update Min: ";
    if(size==0) {
        Min = NULL;
        return;
    }
    if(v != NULL) {
        if(v->key < Min->key) {
            Min = v;
        }
    } else {
        Vertex* start = Min->right;
        Min = start;
        Vertex* i = start->right;
        while(i->name != start->name) {
            if(i->key < Min->key){
                Min = i;
            }
            i = i->right;
        }
    }
}

void FibHeap::link(Vertex* y, Vertex* x) {
    // Remove child from root list
    //cout << y->name << " ==> " << x->name << endl;
    //cout << "y->left: " << y->left->name << endl;
    //cout << "y->right: " << y->right->name << endl;
    y->left->right = y->right;
    y->right->left = y->left;
    
    // Make y a child of x
    if(x->child == NULL) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->left = x->child->left;
        y->right = x->child;
        y->left->right = y;
        y->right->left = y;
    }
    //cout << "after link" << endl;
    //cout << "y->left: " << y->left->name << endl;
    //cout << "y->right: " << y->right->name << endl;
    y->parent = x;
    y->marked = false;
    //cout << y->name << " false d" << endl;
    // increase degree
    x->degree += 1;
    if(x->degree > maxDegree) {
        maxDegree = x->degree;
    }
}

void FibHeap::consolidate(bool dbg) {
    
    if(dbg) {
        cout << "consolidate" << endl;
        cout << "before:" << endl;
        prtSubtrees(Min);
    }
     
    if(Min==NULL) {
        return;
    }
    vector<Vertex*> D(maxDegree*2, NULL);
    Vertex* i = Min; // the iterator
    do {
        Vertex* r = i;  // the root of the tree if the tree needs to link
        Vertex* i_next = i->right;
        while(r->degree+1 <= D.size() && D[r->degree] != NULL) { // need to link
            // take out the existing tree in the degree list, and del it from the list
            Vertex* x = D[r->degree];
            D[r->degree] = NULL;
            // link
            cout << r->name << ' ' << x->name << ' ' << Min->name << endl;
            cout << r->key << ' ' << x->key << endl;
            if(r->key < x->key || Min==r) {
                link(x, r);
            } else {
                link(r, x);
                r = x;
            }
        }
        if(r->degree+1 > D.size()) {
            D.resize(r->degree+1, NULL);
        }
        D[r->degree] = r;
        
        i = i_next;
        
        if(dbg) {
            cout << "step" << endl;
            for(int it=0; it<D.size(); it++) {
                if(D.at(it) != NULL)    cout << D.at(it)->name << ' ';
                else    cout << '\\' << ' ';
            }
            cout << endl;
            prtSubtrees(Min);
        }
    } while(i != Min);
    if(dbg) {
        cout << "after:" << endl;
        prtSubtrees(Min);
    }
    
    
}

void prtSubtree(Vertex* v, int depth) {
    cout << v->name << '\t';
    if(v->child != NULL) {
        Vertex* j = v->child;
        do {
            prtSubtree(j, depth+1);
            j = j->right;
        } while(j != v->child);
    }
    cout << "(end)" << endl;
    for(int i=0; i<depth; i++) {
        cout << '\t';
    }
}

void prtSubtrees(Vertex* mini) {
    if(mini==NULL) {
        cout << "empty heap" << endl;
        return;
    }
    Vertex* i = mini;
    do {
        cout << i->degree << ' ';
        prtSubtree(i, 0);
        i = i->right;
    } while(i!=mini);
    char buffer;
    cin >> buffer;
}
