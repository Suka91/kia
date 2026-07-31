#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void printGraph(vector<vector<pair<int,int>>> &G){
    int n = G.size();
    for (int u = 0; u < n; u++) {
        cout << "[ " << u << " ]:";
        for (pair<int,int> edge: G[u]) {
            int v = edge.first;
            int weight = edge.second;
            cout << " " << v << ":" << weight;
        }
        cout << endl;
    }
}

void insertEdge(vector<vector<pair<int,int>>> &G, int u, int v, int weight) {
    G[u].push_back({v, weight});
    G[v].push_back({u, weight});
}

void createGraph(vector<vector<pair<int,int>>> &G) {
    insertEdge(G, 0, 1, 7);
    insertEdge(G, 0, 2, 9);
    insertEdge(G, 0, 5, 14);

    insertEdge(G, 1, 2, 10);
    insertEdge(G, 1, 3, 15);

    insertEdge(G, 2, 3, 11);
    insertEdge(G, 2, 5, 2);

    insertEdge(G, 3, 4, 6);

    insertEdge(G, 4, 5, 9);
}


int main() {
    int n = 6;
    vector<vector<pair<int,int>>> G(n);
    int u0 = 0;
    vector<int> dist(n, INT_MAX/3);

    createGraph(G);
    printGraph(G);

    dist[u0] = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) {
            for (pair<int,int> edge: G[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    for (int w: dist) {
        cout << w << " ";
    }
    cout << endl;
}