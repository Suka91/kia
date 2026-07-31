#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void printGraph(vector<vector<pair<int,int>>> &G) {
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

void floatDown(vector<int> &H, int heapSize, vector<int> &dist, vector<int> &pos, int k) {
    int temp_dist = dist[H[k]];
    int temp_Hk = H[k];

    while((k > 0) && (2*k <= heapSize)) {
        int i = 2*k;
        int j = i + 1;

        if(j > heapSize) {
            j = 0;
        }

        if(dist[H[i]] > dist[H[j]]) {
            i = j;
        }

        if(temp_dist <= dist[H[i]]) {
            break;
        }

        H[k] = H[i];
        pos[H[i]] = k;
        k = i;
    }
    
    H[k] = temp_Hk;
    pos[temp_Hk] = k;
}

void floatUp(vector<int> &H, vector<int> &dist, vector<int> &pos, int k) {
    int temp_Hk = H[k];
    int temp_dist = dist[H[k]];
    int j = k/2;

    while (j > 0 && (temp_dist < dist[H[j]])) {
        H[k] = H[j];
        pos[H[j]] = k;
        k = j;
        j = k/2;
    }

    H[k] = temp_Hk;
    pos[temp_Hk] = k;
}

int main() {
    int n = 6;
    int u0 = 0;
    int uk;
    vector<vector<pair<int,int>>> G(n);
    vector<int> dist(n + 1, INT_MAX);
    vector<bool> used(n, false);
    vector<int> H(n + 1);
    vector<int> pos(n);

    createGraph(G);
    printGraph(G);

    for (int u = 0; u < n; u++) {
        H[u + 1] = u;
        pos[u] = u + 1;
    }

    H[0] = n; //INT_MAX

    dist[u0] = 0;

    for (int k = 1; k < n; k++){
        uk = H[1];
        H[1] = H[n-k+1];
        pos[H[1]] = 1;
        
        int heapSize = n-k;
        floatDown(H, heapSize, dist, pos, 1);

        used[uk] = true;

        for (pair<int, int> edge: G[uk]) {
            int v = edge.first;
            int weight = edge.second;

            if(used[v]) continue;

            if(dist[v] > dist[uk] + weight){
                dist[v] = dist[uk] + weight;
                floatUp(H, dist, pos, pos[v]);
            }
        }
    }

    for (int v = 0; v < n; v++) {
        cout << dist[v] << " ";
    }
    cout << endl;
}