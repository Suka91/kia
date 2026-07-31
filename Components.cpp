#include <iostream>
#include <vector>

using namespace std;

//Page 27. Exercise 2.2.1

//Input graph G
//Output is number of components NComps and component vector

void printGraph(vector<vector<int>> &G){
    int n = G.size();
    for (int i = 0; i < n; i++) {
        cout << "[" << i << "]: ";
        for(int v: G[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

void insertEdge(vector<vector<int>> &G, int u, int v){
    G[u].push_back(v);
    G[v].push_back(u);
}

int CompRep(vector<int> &CompPtr, int u){
    if(CompPtr[u] < 0){
        return u;
    }
    int theRep = CompRep(CompPtr, CompPtr[u]);
    CompPtr[u] = theRep;
    return theRep;
}

void Merge(vector<int> &CompPtr, int uRep, int vRep){
    int uSize = -CompPtr[uRep];
    int vSize = -CompPtr[vRep];
    if(uSize < vSize){
        CompPtr[uRep] = vRep;
        CompPtr[vRep] = -(uSize + vSize);
    } else {
        CompPtr[vRep] = uRep;
        CompPtr[uRep] = -(uSize + vSize); 
    }
}

int main() {
    int n = 10;
    int NComps = 10;
    vector<vector<int>> G(n);
    vector<int> CompPtr(n, -1);

    insertEdge(G, 0, 1);
    insertEdge(G, 1, 2);
    insertEdge(G, 2, 3);
    insertEdge(G, 1, 4);
    insertEdge(G, 4, 2);

    insertEdge(G, 5, 6);
    insertEdge(G, 6, 7);
    insertEdge(G, 7, 5);
    
    printGraph(G);

    for (int u = 0; u < n; u++){
        for (int v: G[u]){
            if(u < v){
                int uRep = CompRep(CompPtr, u);
                int vRep = CompRep(CompPtr, v);
                if(uRep != vRep){
                    Merge(CompPtr, uRep, vRep);
                    NComps--;
                }
            }
        }
    }

    cout << NComps << endl;

    for(int i: CompPtr){
        cout << i << " ";
    }
    cout << endl;

    return 0;
}