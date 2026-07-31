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

void insertEdge (vector<vector<int>> &G, vector<Edge> &Edges, int u, int v, int capacity) {
    int edgeId = Edges.size();

    Edges.push_back({u, v, 0, capacity});

    G[u].push_back(edgeId);
    G[v].push_back(edgeId);
}

void createGraph (vector<vector<int>> &G, vector<Edge> &Edges) {
    insertEdge(G, Edges, 0, 1, 8);
    insertEdge(G, Edges, 0, 2, 9);
    insertEdge(G, Edges, 0, 3, 7);

    insertEdge(G, Edges, 1, 4, 2);
    insertEdge(G, Edges, 1, 5, 6);

    insertEdge(G, Edges, 2, 4, 4);
    insertEdge(G, Edges, 2, 5, 6);
    insertEdge(G, Edges, 2, 6, 4);

    insertEdge(G, Edges, 3, 5, 1);
    insertEdge(G, Edges, 3, 6, 5);

    insertEdge(G, Edges, 4, 7, 8);
    insertEdge(G, Edges, 5, 7, 7);
    insertEdge(G, Edges, 6, 7, 9);
}

void AugmentFlow(int t, vector<int> &PrevPt, vector<int> &PrevEdge, vector<int> &ResCap, vector<Edge> &Edges, int &MaxFlow) {
    int v = t;
    int u = PrevPt[t];
    int edgeId = PrevEdge[t];
    int delta = ResCap[t];
    
    while (v != 0) {
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

int main () {
    int n = 8;
    vector<vector<int>> G(n);
    vector<Edge> Edges;
    int MaxFlow = 0;
    int t = 7;

    createGraph(G, Edges);

    while (true) {
        vector<int> ScanQ;
        int QSize = 0;
        vector<bool> onScanQ(n, false);
        vector<int> PrevPt(n, -1);
        vector<int> PrevEdge(n, -1);
        vector<int> ResCap(n, INT_MAX);
        bool flowAugmented = false;
        
        ScanQ.push_back(0);
        QSize++;
        onScanQ[0] = true;

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
                            AugmentFlow(t, PrevPt, PrevEdge, ResCap, Edges, MaxFlow);
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
                            AugmentFlow(t, PrevPt, PrevEdge, ResCap, Edges, MaxFlow);
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

            for (Edge edge: Edges) {
                int from = edge.from;
                int to = edge.to;
                int uvFlow = edge.flow;
                int uvCap = edge.capacity;

                cout << from << " -> " << to << " : " << uvFlow << "/" << uvCap << endl; 
            }
        
            return 0;
        }

    }

    return 0;
}