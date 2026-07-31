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

void Augment (int y, vector<int> &Match, vector<int> &PrevPt, vector<int> &MatchEdge, vector<int> &PrevEdge, vector<bool> &Removed) {
    while(y != -1) {
        int w = PrevPt[y];
        Removed[w] = true;
        Removed[y] = true;

        Match[y] = w;
        int v = Match[w];

        Match[w] = y;

        int wEdgeId = PrevEdge[y];
        MatchEdge[y] = wEdgeId;
        MatchEdge[w] = wEdgeId;

        y = v;
    }
}

bool BipartiteSplit (vector<vector<int>> &G, vector<Edge> &E, vector<int> &X) {
    int n = G.size();
    vector<int> ScanQ;
    vector<int> OnScan(n, false);
    int QSize = 0;
    vector<int> BiColor(n, -1);

    int u0 = 0;
    ScanQ.push_back(u0);
    QSize++;
    OnScan[u0] = true;
    BiColor[u0] = 0;

    int k = 0;

    while (k < QSize) {
        int u = ScanQ[k];
        k++;

        for (int vEdgeId: G[u]) {
            int v = u == E[vEdgeId].u ? E[vEdgeId].v : E[vEdgeId].u;

            if (OnScan[v]) continue;
            
            if (BiColor[v] == BiColor[u]) {
                return false;
            }

            ScanQ.push_back(v);
            QSize++;
            OnScan[v] = true;

            if (BiColor[v] == -1) {
                BiColor[v] = 1 - BiColor[u];
            }
        }
    }

    for (int u = 0; u < n; u++) {
        if (BiColor[u] == 0) {
            X.push_back(u);
        }
    }

    return true;
}

int main () {
    int n = 11;
    vector<vector<int>> G(n);
    vector<int> X;
    vector<Edge> E;
    int currentColor = 0;
    

    createGraph(G, E);

    BipartiteSplit(G, E, X);

    int uncoloredEdges = E.size();

    while (uncoloredEdges > 0) {
        vector<bool> Removed(n, false);
        vector<int> Match(n, -1);
        vector<int> MatchEdge(n, -1);

        for(int u: X) {
            if(Removed[u]) continue;
            vector<int> ScanQ;
            int QSize = 1;
            vector<int> PrevPt(n, -1);
            vector<int> PrevEdge(n, -1);
            vector<bool> NS(n, false);
            bool isAugmented = false;

            ScanQ.push_back(u);
            int k = 0;

            while (k < QSize) {
                int x = ScanQ[k];
                k++;

                for (int yEdgeId: G[x]) {
                    
                    if (E[yEdgeId].color != -1) continue;

                    int y = x == E[yEdgeId].u ? E[yEdgeId].v : E[yEdgeId].u;

                    if (!NS[y]) {
                        NS[y] = true;
                        PrevPt[y] = x;
                        PrevEdge[y] = yEdgeId;

                        if(Match[y] == -1) {
                            Augment(y, Match, PrevPt, MatchEdge, PrevEdge, Removed);
                            isAugmented = true;
                            break;
                        }

                        ScanQ.push_back(Match[y]);
                        QSize++;
                    }
                }
                if(isAugmented) {
                    break;
                }
            }
        }

        for (int u = 0; u < n; u++) {
            if (MatchEdge[u] != -1 && E[MatchEdge[u]].color == -1) {
                E[MatchEdge[u]].color = currentColor;
                uncoloredEdges--;
            }
        }
        
        currentColor++;
    }

    for (Edge edge : E) {
        cout << edge.u << " <-> " << edge.v << " : " << edge.color << endl;
    }

    return 0;
}