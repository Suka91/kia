#include <iostream>
#include <vector>
#include <list>

using namespace std;

void insertEdge (vector<vector<int>> &G, vector<vector<bool>> &Adj, int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);

    Adj[u][v] = true;
    Adj[v][u] = true;
}

void createGraph (vector<vector<int>> &G, vector<vector<bool>> &Adj) {
    insertEdge(G, Adj, 0, 1);
    insertEdge(G, Adj, 0, 2);
    insertEdge(G, Adj, 0, 3);

    insertEdge(G, Adj, 1, 4);
    insertEdge(G, Adj, 1, 5);

    insertEdge(G, Adj, 2, 4);
    insertEdge(G, Adj, 2, 5);
    insertEdge(G, Adj, 2, 6);

    insertEdge(G, Adj, 3, 5);
    insertEdge(G, Adj, 3, 6);

    insertEdge(G, Adj, 4, 7);
    insertEdge(G, Adj, 5, 7);
    insertEdge(G, Adj, 6, 7);
}

int main () {
    int n = 8;
    vector<vector<int>> G(n);
    vector<vector<bool>> Adj(n, vector<bool>(n, false));
    vector<bool> inP(n, false);
    createGraph(G, Adj);

    int x = 0;

    int u = x;
    int v = x;

    list<int> P;
    P.push_back(x);
    inP[x] = true;

    while (true) {
        while (true) {
            bool bExtended = false;

            for (int w: G[u]) {
                if (!inP[w]) {
                    P.push_front(w);
                    inP[w] = true;
                    u = w;
                    bExtended = true;

                    break;
                }
            }

            if (!bExtended) {
                break;
            }
        }

        while (true) {
            bool bExtended = false;

            for (int w: G[v]) {
                if (!inP[w]) {
                    P.push_back(w);
                    inP[w] = true;
                    v = w;
                    bExtended = true;

                    break;
                }
            }

            if (!bExtended) {
                break;
            }
        }

        bool bCrossoverFound = false;

        for (auto it = P.begin(); it != P.end(); it++) {
            int w = *it;
            auto itNext = it;
            itNext++;

            if (itNext == P.end()) break;
            
            int wPlus = *itNext;

            if (Adj[w][wPlus] && inP[wPlus] && Adj[u][wPlus] && Adj[v][w]) {
                list<int> secondPart;
                
                secondPart.splice(secondPart.begin(), P, itNext, P.end());
                secondPart.reverse();
                
                P.splice(P.end(), secondPart);
                int listSize = P.size();

                if (listSize == n) {
                    for (int it: P) {
                        cout << it << " ";
                    }
                    cout << endl;

                    return 0;
                }

                bCrossoverFound = true;
                break;
            }
        }

        if (bCrossoverFound) {
            bool isZFound = false;
            for (auto it = P.begin(); it != P.end(); it++) {
                int z = *it;

                for (int y: G[z]) {
                    if (!inP[y] && Adj[z][y]) {
                        auto itNext = it;
                        itNext++;
                        int zPlus;

                        if (itNext == P.end()) {
                            zPlus = P.front();
                        } else {
                            zPlus = *itNext;
                        }

                        list<int> firstPart;
                        list<int> yList;

                        yList.push_back(y);

                        firstPart.splice(firstPart.begin(), P, P.begin(), itNext);
                        firstPart.splice(firstPart.end(), yList);
                        firstPart.reverse();
                        P.reverse();
                        
                        firstPart.splice(firstPart.end(), P);

                        P = firstPart;

                        u = y;
                        v = zPlus;
                        inP[y] = true;

                        isZFound = true;

                        break;
                    }
                }

                if (isZFound) break;
            }
        } else {
            cout << "Crossover not found" << endl;

            return 0;
        }
    }

    return 0;
}