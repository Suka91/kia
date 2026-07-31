#include <iostream>
#include <vector>

using namespace std;

//Page 17. Exercise 1.2.3

//Input is vector D where index is vertex and value is vertex degree.
//Output are true/false if graph can be constructed from D and graph itself.

void printAdjList(vector<vector<int>> adjList){
    int n = adjList.size();
    for (int i = 0; i < n; i++) {
        cout << "[" << i << "]: ";
        for(int v: adjList[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

int main() {
    bool graphic = false;
    vector<int> D = {4, 4, 4, 4, 4};
    // vector<int> D = {3, 3, 3, 3, 3, 3};
    // vector<int> D = {3, 3, 3, 3, 3, 3, 3, 3};
    // vector<int> D = {3, 3, 3, 3, 3, 3, 3, 3, 3};
    // vector<int> D = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    //vector<int> D = {7, 6, 6, 6, 5, 5, 2, 1};
    int n = D.size();
    vector<vector<int>> AdjList(n);
    vector<vector<int>> TempList(n);
    vector<vector<int>> Pts(n);
    
    for (int v = 0; v < n; v++) {
        Pts[D[v]].push_back(v);
    }
    
    for (int k = n - 1; k >= 0; k--) {
        while (!Pts[k].empty()) {
            int u = Pts[k].back();
            Pts[k].pop_back();
            int i = k;
            for (int j = 1; j <= k; j++) {
                while (Pts[i].empty()) {
                    i--;
                    if(i == 0) {
                        graphic = false;
                        cout << graphic << endl;
                        return 0;
                    }
                }
                int v = Pts[i].back();
                Pts[i].pop_back();
                AdjList[u].push_back(v);
                AdjList[v].push_back(u);
                TempList[i].push_back(v);
                
            }
            for (int j = k; j > 0; j--) {
                while (!TempList[j].empty()) {
                    int v = TempList[j].back();
                    TempList[j].pop_back();
                    Pts[j-1].push_back(v);
                }
            }
        }
    }

    graphic = true;
    cout << graphic << endl;
    
    printAdjList(AdjList);

    return 0;
}