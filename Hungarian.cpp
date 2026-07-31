#include <iostream>
#include <vector>

using namespace std;

void insertEdge(vector<vector<int>> &G, int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
}

void createGraph(vector<vector<int>> &G, vector<int> &X) {
    insertEdge(G, 0, 5);
    insertEdge(G, 0, 6);

    insertEdge(G, 1, 5);
    insertEdge(G, 1, 6);

    insertEdge(G, 2, 5);
    insertEdge(G, 2, 6);
    insertEdge(G, 2, 7);

    insertEdge(G, 3, 6);
    insertEdge(G, 3, 7);
    insertEdge(G, 3, 8);
    insertEdge(G, 3, 9);
    insertEdge(G, 3, 10);

    insertEdge(G, 4, 6);
    insertEdge(G, 4, 7);
    insertEdge(G, 4, 8);
    insertEdge(G, 4, 9);
    insertEdge(G, 4, 10);

    X.push_back(0);
    X.push_back(1);
    X.push_back(2);
    X.push_back(3);
    X.push_back(4);
}

void Augment(int y, vector<int> &Match, vector<int> &PrevPt, vector<bool> &Removed) {
    while(y != -1) {
        int w = PrevPt[y];
        Removed[w] = true;
        Removed[y] = true;

        Match[y] = w;
        int v = Match[w];

        Match[w] = y;

        y = v;
    }
}

int main () {
    int n = 11;
    vector<vector<int>> G(n);
    vector<int> X;
    vector<int> Match(n, -1);
    
    vector<bool> Removed(n, false);  

    createGraph(G, X);

    for(int u: X) {
        if(Removed[u]) continue;
        vector<int> ScanQ;
        int QSize = 1;
        vector<int> PrevPt(n, -1);
        vector<bool> NS(n, false);
        bool isAugmented = false;

        ScanQ.push_back(u);
        int k = 0;

        while (k < QSize) {
            int x = ScanQ[k];
            for (int y: G[x]) {
                if(Removed[y]) continue;

                if (!NS[y]) {
                    NS[y] = true;
                    PrevPt[y] = x;

                    if(Match[y] == -1) {
                        Augment(y, Match, PrevPt, Removed);
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
            k++;
        }
    }

    for (int x : X) {
        cout << x << " - " << Match[x] << endl;
    }

    return 0;
}