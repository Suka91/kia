#include <iostream>
#include <vector>
#include <climits>

using namespace std;

typedef struct _Edge {
    int from;
    int to;
    int flow;
    int capacity;
} Edge;

typedef struct _UndirectedEdge {
    int u;
    int v;
} UndirectedEdge;

void insertEdge (vector<vector<int>> &G, vector<UndirectedEdge> &Edges, int u, int v) {
    int edgeId = Edges.size();

    Edges.push_back({u, v});

    G[u].push_back(edgeId);
    G[v].push_back(edgeId);
}

void createGraph (vector<vector<int>> &G, vector<UndirectedEdge> &Edges) {
    insertEdge(G, Edges, 0, 1);
    insertEdge(G, Edges, 0, 2);
    insertEdge(G, Edges, 0, 3);

    insertEdge(G, Edges, 1, 4);
    insertEdge(G, Edges, 1, 5);

    insertEdge(G, Edges, 2, 4);
    insertEdge(G, Edges, 2, 5);
    insertEdge(G, Edges, 2, 6);

    insertEdge(G, Edges, 3, 5);
    insertEdge(G, Edges, 3, 6);

    insertEdge(G, Edges, 4, 7);
    insertEdge(G, Edges, 5, 7);
    insertEdge(G, Edges, 6, 7);
}

void insertDirectedEdge (vector<vector<int>> &G, vector<Edge> &Edges, int u, int v, int capacity) {
    int edgeId = Edges.size();

    Edges.push_back({u, v, 0, capacity});

    G[u].push_back(edgeId);
    G[v].push_back(edgeId);
}

void createDirectedGraph (vector<vector<int>> &G, vector<UndirectedEdge> &UndirectedEdges, vector<vector<int>> &N, vector<Edge> &Edges) {
    int n = G.size();

    for (int u = 0; u < n; u++) {
        insertDirectedEdge(N, Edges, u, u + n, 1);
    }

    for (UndirectedEdge unEdge: UndirectedEdges) {
        int u = unEdge.u;
        int v = unEdge.v;

        insertDirectedEdge(N, Edges, u + n, v, 1);
        insertDirectedEdge(N, Edges, v + n, u, 1);
    }
}

void AugmentFlow(int s, int t, vector<int> &PrevPt, vector<int> &PrevEdge, vector<int> &ResCap, vector<Edge> &Edges, int &MaxFlow) {
    int v = t;
    int u = PrevPt[t];
    int edgeId = PrevEdge[t];
    int delta = ResCap[t];
    
    while (v != s) {
        int from = Edges[edgeId].from;

        if (u == from) {
            Edges[edgeId].flow += delta;
        } else {
            Edges[edgeId].flow -= delta;
        }

        v = u;
        u = PrevPt[v];
        edgeId = PrevEdge[v];
    }

    MaxFlow += delta;
}

int MaxFlow(vector<vector<int>> &G, vector<Edge> &Edges, int s, int t) {
    int MaxFlow = 0;
    int n = G.size();

    while (true) {
        vector<int> ScanQ;
        int QSize = 0;
        vector<bool> onScanQ(n, false);
        vector<int> PrevPt(n, -1);
        vector<int> PrevEdge(n, -1);
        vector<int> ResCap(n, INT_MAX);
        bool flowAugmented = false;
        
        ScanQ.push_back(s);
        QSize++;
        onScanQ[s] = true;

        int k = 0; 
        while (k < QSize) {
            int u = ScanQ[k];
            
            for (int edgeId: G[u]) {
                int from = Edges[edgeId].from;
                int to = Edges[edgeId].to;
                int uvFlow = Edges[edgeId].flow;
                int uvCap = Edges[edgeId].capacity;
                int v = u == to ? from : to;

                if (onScanQ[v]) continue;

                if(u == from) {
                    if(uvCap > uvFlow) {
                        ScanQ.push_back(v);
                        QSize++;
                        onScanQ[v] = true;
                        PrevPt[v] = u;
                        PrevEdge[v] = edgeId;

                        if (ResCap[u] < uvCap - uvFlow) {
                            ResCap[v] = ResCap[u];
                        } else {
                            ResCap[v] = uvCap - uvFlow;
                        }

                        if (v == t) {
                            AugmentFlow(s, t, PrevPt, PrevEdge, ResCap, Edges, MaxFlow);
                            flowAugmented = true;
                            break;
                        }
                    }
                } else {
                    if (uvFlow > 0) {
                        ScanQ.push_back(v);
                        QSize++;
                        onScanQ[v] = true;
                        PrevPt[v] = u;
                        PrevEdge[v] = edgeId;

                        if (ResCap[u] < uvFlow) {
                            ResCap[v] = ResCap[u];
                        } else {
                            ResCap[v] = uvFlow;
                        }

                        if (v == t) {
                            AugmentFlow(s, t, PrevPt, PrevEdge, ResCap, Edges, MaxFlow);
                            flowAugmented = true;
                            break;
                        }
                    }
                }
            }
        
            if (flowAugmented) {
                break;
            }
        
            k++;
        }

        if (k == QSize) {
            cout << "MaxFlow: " << MaxFlow << endl;

            // for (Edge edge: Edges) {
            //     int from = edge.from;
            //     int to = edge.to;
            //     int uvFlow = edge.flow;
            //     int uvCap = edge.capacity;

            //     cout << from << " -> " << to << " : " << uvFlow << "/" << uvCap << endl; 
            // }
        
            return MaxFlow;
        }

    }

    return MaxFlow;
}

bool isAdj(vector<vector<int>> &G, vector<UndirectedEdge> &UnEdges, int s, int t) {
    for (int unEdgeId: G[s]) {
        if (UnEdges[unEdgeId].u == t || UnEdges[unEdgeId].v == t) {
            return true;
        }
    }

    return false;
}

int main () {
    int n = 8;
    vector<vector<int>> G(n);
    vector<UndirectedEdge> UnEdges;

    createGraph(G, UnEdges);

    vector<vector<int>> N(2*n);
    vector<Edge> Edges;

    createDirectedGraph(G, UnEdges, N, Edges);

    int k = n - 1;
 
    for (int s = 0; s < n - 1; s++) {
        for (int t = s + 1; t < n; t++) {
            
            if (isAdj(G, UnEdges, s, t)) continue;

            vector<Edge> tempEdges = Edges;

            int maxFlow = MaxFlow(N, tempEdges, s + n, t);

            k = maxFlow < k ? maxFlow : k;
        }
    }

    cout << "Connectivity: " << k << endl;

    return 0;
}