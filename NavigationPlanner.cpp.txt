#include <bits/stdc++.h>
using namespace std;

// ===================== Building Data ADT =====================

struct Building {
    int buildingID;
    string buildingName;
    string locationDetails;
};

// ===================== BST (for BuildingTree) =====================

struct BSTNode {
    Building data;
    BSTNode *left;
    BSTNode *right;
    BSTNode(const Building &b) : data(b), left(nullptr), right(nullptr) {}
};

class BuildingBST {
    BSTNode *root;

    BSTNode* insertRec(BSTNode *node, const Building &b) {
        if (!node) return new BSTNode(b);
        if (b.buildingID < node->data.buildingID)
            node->left = insertRec(node->left, b);
        else if (b.buildingID > node->data.buildingID)
            node->right = insertRec(node->right, b);
        return node;
    }

    void inorderRec(BSTNode *node) {
        if (!node) return;
        inorderRec(node->left);
        printBuilding(node->data);
        inorderRec(node->right);
    }

    void preorderRec(BSTNode *node) {
        if (!node) return;
        printBuilding(node->data);
        preorderRec(node->left);
        preorderRec(node->right);
    }

    void postorderRec(BSTNode *node) {
        if (!node) return;
        postorderRec(node->left);
        postorderRec(node->right);
        printBuilding(node->data);
    }

    void destroy(BSTNode *node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BuildingBST() : root(nullptr) {}
    ~BuildingBST() { destroy(root); }

    void insertBuilding(const Building &b) {
        root = insertRec(root, b);
    }

    void traverseBuildings(int mode) {
        // 1: inorder, 2: preorder, 3: postorder
        if (!root) {
            cout << "No buildings in tree.\n";
            return;
        }
        if (mode == 1) inorderRec(root);
        else if (mode == 2) preorderRec(root);
        else if (mode == 3) postorderRec(root);
        else cout << "Invalid traversal mode.\n";
    }

    void printBuilding(const Building &b) {
        cout << "ID: " << b.buildingID
             << ", Name: " << b.buildingName
             << ", Location: " << b.locationDetails << "\n";
    }
};

// ===================== Simple AVL (height-balanced) =====================

struct AVLNode {
    Building data;
    AVLNode *left;
    AVLNode *right;
    int height;
    AVLNode(const Building &b) : data(b), left(nullptr), right(nullptr), height(1) {}
};

class BuildingAVL {
    AVLNode *root;

    int h(AVLNode *n) { return n ? n->height : 0; }

    int getBalance(AVLNode *n) { return n ? h(n->left) - h(n->right) : 0; }

    AVLNode* rightRotate(AVLNode *y) {
        AVLNode *x = y->left;
        AVLNode *T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(h(y->left), h(y->right)) + 1;
        x->height = max(h(x->left), h(x->right)) + 1;
        return x;
    }

    AVLNode* leftRotate(AVLNode *x) {
        AVLNode *y = x->right;
        AVLNode *T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(h(x->left), h(x->right)) + 1;
        y->height = max(h(y->left), h(y->right)) + 1;
        return y;
    }

