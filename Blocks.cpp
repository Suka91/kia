#include <iostream>
#include <vector>

using namespace std;

typedef struct _Edge {
    int x;
    int y;
} Edge;

void insertEdge(vector<vector<pair<int,int>>> &G, vector<Edge> &Edges, int u, int v) {
    int id = Edges.size();

    Edges.push_back({u, v});
    G[u].push_back({v, id});
    G[v].push_back({u, id});
}

void createGraph(vector<vector<pair<int,int>>> &G, vector<Edge> &Edges) {
    insertEdge(G, Edges, 0, 1);
    insertEdge(G, Edges, 1, 2);
    insertEdge(G, Edges, 2, 0);

    insertEdge(G, Edges, 2, 3);
    insertEdge(G, Edges, 3, 4);
    insertEdge(G, Edges, 4, 2);

    insertEdge(G, Edges, 4, 5);

    insertEdge(G, Edges, 5, 6);
    insertEdge(G, Edges, 6, 7);
    insertEdge(G, Edges, 7, 8);
    insertEdge(G, Edges, 8, 5);

    insertEdge(G, Edges, 6, 8);
}

int CompRep(vector<int> &CompPtr, int u) {
    if(CompPtr[u] < 0) return u;

    int uRep = CompRep(CompPtr, CompPtr[u]);
    CompPtr[u] = uRep;
    return uRep;
}

int MergeComp(vector<int> &CompPtr, int uRep, int vRep) {
    int uSize = -CompPtr[uRep];
    int vSize = -CompPtr[vRep];

    if(uSize > vSize) {
        CompPtr[uRep] = -(uSize + vSize);
        CompPtr[vRep] = uRep;
        return uRep;
    } else {
        CompPtr[vRep] = -(uSize + vSize);
        CompPtr[uRep] = vRep;
        return vRep;
    }
}

int main() {
    int n = 9;
    vector<vector<pair<int,int>>> G(n);
    vector<Edge> Edges;
    int QSize = 0;
    vector<int> ScanQ;
    vector<bool> onScanQ(n, false);
    
    createGraph(G, Edges);
    
    int m = Edges.size();
    vector<int> CompPtr(m, -1);

    vector<int> parentV(n, -1);
    vector<int> parentE(n, -1);
    vector<int> dist(n, -1);

    int u0 = 0;
    ScanQ.push_back(u0);
    onScanQ[u0] = true;
    QSize++;
    dist[u0] = 0;

    int k = 0;
    
    while(k < QSize) {
        int u = ScanQ[k];
        k++;

        for (pair<int, int> pv: G[u]) {
            int v = pv.first;
            int uvEdgeId = pv.second;

            if(v < u) continue;

            if(!onScanQ[v]) {
                ScanQ.push_back(v);
                onScanQ[v] = true;
                QSize++;

                parentV[v] = u;
                parentE[v] = uvEdgeId;
                dist[v] = dist[u] + 1;
            } else {
                int uvRep = CompRep(CompPtr, uvEdgeId);
                int a = u;
                int b = v;
                vector<int> C;
                
                C.push_back(uvEdgeId);
                while(dist[a] != dist[b]) {
                    if(dist[a] > dist[b]) {
                        C.push_back(parentE[a]);
                        a = parentV[a];
                    } else if (dist[b] > dist[a]) {
                        C.push_back(parentE[b]);
                        b = parentV[b];
                    }
                }

                while(a != b) {
                    C.push_back(parentE[a]);
                    a = parentV[a];

                    C.push_back(parentE[b]);
                    b = parentV[b];
                }

                for(int edgeId: C) {
                    int eRep = CompRep(CompPtr, edgeId);

                    if(uvRep != eRep) {
                        uvRep = MergeComp(CompPtr, uvRep, eRep);
                    }
                }
            }
        }
    }

    vector<vector<int>> Blocks(m);
    for (int e = 0; e < m; e++) {
        int eRep = CompRep(CompPtr, e);
        Blocks[eRep].push_back(e);
    }
    int blockSize = Blocks.size();
    for(int i = 0; i < blockSize; i++) {
        if(Blocks[i].empty()) continue;

        for(int e: Blocks[i]) {
            cout << Edges[e].x << "-" << Edges[e].y << " ";
        }
        cout << endl;
    }

    return 0;
}