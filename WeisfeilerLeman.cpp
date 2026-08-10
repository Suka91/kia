#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void insertEdge (vector<vector<int>> &G, int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
}

void createGraphA (vector<vector<int>> &G) {
    insertEdge(G, 0, 1);

    insertEdge(G, 0, 2);
    insertEdge(G, 2, 6);
    insertEdge(G, 6, 7);
    insertEdge(G, 7, 4);
    insertEdge(G, 4, 1);

    insertEdge(G, 0, 3);
    insertEdge(G, 3, 8);
    insertEdge(G, 8, 9);
    insertEdge(G, 9, 5);
    insertEdge(G, 5, 1);
}

void createGraphB (vector<vector<int>> &G) {
    insertEdge(G, 0, 1);

    insertEdge(G, 0, 2);
    insertEdge(G, 2, 6);
    insertEdge(G, 6, 7);
    insertEdge(G, 7, 3);
    insertEdge(G, 3, 0);

    insertEdge(G, 1, 4);
    insertEdge(G, 4, 8);
    insertEdge(G, 8, 9);
    insertEdge(G, 9, 5);
    insertEdge(G, 5, 1);
}

void createGraphC (vector<vector<int>> &G) {
    insertEdge(G, 7, 2);

    insertEdge(G, 7, 9);
    insertEdge(G, 9, 1);
    insertEdge(G, 1, 5);
    insertEdge(G, 5, 0);
    insertEdge(G, 0, 2);

    insertEdge(G, 7, 4);
    insertEdge(G, 4, 8);
    insertEdge(G, 8, 3);
    insertEdge(G, 3, 6);
    insertEdge(G, 6, 2);
}

void createGraphD (vector<vector<int>> &G) {
    insertEdge(G, 0, 2);
    insertEdge(G, 2, 3);
    insertEdge(G, 3, 1);

    insertEdge(G, 0, 4);
    insertEdge(G, 4, 5);
    insertEdge(G, 5, 6);
    insertEdge(G, 6, 1);

    insertEdge(G, 0, 7);
    insertEdge(G, 7, 8);
    insertEdge(G, 8, 9);
    insertEdge(G, 9, 1);
}

int getMaxColor(vector<int> &Color) {
    int maxColor = 0;

    for (int color: Color) {
        if (color > maxColor) {
            maxColor = color;
        }
    }

    return maxColor;
}

void WlColorRefinement (vector<vector<int>> &G, vector<int> &ColorRefinement) {
    int n = G.size();
    vector<int> Color(n, 0);
    int maxColor = -1;

    while (maxColor != getMaxColor(Color)) {
        maxColor = getMaxColor(Color);

        vector<pair<int, vector<int>>> ColorMultisetVector(n);

        for (int u = 0; u < n; u++) {
            pair<int, vector<int>> uMultiset;
            vector<int> neighbourColors;
            uMultiset.first = Color[u];

            for (int v: G[u]) {
                neighbourColors.push_back(Color[v]);
            }

            sort(neighbourColors.begin(), neighbourColors.end());
            uMultiset.second = neighbourColors;

            ColorMultisetVector[u] = uMultiset;
        }

        vector<pair<int, vector<int>>> UniqueMultisetVector = ColorMultisetVector;
        sort(UniqueMultisetVector.begin(), UniqueMultisetVector.end());
        UniqueMultisetVector.erase(
            unique(UniqueMultisetVector.begin(), UniqueMultisetVector.end()), 
            UniqueMultisetVector.end()
        );

        for (int u = 0; u < n; u++) {
            pair<int, vector<int>> uMultiset = ColorMultisetVector[u];
            int uniqueSize = UniqueMultisetVector.size();
            int index = 0;

            while (index < uniqueSize) {
                if (uMultiset == UniqueMultisetVector[index]) {
                    break;
                }
                index++;
            }

            Color[u] = index;
        }        
    }

    sort(Color.begin(), Color.end(), greater<int>());
    ColorRefinement = Color;
}

void CompareRefinement(vector<int> &ColorRefinementA, vector<int> &ColorRefinementB) {
    for (int c: ColorRefinementA) {
        cout << c << " ";
    }
    cout << endl;

    for (int c: ColorRefinementB) {
        cout << c << " ";
    }
    cout << endl;

    if (ColorRefinementA != ColorRefinementB) {
        cout << "Non isomorphic" << endl;
    } else {
        cout << "Test is inconclusive" << endl;
    }
    cout << endl;
}


int main () {
    int n = 10;
    vector<vector<int>> GA(n);
    vector<int> ColorRefinementA(n);
    vector<vector<int>> GB(n);
    vector<int> ColorRefinementB(n);
    vector<vector<int>> GC(n);
    vector<int> ColorRefinementC(n);
    vector<vector<int>> GD(n);
    vector<int> ColorRefinementD(n);

    createGraphA(GA);
    createGraphB(GB);
    createGraphC(GC);
    createGraphD(GD);

    WlColorRefinement(GA, ColorRefinementA);
    WlColorRefinement(GB, ColorRefinementB);
    CompareRefinement(ColorRefinementA, ColorRefinementB);

    WlColorRefinement(GC, ColorRefinementC);
    CompareRefinement(ColorRefinementA, ColorRefinementC);


    WlColorRefinement(GD, ColorRefinementD);
    CompareRefinement(ColorRefinementA, ColorRefinementD);


    return 0;
}