    AVLNode* insertRec(AVLNode *node, const Building &b) {
        if (!node) return new AVLNode(b);
        if (b.buildingID < node->data.buildingID)
            node->left = insertRec(node->left, b);
        else if (b.buildingID > node->data.buildingID)
            node->right = insertRec(node->right, b);
        else
            return node;

        node->height = 1 + max(h(node->left), h(node->right));
        int balance = getBalance(node);

        if (balance > 1 && b.buildingID < node->left->data.buildingID)
            return rightRotate(node);
        if (balance < -1 && b.buildingID > node->right->data.buildingID)
            return leftRotate(node);
        if (balance > 1 && b.buildingID > node->left->data.buildingID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && b.buildingID < node->right->data.buildingID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void inorderRec(AVLNode *node) {
        if (!node) return;
        inorderRec(node->left);
        printBuilding(node->data);
        inorderRec(node->right);
    }

    void destroy(AVLNode *node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BuildingAVL() : root(nullptr) {}
    ~BuildingAVL() { destroy(root); }

    void insertBuilding(const Building &b) {
        root = insertRec(root, b);
    }

    void inorder() {
        if (!root) {
            cout << "No buildings in AVL tree.\n";
            return;
        }
        inorderRec(root);
    }

    void printBuilding(const Building &b) {
        cout << "ID: " << b.buildingID
             << ", Name: " << b.buildingName
             << ", Location: " << b.locationDetails << "\n";
    }
};

// ===================== Graph (Adjacency List) =====================

class CampusGraph {
    int V;
    vector<vector<pair<int,int>>> adj; // to, weight
    vector<string> buildingNames;

public:
    CampusGraph(int v = 0) : V(v), adj(v), buildingNames(v) {}

    void reset(int v) {
        V = v;
        adj.assign(v, {});
        buildingNames.assign(v, "");
    }

    void setBuildingName(int idx, const string &name) {
        if (idx >= 0 && idx < V) buildingNames[idx] = name;
    }

    void addEdge(int u, int v, int w) {
        if (u < 0 || v < 0 || u >= V || v >= V) return;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void printGraph() {
        for (int i = 0; i < V; i++) {
            cout << "Building " << i << " (" << buildingNames[i] << ") -> ";
            for (auto &p : adj[i]) {
                cout << "[" << p.first << " (w=" << p.second << ")] ";
            }
            cout << "\n";
        }
    }

    void dijkstra(int src) {
        const int INF = 1e9;
        vector<int> dist(V, INF);
        dist[src] = 0;
        using P = pair<int,int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (auto &edge : adj[u]) {
                int v = edge.first, w = edge.second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Shortest distances from building " << src << ":\n";
        for (int i = 0; i < V; i++) {
            cout << "To " << i << " (" << buildingNames[i] << "): ";
            if (dist[i] == INF) cout << "INF\n";
            else cout << dist[i] << "\n";
        }
    }

    struct Edge { int u, v, w; };

    void kruskalMST() {
        vector<Edge> edges;
        for (int u = 0; u < V; u++) {
            for (auto &p : adj[u]) {
                int v = p.first, w = p.second;
                if (u < v) edges.push_back({u, v, w});
            }
        }
        sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
            return a.w < b.w;
        });

        vector<int> parent(V), rankV(V,0);
        iota(parent.begin(), parent.end(), 0);

        function<int(int)> findSet = [&](int x) {
            if (parent[x] == x) return x;
            return parent[x] = findSet(parent[x]);
        };

        auto unionSet = [&](int a, int b) {
            a = findSet(a);
            b = findSet(b);
            if (a != b) {
                if (rankV[a] < rankV[b]) swap(a,b);
                parent[b] = a;
                if (rankV[a] == rankV[b]) rankV[a]++;
                return true;
            }
            return false;
        };

        int mstCost = 0;
        cout << "Kruskal MST edges (for utility layout):\n";
        for (auto &e : edges) {
            if (unionSet(e.u, e.v)) {
                mstCost += e.w;
                cout << e.u << " (" << buildingNames[e.u] << ") - "
                     << e.v << " (" << buildingNames[e.v] << "), cost=" << e.w << "\n";
            }
        }
        cout << "Total cable cost (MST weight): " << mstCost << "\n";
    }
};

// ===================== Expression Tree for Energy Bill =====================

struct ExprNode {
    string val;
    ExprNode *left;
    ExprNode *right;
    ExprNode(const string &v) : val(v), left(nullptr), right(nullptr) {}
};

class ExpressionTree {
    ExprNode *root;

    bool isOperator(const string &s) {
        return s == "+" || s == "-" || s == "*" || s == "/";
    }

    double evalRec(ExprNode *node) {
        if (!node) return 0.0;
        if (!isOperator(node->val)) {
            return stod(node->val);
        }
        double l = evalRec(node->left);
        double r = evalRec(node->right);
        if (node->val == "+") return l + r;
        if (node->val == "-") return l - r;
        if (node->val == "*") return l * r;
        return r != 0 ? l / r : 0.0;
    }

    void destroy(ExprNode *node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ExpressionTree() : root(nullptr) {}
    ~ExpressionTree() { destroy(root); }

    void buildFromPostfix(const vector<string> &postfix) {
        destroy(root);
        root = nullptr;
        stack<ExprNode*> st;
        for (const string &token : postfix) {
            if (!isOperator(token)) {
                st.push(new ExprNode(token));
            } else {
                ExprNode *r = st.top(); st.pop();
                ExprNode *l = st.top(); st.pop();
                ExprNode *op = new ExprNode(token);
                op->left = l;
                op->right = r;
                st.push(op);
            }
        }
        if (!st.empty()) root = st.top();
    }

    double evaluateExpression() {
        if (!root) return 0.0;
        return evalRec(root);
    }
};

// ===================== Campus Navigation and Utility Planner =====================

class CampusPlanner {
    BuildingBST buildingTree;
    BuildingAVL buildingAVL;
    CampusGraph graph;
    ExpressionTree exprTree;

public:
    void addBuildingRecord() {
        Building b;
        cout << "Enter BuildingID (index in graph): ";
        cin >> b.buildingID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Building Name: ";
        getline(cin, b.buildingName);
        cout << "Enter Location Details: ";
        getline(cin, b.locationDetails);

        buildingTree.insertBuilding(b);
        buildingAVL.insertBuilding(b);

        cout << "Building inserted into BST and AVL.\n";
    }

    void listCampusLocations() {
        int ch;
        cout << "Traversal type: 1.Inorder  2.Preorder  3.Postorder\n";
        cin >> ch;
        cout << "BST Traversal:\n";
        buildingTree.traverseBuildings(ch);
        cout << "AVL Inorder (sorted by ID):\n";
        buildingAVL.inorder();
    }

    void constructCampusGraph() {
        int V;
        cout << "Enter number of buildings (vertices): ";
        cin >> V;
        graph.reset(V);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        for (int i = 0; i < V; i++) {
            string name;
            cout << "Enter name for building index " << i << ": ";
            getline(cin, name);
            graph.setBuildingName(i, name);
        }
        int E;
        cout << "Enter number of paths (edges): ";
        cin >> E;
        for (int i = 0; i < E; i++) {
            int u, v, w;
            cout << "Edge " << i + 1 << " (u v distance): ";
            cin >> u >> v >> w;
            graph.addEdge(u, v, w);
        }
        cout << "Campus graph (adjacency list):\n";
        graph.printGraph();
    }

    void findOptimalPath() {
        int src;
        cout << "Enter source building index for Dijkstra: ";
        cin >> src;
        graph.dijkstra(src);
    }

    void planUtilityLayout() {
        graph.kruskalMST();
    }

    void evaluateEnergyExpression() {
        cout << "Enter postfix expression tokens separated by space (e.g., 100 2 * 50 +):\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string line;
        getline(cin, line);
        stringstream ss(line);
        vector<string> tokens;
        string tok;
        while (ss >> tok) tokens.push_back(tok);
        exprTree.buildFromPostfix(tokens);
        double result = exprTree.evaluateExpression();
        cout << "Evaluated energy bill: " << result << "\n";
    }
};

// ===================== main =====================

int main() {

    CampusPlanner planner;
    int choice;

    while (true) {
        cout << "\n===== Campus Navigation and Utility Planner =====\n";
        cout << "1. Add Building Record (BST & AVL)\n";
        cout << "2. List Campus Locations (Tree Traversals)\n";
        cout << "3. Construct Campus Graph & Show\n";
        cout << "4. Find Optimal Path (Dijkstra)\n";
        cout << "5. Plan Utility Layout (Kruskal MST)\n";
        cout << "6. Evaluate Energy Bill (Expression Tree)\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: planner.addBuildingRecord(); break;
            case 2: planner.listCampusLocations(); break;
            case 3: planner.constructCampusGraph(); break;
            case 4: planner.findOptimalPath(); break;
            case 5: planner.planUtilityLayout(); break;
            case 6: planner.evaluateEnergyExpression(); break;
            case 7: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
