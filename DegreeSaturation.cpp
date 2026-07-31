#include <iostream>
#include <vector>

using namespace std;

void insertEdge (vector<vector<int>> &G, vector<int> &Degree, int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);

    Degree[u]++;
    Degree[v]++;
}

void createGraph (vector<vector<int>> &G, vector<int> &Degree) {
    insertEdge(G, Degree, 0, 6);
    insertEdge(G, Degree, 0, 2);
    insertEdge(G, Degree, 0, 4);

    insertEdge(G, Degree, 6, 1);
    insertEdge(G, Degree, 6, 2);
    insertEdge(G, Degree, 6, 5);

    insertEdge(G, Degree, 2, 5);
    insertEdge(G, Degree, 2, 8);

    insertEdge(G, Degree, 4, 8);
    insertEdge(G, Degree, 4, 3);

    insertEdge(G, Degree, 8, 5);

    insertEdge(G, Degree, 5, 1);
    insertEdge(G, Degree, 5, 3);

    insertEdge(G, Degree, 1, 7);
    insertEdge(G, Degree, 1, 9);

    insertEdge(G, Degree, 3, 7);
    insertEdge(G, Degree, 3, 9);

    insertEdge(G, Degree, 7, 9);
}

int SelectLargerVertex(vector<int> &c, vector<int> &Degree, int u, int v) {
    int n = Degree.size();
    if (n == u) return v;
    if (n == v) return u;
    return c[u] > c[v] ? u : 
                        (c[v] > c[u] ? v : 
                                        (Degree[u] > Degree[v] ? u : v));
}

void floatDown(vector<int> &H, int heapSize, vector<int> &pos, vector<int> &c, vector<int> &Degree, int k) {
    int temp_Hk = H[k];

    while((k > 0) && (2*k <= heapSize)) {
        int i = 2*k;
        int j = i + 1;

        if(j > heapSize) {
            j = 0;
        }

        if(H[j] == SelectLargerVertex(c, Degree, H[i], H[j])) {
            i = j;
        }

        if(temp_Hk == SelectLargerVertex(c, Degree, temp_Hk, H[i])) {
            break;
        }

        H[k] = H[i];
        pos[H[i]] = k;
        k = i;
    }
    
    H[k] = temp_Hk;
    pos[temp_Hk] = k;
}

void floatUp(vector<int> &H, vector<int> &pos, vector<int> &c, vector<int> &Degree, int k) {
    int temp_Hk = H[k];
    int j = k/2;

    while (j > 0 && (temp_Hk == SelectLargerVertex(c, Degree, temp_Hk, H[j]))) {
        H[k] = H[j];
        pos[H[j]] = k;
        k = j;
        j = k/2;
    }

    H[k] = temp_Hk;
    pos[temp_Hk] = k;
}

int main () {
    int n = 10;
    vector<vector<int>> G(n);
    vector<int> Degree(n);
    vector<int> c(n, 0);
    vector<int> H(n + 1);
    int heapSize = n;
    vector<int> pos(n);
    vector<int> Color(n, -1);
    vector<vector<bool>> AdjColor(n, vector<bool> (n, false));

    for (int u = 0; u < n; u++) {
        H[u + 1] = u;
        pos[u] = u + 1;
    }

    H[0] = n;

    createGraph(G, Degree);

    for (int u = 0; u < n; u++) {
        floatUp(H, pos, c, Degree, pos[u]);
    }

    for (int i = 0; i < n; i++) {
        int u = H[1];
        H[1] = H[heapSize];
        pos[H[1]] = 1;

        floatDown(H, heapSize, pos, c, Degree, pos[H[1]]);
        
        heapSize--;


        vector<bool> colorUsed(n, false);
        for (int v: G[u]) {
            if (Color[v] != -1) colorUsed[Color[v]] = true;
        }

        for (int color = 0; color < n; color++) {
            if (!colorUsed[color]) {
                Color[u] = color;
                break;
            }
        }

        for (int v: G[u]) {
            if (Color[v] == -1 && !AdjColor[v][Color[u]]) {
                c[v]++;
                AdjColor[v][Color[u]] = true;
                floatUp(H, pos, c, Degree, pos[v]);
            }
        }
    }

    for (int u = 0; u < n; u++) {
        cout << "[" << u << "]: " << Color[u] << endl;
    }

    return 0;
}