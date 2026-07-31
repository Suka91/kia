#include <iostream>
#include <vector>

using namespace std;

typedef struct _Edge {
    int u;
    int v;
    int color;
} Edge;

void insertEdge (vector<vector<int>> &G, vector<Edge> &E, int u, int v) {
    int edgeId = E.size();

    E.push_back({u, v, -1});

    G[u].push_back(edgeId);
    G[v].push_back(edgeId);
}

void createGraph (vector<vector<int>> &G, vector<Edge> &E) {
    insertEdge(G, E, 0, 5);
    insertEdge(G, E, 0, 6);

    insertEdge(G, E, 1, 5);
    insertEdge(G, E, 1, 6);

    insertEdge(G, E, 2, 5);
    insertEdge(G, E, 2, 6);
    insertEdge(G, E, 2, 7);

    insertEdge(G, E, 3, 6);
    insertEdge(G, E, 3, 7);
    insertEdge(G, E, 3, 8);
    insertEdge(G, E, 3, 9);
    insertEdge(G, E, 3, 10);

    insertEdge(G, E, 4, 6);
    insertEdge(G, E, 4, 7);
    insertEdge(G, E, 4, 8);
    insertEdge(G, E, 4, 9);
    insertEdge(G, E, 4, 10);
}

vector<int> countColor (vector<int> &Gu, vector<Edge> &E) {
    int m = E.size();
    vector<int> Color(m + 1, 0);
    
    for (int eId: Gu) {
        if (E[eId].color != -1) {
            Color[E[eId].color]++;
        }
    }

    return Color;
}

int getFirstRepeatedColor (vector<int> &Color) {
    int m = Color.size();
    int k = -1;

    for (int clr = 0; clr < m; clr++) {
        if (Color[clr] > 1) {
            k = clr;
            break;
        }
    }

    return k;
}

int getFirstAvailableColor (vector<int> &Color) {
    int m = Color.size();
    int k = -1;

    for (int clr = 0; clr < m; clr++) {
        if (Color[clr] == 0) {
            k = clr;
            break;
        }
    }

    return k;
}

int getEdgeByColor (vector<int> &Gu, vector<Edge> &E, int &clr) {
    for (int eId: Gu) {
        if (E[eId].color == clr) {
            return eId;
        }
    }

    return -1;
} 

void getMissingColors (vector<int> &Color, int &first, int &second) {
    first = -1;
    second = -1;
    int m = Color.size();

    for (int clr = 0; clr < m; clr++) {
        if (Color[clr] == 0 && first == -1) {
            first = clr;
        } else if (Color[clr] == 0 && second == -1) {
            second = clr;
            break;
        }
    }
}

bool isColorMissing (vector<int> &Gu, vector<Edge> &E, int &clr) {
    for (int eId: Gu) {
        if (E[eId].color == clr) {
            return false;
        }
    }

    return true;
}

void RecolorPath (vector<Edge> &E, vector<int> &PrevPt, vector<int> &PrevEdge, int v, int &j, int &k) {
    while(PrevEdge[v] != -1) {
        E[PrevEdge[v]].color = E[PrevEdge[v]].color == j ? k : j;
        v = PrevPt[v]; 
    }
}

bool ColorBFS (vector<vector<int>> &G, vector<Edge> &E, int &u, int &k, int &j) {
    int n = G.size();
    vector<int> ScanQ;
    int QSize = 0;
    vector<int> OnScan(n, false);
    vector<int> PrevPt(n, -1);
    vector<int> PrevEdge(n, -1);

    for (int eV: G[u]) {
        if (E[eV].color == k) {
            int v = u == E[eV].u ? E[eV].v : E[eV].u;

            ScanQ.push_back(v);
            PrevPt[v] = u;
            PrevEdge[v] = eV;
            OnScan[v] = true;
            QSize++;
        }
    }

    int m = 0;

    while (m < QSize) {
        int v = ScanQ[m];
        m++;

        if (E[PrevEdge[v]].color == k) {
            if (isColorMissing(G[v], E, j) || (v > u)) {
                RecolorPath(E, PrevPt, PrevEdge, v, j, k);
                return true;
            }

            for (int eW: G[v]) {
                if (E[eW].color == j) {
                    int w = v == E[eW].u ? E[eW].v : E[eW].u;

                    if (OnScan[w]) continue;

                    ScanQ.push_back(w);
                    PrevPt[w] = v;
                    PrevEdge[w] = eW;
                    QSize++;
                    OnScan[w] = true;
                }
            }
        } else {
            if (isColorMissing(G[v], E, k) || (v > u)) {
                RecolorPath(E, PrevPt, PrevEdge, v, j, k);
                return true;
            }

            for (int eW: G[v]) {
                if (E[eW].color == k) {
                    int w = v == E[eW].u ? E[eW].v : E[eW].u;

                    if (OnScan[w]) continue;

                    ScanQ.push_back(w);
                    PrevPt[w] = v;
                    PrevEdge[w] = eW;
                    QSize++;
                    OnScan[w] = true;
                }
            }
        }
    }

    return false;
}

int main () {
    int n = 11;
    vector<vector<int>> G(n);
    vector<Edge> E;

    createGraph(G, E);

    for (int u = 0; u < n; u++) {
        vector<int> Color;
        Color = countColor(G[u], E);

        int k = getFirstRepeatedColor(Color);

        while (k != -1) {
            int j = -1;
            int i = -1;

            getMissingColors(Color, j, i);

            if (ColorBFS(G, E, u, k, j)) {
                Color = countColor(G[u], E);
                k = getFirstRepeatedColor(Color);
                continue;
            }

            if (ColorBFS(G, E, u, k, i)) {
                Color = countColor(G[u], E);
                k = getFirstRepeatedColor(Color);
                continue;
            }

            int eUV = getEdgeByColor(G[u], E, k);
            int l, tmp;
            int v = u == E[eUV].u ? E[eUV].v : E[eUV].u;

            Color = countColor(G[v], E);
            getMissingColors(Color, l, tmp);

            E[eUV].color = l;

            Color = countColor(G[u], E);
            k = getFirstRepeatedColor(Color);
        }

        for (int eV: G[u]) {
            if (E[eV].color == -1) {
                E[eV].color = getFirstAvailableColor(Color);
                Color[E[eV].color]++;
            }
        }
    }

    for (Edge edge : E) {
        cout << edge.u << " <-> " << edge.v << " : " << edge.color << endl;
    }

    return 0;
}