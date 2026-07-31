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
    int u0 = 0;
    int uk;
    pair<int,int> min_dist({INT_MAX, INT_MAX});
    vector<vector<pair<int,int>>> G(n);
    vector<int> dist(n, INT_MAX);
    vector<bool> used(n, false);

    createGraph(G);

    printGraph(G);

    dist[u0] = 0;

    for (int k = 1; k < n; k++) {

        min_dist = {INT_MAX, INT_MAX};
        for (int v = 0; v < n; v++) {
            if(!used[v] && (dist[v] < min_dist.second)) {
                min_dist = {v, dist[v]};
            }
        }
        
        uk = min_dist.first;
        if(uk == INT_MAX) {
            break;
        }

        used[uk] = true;

        for (pair<int,int> edge: G[uk]) {
            int v = edge.first;
            int weight = edge.second;
            if (used[v]) continue;
            if (dist[uk] + weight < dist[v]) {
                dist[v] = dist[uk] + weight;
            }
        }
    }

    for (int w: dist) {
        cout << w << " ";
    }
    cout << endl;

    return 0;
}