#include <iostream>
#include <vector>

using namespace std;

typedef struct _Edge {
    bool used;
    int first;
    int second;
} Edge;

void insertEdge(vector<vector<int>> &Graph, vector<Edge> &Edges, int u, int v) {
    int id = Edges.size();
    
    Edges.push_back({false, u, v});
    Graph[u].push_back(id);
    Graph[v].push_back(id);
}

int firstEdge(vector<vector<int>> &Graph, vector<Edge> &Edges, int u) {
    for (int edgeId: Graph[u]) {
        if (!Edges[edgeId].used) {
            return edgeId;
        }
    }
    
    return -1;
}

int otherEndpoint(vector<Edge> &Edges, int edgeId, int x) {
    return Edges[edgeId].first == x ? Edges[edgeId].second : Edges[edgeId].first;
}

void createGraph(vector<vector<int>> &Graph, vector<Edge> &Edges) {
    insertEdge(Graph, Edges, 0, 1);
    insertEdge(Graph, Edges, 0, 5);

    insertEdge(Graph, Edges, 1, 2);
    insertEdge(Graph, Edges, 1, 4);
    insertEdge(Graph, Edges, 1, 5);

    insertEdge(Graph, Edges, 2, 3);
    insertEdge(Graph, Edges, 2, 4);
    insertEdge(Graph, Edges, 2, 5);

    insertEdge(Graph, Edges, 3, 4);

    insertEdge(Graph, Edges, 4, 5);
}

void printEdges(vector<Edge> &Edges) {
    int m = Edges.size();
    for(int i = 0; i < m; i++) {
        cout << "[" << i << "]: " << Edges[i].first << " " << Edges[i].second << endl;
    }
}

void printEulerTour(vector<int> &nextEdge, int startEdge) {
    int m = nextEdge.size();
    int e = startEdge;

    for(int i = 0; i < m; i++) {
        cout << e << " ";
        e = nextEdge[e];
    }
    cout << endl;
}

int main() {
    int n = 6;
    vector<vector<int>> Graph(n);
    vector<Edge> Edges;
    vector<int> ScanQ(n);
    vector<bool> OnScanQ(n, false);
    int QSize = 0;
    vector<int> EulerEdge(n, -1);

    createGraph(Graph, Edges);

    int m = Edges.size();
    vector<int> nextEdge(m, -1);
    vector<int> prevEdge(m, -1);

    int u0 = 0;
    ScanQ[0] = u0;
    OnScanQ[u0] = true;
    QSize++;
    int k = 0;
    while (k < QSize) {
        int u = ScanQ[k];
        int e0;
        while ((e0 = firstEdge(Graph, Edges, u)) != -1) {
            int v = otherEndpoint(Edges, e0, u);
            Edges[e0].used = true;
            int e1 = e0;

            while (u != v) {
                if (!OnScanQ[v]) {
                    ScanQ[QSize++] = v;
                    OnScanQ[v] = true;
                }
                int e2 = firstEdge(Graph, Edges, v);
                nextEdge[e1] = e2;
                prevEdge[e2] = e1;

                if (EulerEdge[v] == -1) {
                    EulerEdge[v] = e2;
                }

                e1 = e2;
                v = otherEndpoint(Edges, e1, v);
                Edges[e1].used = true;
            }

            prevEdge[e0] = e1;
            nextEdge[e1] = e0;

            if (EulerEdge[u] == -1) {
                EulerEdge[u] = e0;
            } else {
                e1 = EulerEdge[u];
                int e2 = prevEdge[e1];
                int e3 = prevEdge[e0];

                nextEdge[e2] = e0;
                prevEdge[e0] = e2;

                nextEdge[e3] = e1;
                prevEdge[e1] = e3;
            }
        }

        k++;
    }

    printEdges(Edges);
    printEulerTour(nextEdge, 0);
}