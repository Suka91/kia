#include <iostream>
#include <vector>
#include <climits>

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
    insertEdge(G, Edges, 0, 1);

    insertEdge(G, Edges, 2, 3);
    insertEdge(G, Edges, 3, 4);
    insertEdge(G, Edges, 3, 6);
    insertEdge(G, Edges, 4, 5);
    insertEdge(G, Edges, 6, 7);

    insertEdge(G, Edges, 8, 9);
    insertEdge(G, Edges, 9, 10);

    insertEdge(G, Edges, 8, 0);
    insertEdge(G, Edges, 2, 1);

    insertEdge(G, Edges, 5, 2);
    insertEdge(G, Edges, 7, 2);
    insertEdge(G, Edges, 10, 2);

    insertEdge(G, Edges, 6, 4);
    insertEdge(G, Edges, 7, 5);

    insertEdge(G, Edges, 9, 5);
    insertEdge(G, Edges, 10, 5);
    insertEdge(G, Edges, 10, 8);
}

void DFS (vector<int> &LowPt,
            vector<int> &DFNum,
            vector<int> &Stack,
            vector<bool> &OnStack,
            vector<vector<int>> &G,
            vector<Edge> &Edges,
            int &DFCount, 
            int v, 
            vector<vector<int>> &StrongComponents) {
    DFCount++;
    DFNum[v] = DFCount;
    LowPt[v] = DFCount;
    Stack.push_back(v);
    OnStack[v] = true;

    for (int wEdgeId: G[v]) {
        if (v == Edges[wEdgeId].to) continue;

        int w = Edges[wEdgeId].to;
        
        if (DFNum[w] == 0) {
            DFS(LowPt, DFNum, Stack, OnStack, G, Edges, DFCount, w, StrongComponents);

            if (LowPt[w] < LowPt[v]) {
                LowPt[v] = LowPt[w];
            }
        } else if (DFNum[w] < DFNum[v] && OnStack[w] && LowPt[w] < LowPt[v]) {
            LowPt[v] = LowPt[w];
        }
    }

    if (LowPt[v] == DFNum[v]) {
        vector<int> component;
        
        while (true) {
            int w = Stack.back();
            Stack.pop_back();

            OnStack[w] = false;

            component.push_back(w);

            if (v == w) break;
        }

        StrongComponents.push_back(component);
    }
}

int main () {
    int n = 11;
    vector<vector<int>> G(n);
    vector<Edge> Edges;
    vector<int> DFNum(n, 0);
    vector<bool> OnStack(n, false);
    int DFCount = 0;
    vector<int> Stack;
    vector<int> LowPt(n, INT_MAX);
    vector<vector<int>> StrongComponents;
    
    createGraph(G, Edges);

    for (int u = 0; u < n; u++) {
        if (DFNum[u] == 0) {
            DFS(LowPt, DFNum, Stack, OnStack, G, Edges, DFCount, u, StrongComponents);
        }
    }

    int comp = 0;
    for (vector<int> component: StrongComponents) {
        cout << "[" << comp << "]: ";
        comp++;
        
        for (int u: component) {
            cout << u << " ";
        }

        cout << endl;
    }

    return 0;
}