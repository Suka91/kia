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
    vector<int> Color = ColorRefinement;
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

    ColorRefinement = Color;
}

bool isDiscrete(vector<int> &ColorRefinement) {
    vector<int> sortedColorRefinement = ColorRefinement;
    sort(sortedColorRefinement.begin(), sortedColorRefinement.end());
    int n = ColorRefinement.size();

    for (int i = 1; i < n; i++) {
        if (sortedColorRefinement[i] == sortedColorRefinement[i - 1]) {
            return false;
        }
    }

    return true;
}

int SmallestNonTrivialCell(vector<int> &ColorRefinement) {
    int maxColor = getMaxColor(ColorRefinement);
    vector<int> ColorCount(maxColor + 1, 0);
    int m = ColorRefinement.size();                            

    for (int color = 0; color < m; color++) {
        ColorCount[ColorRefinement[color]]++;
    }

    int minCell = 0;
    int minCount = m;

    for (int i = 0; i < maxColor + 1; i++) {
        if (ColorCount[i] > 1 && ColorCount[i] < minCount) {
            minCell = i;
            minCount = ColorCount[i];
        }
    }

    return minCell;
}

void BuildSearchTree (vector<vector<int>> &G, vector<int> &ColorRefinement, vector<pair<vector<int>,vector<int>>> &PartitionNest, vector<int> IndicatorValue) {
    int maxColor = getMaxColor(ColorRefinement);
    //Number of cells in partition
    IndicatorValue.push_back(maxColor + 1);
    
    if (isDiscrete(ColorRefinement)) {
        PartitionNest.push_back({IndicatorValue, ColorRefinement});
        return;
    }

    int individualizeCell = SmallestNonTrivialCell(ColorRefinement);

    int m = ColorRefinement.size();

    for (int indivVertex = 0; indivVertex < m; indivVertex++) {
        if (ColorRefinement[indivVertex] != individualizeCell) continue;

        vector<int> CopyColorRefinement = ColorRefinement;

        CopyColorRefinement[indivVertex] = maxColor + 1;
        WlColorRefinement(G, CopyColorRefinement);
        BuildSearchTree(G, CopyColorRefinement, PartitionNest, IndicatorValue);
    }

    return;
}

void FindMaxByIndicatorValue(vector<pair<vector<int>,vector<int>>>  &PartitionNest, vector<vector<int>> &MaxColorRefinements) {
    sort(PartitionNest.begin(), PartitionNest.end(), [](const auto &a, const auto &b) {return a.first > b.first;});

    vector<int> maxIndicator = PartitionNest[0].first;

    for (auto leaf: PartitionNest) {
        if (leaf.first != maxIndicator) break;

        MaxColorRefinements.push_back(leaf.second);
    }
}

void ComputeAdjEncoding(vector<vector<int>> &G, vector<vector<int>> &MaxColorRefinements, vector<vector<int>> &CandidateEncodings) {
    int n = G.size();
    vector<vector<bool>> AdjMatrix(n, vector<bool> (n, false));

    for (int u = 0; u < n; u++) {
        for (int v: G[u]) {
            AdjMatrix[u][v] = true;
            AdjMatrix[v][u] = true;
        }
    }
    
    for (const auto &refinement: MaxColorRefinements) {
        int m = refinement.size();
        vector<int> reorderedRefinement(m, 0);

        /*
        0 1 2 3
        3 0 1 2 
        
        1 2 3 0
        */
        for (int i = 0; i < m; i++) {
            reorderedRefinement[refinement[i]] = i;
        }

        vector<vector<int>> AdjMatrixEncoded(n, vector<int> (n, 0));
    
        for (int u = 0; u < n; u++) {
            for (int v = u; v < n; v++) {
                int rU = reorderedRefinement[u];
                int rV = reorderedRefinement[v];
                if(AdjMatrix[rU][rV]) {
                    AdjMatrixEncoded[u][v] = 1;
                    AdjMatrixEncoded[v][u] = 1;
                }
            }
        }

        vector<int> encoding;
        for (int u = 0; u < n; u++) {
            for (int v = u; v < n; v++) {
                encoding.push_back(AdjMatrixEncoded[u][v]);
            }
        }

        CandidateEncodings.push_back(encoding);
    }
}

void FindMaxByAdjEncoding(vector<vector<int>> &CandidateEncodings, vector<int> &CanonicalForm) {
    sort(CandidateEncodings.begin(), CandidateEncodings.end(), [] (const auto &a, const auto &b) {return a > b;});

    CanonicalForm = CandidateEncodings[0];
}

void ComputeCanonicalForm(vector<vector<int>> &G, vector<int> &CanonicalForm) {
    int n = G.size();
    vector<int> ColorRefinement(n, 0);
    vector<pair<vector<int>, vector<int>>> PartitionNest;
    vector<int> IndicatorValue;
    vector<vector<int>> MaxColorRefinements;
    vector<vector<int>> CandidateEncodings;

    //initial P1 refinement 
    WlColorRefinement(G, ColorRefinement);

    BuildSearchTree(G, ColorRefinement, PartitionNest, IndicatorValue);

    FindMaxByIndicatorValue(PartitionNest, MaxColorRefinements);

    ComputeAdjEncoding(G, MaxColorRefinements, CandidateEncodings);

    FindMaxByAdjEncoding(CandidateEncodings, CanonicalForm);
}

void CompareCanonicalForm(vector<int> &CanonicalFormA, vector<int> &CanonicalFormB) {
    for (int c: CanonicalFormA) {
        cout << c << " ";
    }
    cout << endl;

    for (int c: CanonicalFormB) {
        cout << c << " ";
    }
    cout << endl;

    if (CanonicalFormA != CanonicalFormB) {
        cout << "Non isomorphic" << endl;
    } else {
        cout << "Isomorphic" << endl;
    }
    cout << endl;
}

int main () {
    int n = 10;
    vector<vector<int>> GA(n);
    vector<int> CanonicalFormA;
    vector<vector<int>> GB(n);
    vector<int> CanonicalFormB;
    vector<vector<int>> GC(n);
    vector<int> CanonicalFormC;
    vector<vector<int>> GD(n);
    vector<int> CanonicalFormD;

    createGraphA(GA);
    createGraphB(GB);
    createGraphC(GC);
    createGraphD(GD);

    ComputeCanonicalForm(GA, CanonicalFormA);
    ComputeCanonicalForm(GB, CanonicalFormB);
    CompareCanonicalForm(CanonicalFormA, CanonicalFormB);

    ComputeCanonicalForm(GC, CanonicalFormC);
    CompareCanonicalForm(CanonicalFormA, CanonicalFormC);


    ComputeCanonicalForm(GD, CanonicalFormD);
    CompareCanonicalForm(CanonicalFormA, CanonicalFormD);


    return 0;
}