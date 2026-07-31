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
    insertEdge(G, 0, 1, 3);
    insertEdge(G, 0, 2, 10);

    insertEdge(G, 1, 2, 1);
    insertEdge(G, 1, 3, 2);

    insertEdge(G, 2, 3, 4);

    insertEdge(G, 3, 4, 2);
    insertEdge(G, 0, 4, 100);

    insertEdge(G, 2, 4, 20);
}

void printPath(vector<vector<int>> &prevPt, int start, int end) {
    vector<int> path;

    if(prevPt[start][end] == -1){
        cout << "INF" << endl;
    } else {
        path.push_back(start);
        
        while(start != end) {
            start = prevPt[start][end];
        
            if(start == -1) {
                cout << "INF" << endl;
                break;
            }
        
            path.push_back(start);
        }
        
        if(start == end) {
            for(int v: path) {
                cout << v << " ";
            }
            cout << endl;
        } 
    }
}

int main () {
    int n = 5;
    vector<vector<pair<int,int>>> G(n);
    vector<vector<int>> dist(n, (vector<int>(n, INT_MAX/3)));
    vector<vector<int>> prevPt(n, (vector<int>(n, -1))); 

    createGraph(G);
    printGraph(G);

    for(int u = 0; u < n; u++) {
        dist[u][u] = 0;
        for (pair<int,int> edge: G[u]){
            int v = edge.first;
            int weight = edge.second;
            dist[u][v] = weight;
            dist[v][u] = weight;
            prevPt[u][v] = v;
            prevPt[v][u] = u;
        }
    }

    for (int k = 0; k < n; k++) {
        for (int v = 0; v < n; v++) {
            for (int w = 0; w < n; w++) {
                if (dist[v][w] > dist[v][k] + dist[k][w]) {
                    dist[v][w] = dist[v][k] + dist[k][w];
                    prevPt[v][w] = prevPt[v][k];
                }
            }
        }
    }

    for (int u = 0; u < n; u++) {
        cout << "[" << u << "]:";
        for (int v = 0; v < n; v++) {
            cout << " " << dist[u][v];
        }
        cout << endl;
    }

    for (int u = 0; u < n; u++) {
        cout << "[" << u << "]:";
        for (int v = 0; v < n; v++) {
            cout << " " << prevPt[u][v];
        }
        cout << endl;
    }

    printPath(prevPt, 0, 4);
}