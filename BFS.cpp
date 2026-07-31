#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void printGraph(vector<vector<int>> &G){
    int n = G.size();
    for (int i = 0; i < n; i++) {
        cout << "[" << i << "]: ";
        for(int v: G[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

void insertEdge(vector<vector<int>> &G, int u, int v){
    G[u].push_back(v);
    G[v].push_back(u);
}

void createGraph(vector<vector<int>> &G){
    insertEdge(G, 0, 1);
    insertEdge(G, 0, 2);
    insertEdge(G, 0, 3);

    insertEdge(G, 1, 4);
    insertEdge(G, 1, 5);

    insertEdge(G, 2, 5);
    insertEdge(G, 2, 6);

    insertEdge(G, 3, 6);
    insertEdge(G, 3, 7);

    insertEdge(G, 4, 8);
    insertEdge(G, 5, 8);
    insertEdge(G, 5, 9);
    insertEdge(G, 6, 9);
    insertEdge(G, 7, 10);

    insertEdge(G, 8, 9);
    insertEdge(G, 9, 10);
}

int main(){
    int n = 12;
    int u = 0;
    int QSize = 0;
    vector<vector<int>> G(n);
    vector<int> dist(n, INT_MAX);
    vector<bool> onScanQ(n, false);
    vector<int> ScanQ(n);

    createGraph(G);

    printGraph(G);

    dist[u] = 0;
    ScanQ[0] = u;
    onScanQ[u] = true;
    QSize = 1;
    int k = 1;
    do {
        int v = ScanQ[k];
        for (int w: G[v]){
            if(!onScanQ[w]) {
                QSize++;
                ScanQ[QSize] = w;
                onScanQ[w] = true;
                dist[w] = dist[v] + 1;
            }
        }
        k = k + 1;
    } while (k < QSize);

    for (int v = 0; v < n; v++) {
        cout << "[" << v << "]: " << dist[v] << endl;
    }

    return 0;
}