#include <iostream>
#include <vector>

#define DUMMY_NODE -1

using namespace std;

typedef struct _LBTree {
    int edgeId;
    int rPath;
    _LBTree *left;
    _LBTree *right;
} LBTree;

typedef struct _Edge {
    int x;
    int y;
    int weight;
} Edge;

void insertEdge(vector<vector<int>> &G, vector<Edge> &Edges, int u, int v, int weight) {
    int id = Edges.size();

    Edges.push_back({u, v, weight});
    G[u].push_back(id);
    G[v].push_back(id);
}

void createGraph(vector<vector<int>> &G, vector<Edge> &Edges) {
    insertEdge(G, Edges, 0, 1, 1);
    insertEdge(G, Edges, 0, 2, 1);
    insertEdge(G, Edges, 0, 4, 100);

    insertEdge(G, Edges, 1, 2, 2);

    insertEdge(G, Edges, 2, 3, 2);
    insertEdge(G, Edges, 2, 4, 3);

    insertEdge(G, Edges, 3, 4, 1);

    insertEdge(G, Edges, 5, 6, 10);
}

void removeDummy(LBTree* lbTree, vector<LBTree*> &MergeQ) {
    if(lbTree == nullptr)
        return;

    if(lbTree->edgeId != DUMMY_NODE) {
        MergeQ.push_back(lbTree);
        return;
    }

    removeDummy(lbTree->right, MergeQ);
    removeDummy(lbTree->left, MergeQ);
}

LBTree* delayMerge(LBTree* A, LBTree* B) {
    LBTree* dummyNode = new LBTree();
    
    dummyNode->edgeId = DUMMY_NODE;
    dummyNode->rPath = DUMMY_NODE;
    dummyNode->left = A;
    dummyNode->right = B;

    return dummyNode;
}

int compRep(vector<int> &CompPtr, int u) {
    if(CompPtr[u] < 0)
        return u;
    int uRep = compRep(CompPtr, CompPtr[u]);
    CompPtr[u] = uRep;
    return uRep;
} 

int mergeComp(vector<int> &CompPtr, int uRep, int vRep) {
    int uSize = -CompPtr[uRep];
    int vSize = -CompPtr[vRep];

    if(uSize > vSize) {
        CompPtr[uRep] = -(uSize + vSize);
        CompPtr[vRep] = uRep;
        return uRep;
    } else {
        CompPtr[vRep] = -(uSize + vSize);
        CompPtr[uRep] = vRep;
        return vRep;
    }
}

LBTree* LBMerge(vector<Edge> &edges, LBTree* A, LBTree* B) {
    if(A == nullptr) return B;
    if(B == nullptr) return A;

    int weightA = edges[A->edgeId].weight;
    int weightB = edges[B->edgeId].weight;
    
    if(weightA > weightB) {
        swap(A, B);
    }

    LBTree* P = nullptr;
    
    if (A->right == nullptr)
        P = B;
    else
        P = LBMerge(edges, A->right, B);

    if (A->left == nullptr) {
        A->right = nullptr;
        A->left = P;
        A->rPath = 0;
    } else {
        if (P->rPath <= A->left->rPath) {
            A->right = P;
        } else {
            A->right = A->left;
            A->left = P;
        }
        A->rPath = A->right->rPath + 1;
    }

    return A;
}

LBTree* buildLBTree(vector<Edge> &edges, vector<LBTree*> MergeQ) {
    while(MergeQ.size() > 1) {
        LBTree* A = MergeQ.back();
        MergeQ.pop_back();
        LBTree* B = MergeQ.back();
        MergeQ.pop_back();
        LBTree* AB = LBMerge(edges, A, B);
        MergeQ.push_back(AB);
    }

    return MergeQ.back();
}



int main() {
    int n = 7;
    vector<vector<int>> G(n);
    vector<Edge> Edges;
    vector<LBTree*> PQ(n, nullptr);
    vector<int> TreeQ;
    int QSize = 0;
    vector<int> CompPtr(n, -1);
    vector<Edge> Tree;
    int t = 0;

    createGraph(G, Edges);

    int m = Edges.size();


    for(int e = 0; e < m; e++) {
        int u = Edges[e].x;
        int v = Edges[e].y;

        LBTree* node1 = new LBTree();
        node1->edgeId = e;
        node1->left = nullptr;
        node1->right = nullptr;
        node1->rPath = 0;
        
        if(PQ[u] == nullptr) PQ[u] = node1;
        else PQ[u] = LBMerge(Edges, PQ[u], node1);

        LBTree* node2 = new LBTree();
        node2->edgeId = e;
        node2->left = nullptr;
        node2->right = nullptr;
        node2->rPath = 0;

        if(PQ[v] == nullptr) PQ[v] = node2;
        else PQ[v] = LBMerge(Edges, PQ[v], node2);
    }

    for(int u = 0; u < n; u++) {
        TreeQ.push_back(u);
        QSize++;
    }
    
    int k = 0;
    while((k < QSize) && (t < n - 1)) {
        int rep = TreeQ[k];
        k++;

        if(compRep(CompPtr, rep) != rep) continue;

        while(PQ[rep] != nullptr) {
            if(PQ[rep]->edgeId == DUMMY_NODE) {
                vector<LBTree*> MergeQ;
                removeDummy(PQ[rep], MergeQ);
                PQ[rep] = buildLBTree(Edges, MergeQ);
            }

            if(PQ[rep] == nullptr) break;

            int pqEdgeId = PQ[rep]->edgeId;
            int uRep = compRep(CompPtr, Edges[pqEdgeId].x);
            int vRep = compRep(CompPtr, Edges[pqEdgeId].y);

            if(uRep == vRep) {
                LBTree* leftTree = PQ[rep]->left;
                LBTree* rightTree = PQ[rep]->right;

                PQ[rep] = LBMerge(Edges, leftTree, rightTree);
                continue;
            }
            
            Tree.push_back(Edges[pqEdgeId]);
            t++;

            int newRep = mergeComp(CompPtr, uRep, vRep);
            PQ[newRep] = delayMerge(PQ[uRep], PQ[vRep]);
            TreeQ.push_back(newRep);
            QSize++;
            break;
        }
    }

    for (Edge edge: Tree) {
        cout << edge.x << " <--" << edge.weight << "--> " << edge.y << endl;  
    }

    return 0;
}