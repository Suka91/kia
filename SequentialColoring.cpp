#include <iostream>
#include <vector>

using namespace std;

void insertEdge (vector<vector<int>> &G, int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
}

void createGraph (vector<vector<int>> &G) {
    insertEdge(G, 0, 6);
    insertEdge(G, 0, 2);
    insertEdge(G, 0, 4);

    insertEdge(G, 6, 1);
    insertEdge(G, 6, 2);
    insertEdge(G, 6, 5);

    insertEdge(G, 2, 5);
    insertEdge(G, 2, 8);

    insertEdge(G, 4, 8);
    insertEdge(G, 4, 3);

    insertEdge(G, 8, 5);

    insertEdge(G, 5, 1);
    insertEdge(G, 5, 3);

    insertEdge(G, 1, 7);
    insertEdge(G, 1, 9);

    insertEdge(G, 3, 7);
    insertEdge(G, 3, 9);

    insertEdge(G, 7, 9);
}

void DFS (vector<vector<int>> &G, vector<int> &DFNum, int &DFCount, vector<int> &Sequence, int v) {
    DFCount++;
    DFNum[v] = DFCount;
    Sequence.push_back(v);

    for (int w: G[v]) {
        if (DFNum[w] == 0) {
            DFS(G, DFNum, DFCount, Sequence, w);
        }
    }
}

vector<int> SequencialColoring(vector<vector<int>> &G, vector<int> &Sequence) {
    int n = Sequence.size();
    vector<int> Color(n, -1);

    for (int i = n - 1; i >= 0; i--) {
        int u = Sequence[i];

        vector<bool> colorUsed(n, false);

        for (int v: G[u]) {
            if (Color[v] != -1) {
                colorUsed[Color[v]] = true;
            }
        }

        for (int color = 0; color < n; color++) {
            if (!colorUsed[color]) {
                Color[u] = color;
                break;
            }
        }
        
    }

    return Color;
}

void BFS (vector<vector<int>> &G, vector<int> &Sequence) {
    int n = G.size();
    vector<int> ScanQ;
    vector<bool> OnScanQ(n, false);
    int QSize = 0;

    int u0 = 0;
    ScanQ.push_back(u0);
    QSize++;
    OnScanQ[u0] = true;

    int k = 0;

    while (k < QSize) {
        int u = ScanQ[k];
        k++;

        for (int v: G[u]) {
            if (OnScanQ[v]) continue;

            ScanQ.push_back(v);
            QSize++;
            OnScanQ[v] = true;
        }
    }

    Sequence = ScanQ;
}


int main () {
    int n = 10;
    vector<vector<int>> G(n);
    vector<int> DFNum(n, 0);
    int DFCount = 0;
    vector<int> Sequence;
    vector<int> Color;

    createGraph(G);

    for (int u = 0; u < n; u++) {
        if (DFNum[u] == 0) {
            DFS(G, DFNum, DFCount, Sequence, u);
        }
    }

    Color = SequencialColoring(G, Sequence);

    for (int u = 0; u < n; u++) {
        cout << "[" << u << "]: " << Color[u] << endl;
    }
    
    cout << endl;

    BFS(G, Sequence);
    Color = SequencialColoring(G, Sequence);

    for (int u = 0; u < n; u++) {
        cout << "[" << u << "]: " << Color[u] << endl;
    }

    return 0;
}