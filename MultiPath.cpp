#include <iostream>
#include <vector>

using namespace std;

typedef enum _EdgeState {
    AVAILABLE,
    FORCED,
    DELETED
} EdgeState;

typedef enum _ForceResult {
    HAMILTON,
    CONTINUE,
    BACKTRACK
} ForceResult;


typedef struct _Edge {
    int u;
    int v;
    EdgeState state;
} Edge;

void insertEdge(vector<vector<int>> &G, vector<Edge> &Edges, vector<int> &Degree, int u, int v) {
    int edgeId = Edges.size();

    Edges.push_back({u, v, AVAILABLE});
    
    G[u].push_back(edgeId);
    G[v].push_back(edgeId);

    Degree[u]++;
    Degree[v]++;
}

void createGraph(vector<vector<int>> &G, vector<Edge> &Edges, vector<int> &Degree) {
    insertEdge(G, Edges, Degree, 0, 1);
    insertEdge(G, Edges, Degree, 0, 2);
    insertEdge(G, Edges, Degree, 0, 3);

    insertEdge(G, Edges, Degree, 1, 4);
    insertEdge(G, Edges, Degree, 1, 5);

    insertEdge(G, Edges, Degree, 2, 4);
    insertEdge(G, Edges, Degree, 2, 5);
    insertEdge(G, Edges, Degree, 2, 6);

    insertEdge(G, Edges, Degree, 3, 5);
    insertEdge(G, Edges, Degree, 3, 6);

    insertEdge(G, Edges, Degree, 4, 7);
    insertEdge(G, Edges, Degree, 5, 7);
    insertEdge(G, Edges, Degree, 6, 7);
}

int CompRep (vector<int> &Segment, int u) {
    if (Segment[u] < 0) return u;

    int theRep = CompRep(Segment, Segment[u]);
    Segment[u] = theRep;
    return theRep;
}

void MergeSegment (vector<int> &Segment, vector<int> &SegmentSize, int u, int v) {
    int RepU = CompRep(Segment, u);
    int RepV = CompRep(Segment, v);

    int LeftU = -(Segment[RepU] + 1);
    int LeftV = -(Segment[RepV] + 1);

    int newLeft = u == LeftU ? RepU : LeftU;
    int newRep = v == LeftV ? RepV : LeftV;
    int newSize = SegmentSize[RepU] + SegmentSize[RepV];

    Segment[RepU] = newRep;
    Segment[RepV] = newRep;
    Segment[newRep] = -(newLeft + 1);

    SegmentSize[RepU] = 0;
    SegmentSize[RepV] = 0;
    SegmentSize[newRep] = newSize;
}

ForceResult ForcedEdges (vector<Edge> &Edges, 
                            int uvEdgeId, 
                            vector<int> &ForcedDegree, 
                            vector<int> &Degree, 
                            vector<int> &Segment, 
                            vector<int> &SegmentSize, 
                            vector<vector<int>> &G) {
    int n = Segment.size();
    vector<int> ScanQ;
    int QSize = 0;
    int k = 0;

    ScanQ.push_back(uvEdgeId);
    QSize++;

    while (k < QSize) {
        int edgeId = ScanQ[k];
        k++;

        int u = Edges[edgeId].u;
        int v = Edges[edgeId].v;

        if (Edges[edgeId].state == FORCED) continue;

        if (Edges[edgeId].state == DELETED) return BACKTRACK;

        Edges[edgeId].state = FORCED;

        ForcedDegree[u]++;
        ForcedDegree[v]++;

        if (ForcedDegree[u] > 2 || ForcedDegree[v] > 2) return BACKTRACK;
        
        int RepU = CompRep(Segment, u);
        int RepV = CompRep(Segment, v);

        if (RepU == RepV) {
            if (SegmentSize[RepU] == n) {
                return HAMILTON;
            } else {
                return BACKTRACK;
            }
        }

        MergeSegment(Segment, SegmentSize, u, v);

        for(int x: {u,v}) {
            if (ForcedDegree[x] != 2) continue;

            for (int yEdgeId: G[x]) {
                if (Edges[yEdgeId].state != AVAILABLE) continue;

                Edges[yEdgeId].state = DELETED;

                int y = Edges[yEdgeId].u == x ? Edges[yEdgeId].v : Edges[yEdgeId].u;

                Degree[x]--;
                Degree[y]--;

                if (Degree[x] < 2 || Degree[y] < 2) return BACKTRACK;

                if (Degree[y] == 2) {
                    for (int zEdgeId: G[y]) {
                        if (Edges[zEdgeId].state != AVAILABLE) continue;

                        ScanQ.push_back(zEdgeId);
                        QSize++;
                    }
                }
            }
        }
    }

    return CONTINUE;
}

void MultiPath(vector<int> &Segment, 
                vector<Edge> &Edges,
                vector<int> &ForcedDegree, 
                vector<int> &Degree, 
                vector<int> &SegmentSize, 
                vector<vector<int>> &G,
                bool &isHamilton) {

    int u = CompRep(Segment, 0);

    for (int uwEdgeId: G[u]) {
        if (Edges[uwEdgeId].state != AVAILABLE) continue;

        vector<Edge> savedEdges = Edges;
        vector<int> savedSegment = Segment;
        vector<int> savedSegmentSize = SegmentSize;
        vector<int> savedDegree = Degree;
        vector<int> savedForcedDegree = ForcedDegree;

        ForceResult result = ForcedEdges(Edges, uwEdgeId, ForcedDegree, Degree, Segment, SegmentSize, G);

        cout << result << endl;

        if (result == HAMILTON) {
            isHamilton = true;
            return;
        }

        if (result == CONTINUE) {
            MultiPath(Segment, Edges, ForcedDegree, Degree, SegmentSize, G, isHamilton);

            if (isHamilton) return;
        }
        
        Edges = savedEdges;
        Segment = savedSegment;
        SegmentSize = savedSegmentSize;
        Degree = savedDegree;
        ForcedDegree = savedForcedDegree;
    }
}

int main () {
    int n = 8;
    vector<vector<int>> G(n);
    vector<Edge> Edges;
    vector<int> Segment(n);
    vector<int> SegmentSize(n, 1);
    vector<int> Degree(n, 0);
    vector<int> ForcedDegree(n, 0);
    bool isHamilton = false;

    createGraph(G, Edges, Degree);

    for (int u = 0; u < n; u++) {
        Segment[u] = -(u + 1);
    }

    MultiPath(Segment, Edges, ForcedDegree, Degree, SegmentSize, G, isHamilton);
    
    cout << (isHamilton ? "Hamilton" : "Not Hamilton") << endl;

    return 0;
}