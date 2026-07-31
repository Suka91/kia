#include <iostream>
#include <vector>

using namespace std;

typedef struct _Edge {
    int from;
    int to;
} Edge;

void insertEdge (vector<vector<int>> &G, vector<Edge> &Edges, int u, int v) {
    int edgeId = Edges.size();

    Edges.push_back({u, v});
    
    G[u].push_back(edgeId);
    G[v].push_back(edgeId);
}

void createGraph (vector<vector<int>> &G, vector<Edge> &Edges) {
    insertEdge(G, Edges, 0, 6);
    insertEdge(G, Edges, 0, 2);
    insertEdge(G, Edges, 0, 4);

    insertEdge(G, Edges, 6, 1);
    insertEdge(G, Edges, 6, 2);
    insertEdge(G, Edges, 6, 5);

    insertEdge(G, Edges, 2, 5);
    insertEdge(G, Edges, 2, 8);

    insertEdge(G, Edges, 4, 8);
    insertEdge(G, Edges, 4, 3);

    insertEdge(G, Edges, 8, 5);

    insertEdge(G, Edges, 5, 1);
    insertEdge(G, Edges, 5, 3);

    insertEdge(G, Edges, 1, 7);
    insertEdge(G, Edges, 1, 9);

    insertEdge(G, Edges, 3, 7);
    insertEdge(G, Edges, 3, 9);

    insertEdge(G, Edges, 7, 9);
}

void DFS (vector<vector<int>> &G, vector<Edge> &Edges, vector<int> &DFNum, vector<int> &NUM, int &Rank, int &DFCount, int v) {
    DFCount++;
    DFNum[v] = DFCount;

    for (int wEdgeId: G[v]) {
        if (Edges[wEdgeId].to == v) continue;

        int w = Edges[wEdgeId].to;
        if (DFNum[w] == 0) {
            DFS(G, Edges, DFNum, NUM, Rank, DFCount, w);
        }
    }

    NUM[v] = Rank;
    Rank--;
}

int main () {
    int n = 10;
    vector<vector<int>> G(n);
    vector<Edge> Edges;
    vector<int> NUM(n, 0);
    vector<int> DFNum(n, 0);
    int DFCount = 0;
    int Rank = n - 1;

    createGraph(G, Edges);

    for (int u = 0; u < n; u++) {
        if (DFNum[u] == 0) {
            DFS(G, Edges, DFNum, NUM, Rank, DFCount, u);
        }
    }

    for (int i = 0; i < n; i++) {
        cout << i << ": " << NUM[i] << endl;
    }

    return 0;
}