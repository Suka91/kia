#include <iostream>
#include <vector>

using namespace std;

typedef struct _Edge {
    int x;
    int y;
    int weight;
} Edge;

void insertEdge(vector<vector<int>> &G, vector<Edge> &Edges, int u, int v, int weight) {
    int id = Edges.size();

    Edges.push_back({u, v, weight});
    G[u].push_back(id);
    G[v].push_back(id);
}

void createGraph(vector<vector<int>> &G, vector<Edge> &Edges) {
    insertEdge(G, Edges, 0, 1, 1);
    insertEdge(G, Edges, 0, 2, 1);
    insertEdge(G, Edges, 0, 4, 100);

    insertEdge(G, Edges, 1, 2, 2);

    insertEdge(G, Edges, 2, 3, 2);
    insertEdge(G, Edges, 2, 4, 3);

    insertEdge(G, Edges, 3, 4, 1);

    insertEdge(G, Edges, 5, 6, 10);
}

void floatDown(vector<int> &H, vector<Edge> &Edges, int n, int k) {
    int temp = H[k];

    while (2 * k + 1 < n) {
        int i = 2 * k + 1;
        int j = i + 1;

        if (j < n && Edges[H[j]].weight < Edges[H[i]].weight) i = j;

        if (Edges[temp].weight <= Edges[H[i]].weight) break;

        H[k] = H[i];
        k = i;
    }

    H[k] = temp;
}

void buildHeapBottomUp(vector<int> &H, vector<Edge> &Edges) {
    int n = H.size();
    int k = n / 2 - 1;
    
    while (k >= 0) {
        floatDown(H, Edges, n, k);
        k--;
    }
}

int CompRep(vector<int> &CompPtr, int u) {
    if (CompPtr[u] < 0) return u;

    int theRep = CompRep(CompPtr, CompPtr[u]);
    CompPtr[u] = theRep;
    return theRep;
}

void Merge(vector<int> &CompPtr, int uRep, int vRep) {
    int uSize = -CompPtr[uRep];
    int vSize = -CompPtr[vRep];

    if (uSize > vSize) {
        CompPtr[uRep] = -(uSize + vSize);
        CompPtr[vRep] = uRep;
    } else {
        CompPtr[vRep] = -(uSize + vSize);
        CompPtr[uRep] = vRep;
    }
}

int main() {
    int n = 7;
    vector<vector<int>> G(n);
    vector<Edge> Edges;
    vector<int> CompPtr(n, -1);
    int t = 0;
    vector<Edge> Tree;

    createGraph(G, Edges);
    
    int m = Edges.size();
    vector<int> H(m);

    for (int e = 0; e < m; e++) {
        H[e] = e;
    }

    buildHeapBottomUp(H, Edges);

    while (t < n - 1 && m > 0) {
        int e = H[0];
        H[0] = H[m - 1];
        m--;
        floatDown(H, Edges, m, 0);

        int uRep = CompRep(CompPtr, Edges[e].x);
        int vRep = CompRep(CompPtr, Edges[e].y);
        if(uRep != vRep) {
            Merge(CompPtr, uRep, vRep);
            Tree.push_back(Edges[e]);
            t++;
        }
    }

    for (Edge edge: Tree) {
        cout << edge.x << " <--" << edge.weight << "--> " << edge.y << endl;  
    }

    return 0;
}