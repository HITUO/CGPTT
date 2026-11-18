#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
using vi = vector<int>;
using vll = vector<ll>;
constexpr int INF = 0x3f3f3f3f;
constexpr ll LINF = (1LL << 62);

// 5. 树：LCA（二进制提升）
// ============================================================

struct LCA {
    int n, LOG;
    vector<int> dep;
    vector<vector<int>> up;
    vector<vector<int>> g;

    LCA(int n) : n(n) {
        LOG = 1;
        while ((1 << LOG) <= n) ++LOG;
        dep.assign(n + 1, 0);
        up.assign(LOG, vector<int>(n + 1, 0));
        g.assign(n + 1, {});
    }
    void add_edge(int u, int v) {
        g[u].push_back(v); g[v].push_back(u);
    }
    void build(int root = 1) {
        queue<int> q;
        vector<int> vis(n + 1, 0);
        vis[root] = 1; dep[root] = 1; q.push(root);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            up[0][u] = (u == root ? 0 : up[0][u]);
            for (int v : g[u]) if (!vis[v]) {
                vis[v] = 1;
                dep[v] = dep[u] + 1;
                up[0][v] = u;
                q.push(v);
            }
        }
        for (int j = 1; j < LOG; ++j)
            for (int v = 1; v <= n; ++v)
                up[j][v] = up[j-1][up[j-1][v]];
    }
    int query(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        int diff = dep[u] - dep[v];
        for (int j = 0; j < LOG; ++j)
            if (diff >> j & 1) u = up[j][u];
        if (u == v) return u;
        for (int j = LOG - 1; j >= 0; --j)
            if (up[j][u] != up[j][v]) {
                u = up[j][u]; v = up[j][v];
            }
        return up[0][u];
    }
};

// ============================================================
