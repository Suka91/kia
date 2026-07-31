#include <iostream>
#include <vector>

using namespace std;

typedef struct _Edge {
    int from;
    int to;
} Edge;

void insertEdge (vector<vector<int>> &G, vector<Edge> &Edges, vector<int> &InDegree, int u, int v) {
    int edgeId = Edges.size();

    Edges.push_back({u, v});
    
    G[u].push_back(edgeId);
    G[v].push_back(edgeId);

    InDegree[v]++;
}

void createGraph (vector<vector<int>> &G, vector<Edge> &Edges, vector<int> &InDegree) {
    insertEdge(G, Edges, InDegree, 0, 6);
    insertEdge(G, Edges, InDegree, 0, 2);
    insertEdge(G, Edges, InDegree, 0, 4);

    insertEdge(G, Edges, InDegree, 6, 1);
    insertEdge(G, Edges, InDegree, 6, 2);
    insertEdge(G, Edges, InDegree, 6, 5);

    insertEdge(G, Edges, InDegree, 2, 5);
    insertEdge(G, Edges, InDegree, 2, 8);

    insertEdge(G, Edges, InDegree, 4, 8);
    insertEdge(G, Edges, InDegree, 4, 3);

    insertEdge(G, Edges, InDegree, 8, 5);

    insertEdge(G, Edges, InDegree, 5, 1);
    insertEdge(G, Edges, InDegree, 5, 3);

    insertEdge(G, Edges, InDegree, 1, 7);
    insertEdge(G, Edges, InDegree, 1, 9);

    insertEdge(G, Edges, InDegree, 3, 7);
    insertEdge(G, Edges, InDegree, 3, 9);

    insertEdge(G, Edges, InDegree, 7, 9);
}

int main () {
    int n = 10;
    vector<vector<int>> G(n);
    vector<Edge> Edges;
    vector<int> InDegree(n, 0);
    vector<int> ScanQ;
    int QSize = 0;
    int k = 0;
    
    createGraph(G, Edges, InDegree);

    for (int v = 0; v < n; v++) {
        if (InDegree[v] == 0) {
            ScanQ.push_back(v);
            QSize++;
        }
    }

    while (k < QSize) {
        int u = ScanQ[k];
        k++;

        for (int vEdgeId: G[u]) {
            if (u == Edges[vEdgeId].to) continue;

            int v = Edges[vEdgeId].to;
            InDegree[v]--;

            if (InDegree[v] == 0) {
                ScanQ.push_back(v);
                QSize++;

                if (QSize == n) {
                    for (int w: ScanQ) {
                        cout << w << " ";
                    }

                    cout << endl;
                    return 0;
                }
            }
        }
    }

    cout << "G contians a directed cycle" << endl;

    return 0;
}