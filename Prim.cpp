#include <iostream>
#include <vector>
#include <climits>

using namespace std;

typedef struct _Edge {
    int x;
    int y;
    int weight;
} Edge;

void insertEdge(vector<vector<pair<int,int>>> &G, int u, int v, int weight) {
    G[u].push_back({v, weight});
    G[v].push_back({u, weight});
}

void createGraph(vector<vector<pair<int,int>>> &G) {
    insertEdge(G, 0, 1, 1);
    insertEdge(G, 0, 2, 1);
    insertEdge(G, 0, 4, 100);

    insertEdge(G, 1, 2, 2);

    insertEdge(G, 2, 3, 2);
    insertEdge(G, 2, 4, 3);

    insertEdge(G, 3, 4, 1);
}

void floatUp(vector<int> &H, vector<int> &MinWt, vector<int> &pos, int k) {
    int temp = H[k];
    int j = k / 2;

    while (j > 0 && MinWt[temp] < MinWt[H[j]]) {
        H[k] = H[j];
        pos[H[k]] = k;
        k = j;
        j = k / 2;
    }

    H[k] = temp;
    pos[H[k]] = k;
}

void floatDown(vector<int> &H, int heapSize, vector<int> &MinWt, vector<int> &pos, int k) {
    int temp = H[k];
    int n = heapSize;
    
    while (2*k <= n) {
        int i = 2*k;
        int j = i + 1;
        
        if (j > n) j = 0;

        if (MinWt[H[j]] < MinWt[H[i]]) i = j;

        if (MinWt[temp] <= MinWt[H[i]]) break;

        H[k] = H[i];
        pos[H[i]] = k;
        k = i;
    }

    H[k] = temp;
    pos[temp] = k;
}

int main() {
    int n = 5;
    vector<vector<pair<int,int>>> G(n); 
    vector<int> VT(n, false);
    vector<Edge> Tree;
    int t = 0;
    vector<int> MinWt(n + 1, INT_MAX);
    vector<int> MinPt(n, -1);
    vector<int> H(n + 1);
    vector<int> pos(n);
    int heapSize = n;

    createGraph(G);
    
    for (int u = 0; u < n; u++) {
        H[u + 1] = u;
        pos[u] = u + 1;
    }
    H[0] = n;
    
    int u0 = 0;
    MinWt[u0] = 0;
    floatUp(H, MinWt, pos, pos[u0]);

    while (t < n - 1) {
        int y = H[1];
        H[1] = H[heapSize];
        pos[H[1]] = 1;
        heapSize--;
        floatDown(H, heapSize, MinWt, pos, 1);

        if (MinPt[y] != -1) {
            Tree.push_back({MinPt[y], y, MinWt[y]});
            t++;
        }

        VT[y] = true;
        
        for (pair<int, int> edge: G[y]) {
            int w = edge.first;
            int weight = edge.second;
            if(!VT[w] && weight < MinWt[w]) {
                MinWt[w] = weight;
                MinPt[w] = y;
                floatUp(H, MinWt, pos, pos[w]);
            }
        }
    }

    for (Edge edge: Tree) {
        int x = edge.x;
        int y = edge.y;
        int w = edge.weight;
        cout << x << " <--" << w << "--> " << y << endl;  
    }

    return 0;
}