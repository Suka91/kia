#include <iostream>
#include <vector>
#include <climits>

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

void DFSearch(vector<vector<pair<int,int>>> &G, 
                vector<Edge> &Edges, 
                int &DFCount, 
                vector<int> &DFNum, 
                vector<int> &LowPt, 
                vector<int> &Parent, 
                vector<int> &Stack, 
                vector<vector<int>> &Blocks,
                vector<bool> &CutVertex,
                int &Root,
                int &NumRootChild,
                int u) {

    DFCount++;
    DFNum[u] = DFCount;

    for(pair<int, int> e: G[u]) {
        int v = e.first;
        int edgeId = e.second;

        if(DFNum[v] == 0) {
            Parent[v] = u;
            Stack.push_back(edgeId);
            LowPt[v] = DFNum[u];
            
            if(u == Root) {
                NumRootChild++;
            }

            DFSearch(G, Edges, DFCount, DFNum, LowPt, Parent, Stack, Blocks, CutVertex, Root, NumRootChild,  v);

            if(LowPt[v] == DFNum[u]) {
                vector<int> block;
                while(true) {
                    int blockEdgeId = Stack.back();
                    Stack.pop_back();
                    block.push_back(blockEdgeId);
                    if(blockEdgeId == edgeId) {
                        break;
                    }
                }
                Blocks.push_back(block);

                if(Root != u) {
                    CutVertex[u] = true;
                } else if((Root == u) && (NumRootChild >= 2)) {
                    CutVertex[u] = true;
                }
            } else if(LowPt[v] < LowPt[u]) {
                LowPt[u] = LowPt[v];
            }
        } else if(Parent[u] != v) {
            if(DFNum[v] < DFNum[u]) {
                Stack.push_back(edgeId);
                if(DFNum[v] < LowPt[u]) {
                    LowPt[u] = DFNum[v];
                }
            }
        }
    }

}


int main() {
    int n = 9;
    vector<vector<pair<int,int>>> G(n);
    vector<Edge> Edges;
    int DFCount = 0;
    vector<int> DFNum(n, 0);
    vector<int> LowPt(n, INT_MAX);
    vector<int> Parent(n, -1);
    vector<int> Stack;
    vector<vector<int>> Blocks;
    int NumRootChild = 0;
    vector<bool> CutVertex(n, false);

    createGraph(G, Edges);

    int u0 = 0;
    LowPt[u0] = 1;

    DFSearch(G, Edges, DFCount, DFNum, LowPt, Parent, Stack, Blocks, CutVertex, u0, NumRootChild, u0);

    int blockSize = Blocks.size();
    for(int i = 0; i < blockSize; i++) {
        if(Blocks[i].empty()) continue;

        for(int e: Blocks[i]) {
            cout << Edges[e].x << "-" << Edges[e].y << " ";
        }
        cout << endl;
    }
    
    int cvSize = CutVertex.size();
    for(int u = 0; u < cvSize; u++) {
        if(!CutVertex[u]) continue;

        cout << u << " ";
    }
    cout << endl;



    return 0;
}