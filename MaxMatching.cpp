#include <iostream>
#include <vector>

using namespace std;

void insertEdge(vector<vector<int>> &G, int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
}

void createGraph(vector<vector<int>> &G) {
    insertEdge(G, 0, 1);
    insertEdge(G, 0, 2);

    insertEdge(G, 1, 3);
    insertEdge(G, 1, 9);

    insertEdge(G, 2, 3);
    insertEdge(G, 2, 4);

    insertEdge(G, 3, 5);
    insertEdge(G, 3, 6);

    insertEdge(G, 4, 5);
    insertEdge(G, 4, 6);

    insertEdge(G, 5, 7);
    insertEdge(G, 5, 8);
    insertEdge(G, 5, 9);

    insertEdge(G, 6, 7);
    insertEdge(G, 6, 8);
    insertEdge(G, 6, 9);

    insertEdge(G, 7, 8);
}

int BlossomBase (vector<int> &BasePtr, int u) {
    if (BasePtr[u] == -1) return -1;

    if (BasePtr[u] == u) return u;

    int theRep = BlossomBase(BasePtr, BasePtr[u]);
    BasePtr[u] = theRep;
    return theRep;
}

void MarkSide (int current, int previous, int base, vector<int> Match, vector<int> &BasePtr, vector<int> &Marked, vector<int> &PrevPt) {
    int currentBase;
    while ((currentBase = BlossomBase(BasePtr, current)) != base) {
        int currentMatch = Match[currentBase];

        Marked[currentBase] = true;
        
        int currentMatchBase = BlossomBase(BasePtr, currentMatch);
        if (currentMatchBase == -1) {
            currentMatchBase = currentMatch;
        }

        PrevPt[currentBase] = previous;

        previous = currentMatch;
        current = PrevPt[currentMatch];
    }
}

int FindCommonBase (int xBase, int yBase, vector<int> Match, vector<int> &BasePtr, vector<int> &PrevPt) {
    int n = Match.size();
    vector<bool> Seen(n, false);

    while (true) {
        xBase = BlossomBase(BasePtr, xBase);
        Seen[xBase] = true;

        if (Match[xBase] == -1) break;

        xBase = PrevPt[Match[xBase]];
    }

    while (true) {
        yBase = BlossomBase(BasePtr, yBase);

        if(Seen[yBase]) return yBase;

        yBase = PrevPt[Match[yBase]];
    }

    cout << "Should not reach here!" << endl;
    return -1;
}

void ShrinkBlossom (int xBase, int yBase, vector<int> Match, vector<int> &BasePtr, vector<int> &PrevPt, vector<int> &ScanQ, int &QSize) {
    int base = FindCommonBase(xBase, yBase, Match, BasePtr, PrevPt);
    int n = Match.size();
    vector<int> Marked(n, false);

    MarkSide(xBase, yBase, base, Match, BasePtr, Marked, PrevPt);
    MarkSide(yBase, xBase, base, Match, BasePtr, Marked, PrevPt);
    Marked[base] = true;

    for (int u = 0; u < n; u++) {
        if (Marked[u]) {
            BasePtr[u] = base;
            ScanQ.push_back(u);
            QSize++;
        }
    }
}

void Augment(int y, vector<int> &Match, vector<int> &PrevPt) {
    while(y != -1) {
        int w = PrevPt[y];

        Match[y] = w;
        int v = Match[w];

        Match[w] = y;

        y = v;
    }
}

int main () {
    int n = 10;
    vector<vector<int>> G(n);
    vector<int> Match(n, -1);

    createGraph(G);
    
    for(int u = 0; u < n; u++) {
        if (Match[u] != -1) continue;
        
        bool augmented = false;
        vector<int> BasePtr(n, -1);
        vector<int> PrevPt(n, -1);
        vector<int> ScanQ;
        int QSize = 0;

        BasePtr[u] = u;
        ScanQ.push_back(u);
        QSize++;

        int k = 0;

        while (k < QSize) {
            int x = ScanQ[k];
            int xBase = BlossomBase(BasePtr, x);

            for (int y: G[x]) {
                int yBase = BlossomBase(BasePtr, y);

                if (xBase == yBase) continue;

                if (yBase != -1) {
                    ShrinkBlossom(xBase, yBase, Match, BasePtr, PrevPt, ScanQ, QSize);
                    continue;
                }

                if (PrevPt[y] != -1) continue;

                PrevPt[y] = x;

                if (Match[y] == -1) {
                    Augment(y, Match, PrevPt);
                    augmented = true;
                    break;
                }

                int v = Match[y];
                ScanQ.push_back(v);
                QSize++;
                BasePtr[v] = v;
            }

            if (augmented) break;
            
            k++;
        }
    }

    for (int u = 0; u < n; u++) {
        if(u < Match[u] || Match[u] == -1) {
            cout << u << " - " << Match[u] << endl;
        }
    }

    return 0;
}