#include <iostream>
#include <vector>

using namespace std;

void insertEdge (vector<vector<int>> &G, vector<vector<bool>> &Adj, int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);

    Adj[u][v] = true;
    Adj[v][u] = true;
}

void createGraph (vector<vector<int>> &G, vector<vector<bool>> &Adj) {
    insertEdge(G, Adj, 0, 6);
    insertEdge(G, Adj, 0, 2);
    insertEdge(G, Adj, 0, 4);

    insertEdge(G, Adj, 6, 1);
    insertEdge(G, Adj, 6, 2);
    insertEdge(G, Adj, 6, 5);

    insertEdge(G, Adj, 2, 5);
    insertEdge(G, Adj, 2, 8);

    insertEdge(G, Adj, 4, 8);
    insertEdge(G, Adj, 4, 3);

    insertEdge(G, Adj, 8, 5);

    insertEdge(G, Adj, 5, 1);
    insertEdge(G, Adj, 5, 3);

    insertEdge(G, Adj, 1, 7);
    insertEdge(G, Adj, 1, 9);

    insertEdge(G, Adj, 3, 7);
    insertEdge(G, Adj, 3, 9);

    insertEdge(G, Adj, 7, 9);
}

void ExtendClique(vector<int> &C, vector<int> &CPrime, vector<vector<bool>> &Adj, vector<int> &S, int v) {
    vector<int> SPrime;

    for (int w: S) {
        if (Adj[v][w]) {
            SPrime.push_back(w);
        }
    }

    if (SPrime.size() == 0) {
        if (CPrime.size() > C.size()) {
            C = CPrime;
        }

        return;
    }

    while (SPrime.size() > 0 && (CPrime.size() + SPrime.size() > C.size())) {
        int u = SPrime.back();
        SPrime.pop_back();


        CPrime.push_back(u);

        ExtendClique(C, CPrime, Adj, SPrime, u);

        CPrime.pop_back();
    }
}

int main () {
    int n = 10;
    vector<vector<int>> G(n);
    vector<vector<bool>> Adj(n, vector<bool> (n, false));
    vector<int> C;
    vector<int> S;

    createGraph(G, Adj);

    for (int i = n - 1; i >= 0; i--) {
        S.push_back(i);
    }

    while (C.size() < S.size()) {
        int v = S.back();
        S.pop_back();
        
        vector<int> CPrime;
        CPrime.push_back(v);

        ExtendClique(C, CPrime, Adj, S, v);
    }

    for (int v: C) {
        cout << v << endl;
    }

    return 0;
}