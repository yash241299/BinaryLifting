#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;
const int LOG = 20;

vector<int> adj[MAXN];
int parent[MAXN][LOG];
int depth[MAXN];
int xorFromRoot[MAXN];

void dfs(int node, int par, int currXOR) {
    parent[node][0] = par;
    depth[node] = depth[par] + 1;
    xorFromRoot[node] = currXOR;

    for (int child : adj[node]) {
        if (child != par) {
            dfs(child, node, currXOR ^ child);
        }
    }
}

// Preprocess binary lifting for for O(log n) per query Query
void preprocessLCA(int n) {
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= n; i++) {
            if (parent[i][j - 1] != -1) {
                parent[i][j] = parent[parent[i][j - 1]][j - 1];
            }
        }
    }
}

// Get LCA of two nodes
int getLCA(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    for (int j = LOG - 1; j >= 0; j--) {
        if (depth[u] - (1 << j) >= depth[v]) {
            u = parent[u][j];
        }
    }

    if (u == v) return u;

    for (int j = LOG - 1; j >= 0; j--) {
        if (parent[u][j] != parent[v][j]) {
            u = parent[u][j];
            v = parent[v][j];
        }
    }

    return parent[u][0];
}

bool query(int u, int v, int k) {
    int lca = getLCA(u, v);
    
    // XOR from u to v: XOR from root to u, root to v, and exclude the LCA once
    int pathXOR = xorFromRoot[u] ^ xorFromRoot[v] ^ lca;

    if (pathXOR == k) return true;

    // Try excluding one node and see if XOR becomes k
    // XOR with any single node on the path
    if ((pathXOR ^ xorFromRoot[u]) == k || (pathXOR ^ xorFromRoot[v]) == k) {
        return true;
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    //cout << n << " " << q;
    // Input the tree
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Initialize parent array with -1
    memset(parent, -1, sizeof(parent));

    // Run DFS to initialize parent, depth, and XOR arrays
    dfs(1, -1, 1);  // Start DFS from node 1, with XOR = 1 (or any arbitrary XOR)

    // Preprocess LCA for binary lifting for O(log n) query time
    preprocessLCA(n);

    // Handling each query
    while (q--) {
        int u, v, k;
        cin >> u >> v >> k;

        if (query(u, v, k)) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
