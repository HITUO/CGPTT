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

struct DSU {
    vector<int> fa, sz;
    DSU(int n = 0) { init(n); }
    void init(int n) { fa.resize(n + 1); iota(fa.begin(), fa.end(), 0); sz.assign(n + 1, 1); }
    int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
    bool unite(int a, int b) { a=find(a); b=find(b); if(a==b)return false; if(sz[a]<sz[b])swap(a,b); fa[b]=a; sz[a]+=sz[b]; return true; }
    bool same(int a,int b){return find(a)==find(b);}
};

// 4. 图论：存图、BFS、DFS、拓扑排序、最短路、最小生成树
// ============================================================

// 邻接表：vector<vector<pair<int,ll>>> g(n+1);
// 无权边可使用 vector<vector<int>> g(n+1)。

// BFS：无权图最短路，起点 s，返回距离；不可达为 -1。
vector<int> bfs_shortest(const vector<vector<int>>& g, int s) {
    int n = (int)g.size() - 1;
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[s] = 0; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (dist[v] == -1) {
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}

// 拓扑排序：有向无环图 DAG；返回序列，若长度 < n 则有环。
vector<int> topological_sort(const vector<vector<int>>& g) {
    int n = (int)g.size() - 1;
    vector<int> deg(n + 1, 0), order;
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) ++deg[v];
    queue<int> q;
    for (int i = 1; i <= n; ++i) if (!deg[i]) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) if (--deg[v] == 0) q.push(v);
    }
    return order;
}

// Dijkstra：非负边权；返回源点 s 到各点最短距离。
vector<ll> dijkstra(const vector<vector<pair<int,ll>>>& g, int s) {
    int n = (int)g.size() - 1;
    vector<ll> dist(n + 1, LINF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    dist[s] = 0; pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// Bellman-Ford：允许负边；若第 n 轮仍可松弛，则存在可从源点到达的负环。
struct Edge { int u, v; ll w; };
bool bellman_ford(int n, int s, const vector<Edge>& edges, vector<ll>& dist) {
    dist.assign(n + 1, LINF);
    dist[s] = 0;
    for (int i = 1; i <= n; ++i) {
        bool changed = false;
        for (auto e : edges) if (dist[e.u] != LINF &&
                                  dist[e.v] > dist[e.u] + e.w) {
            dist[e.v] = dist[e.u] + e.w;
            changed = true;
            if (i == n) return false;
        }
        if (!changed) break;
    }
    return true;
}

// Floyd：多源最短路，O(n^3)；初始化 dist[i][i]=0，不存在边为 LINF。
void floyd_warshall(vector<vector<ll>>& d) {
    int n = (int)d.size() - 1;
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                if (d[i][k] != LINF && d[k][j] != LINF)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

// Kruskal：无向连通带权图最小生成树；不连通时返回 false。
ll kruskal(int n, vector<Edge> edges, bool& connected) {
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });
    DSU dsu(n);
    ll ans = 0;
    int used = 0;
    for (auto e : edges) if (dsu.unite(e.u, e.v)) {
        ans += e.w;
        if (++used == n - 1) break;
    }
    connected = (n <= 1 || used == n - 1);
    return ans;
}

// 0-1 BFS：边权只能是 0 或 1；双端队列实现最短路。
vector<int> zero_one_bfs(const vector<vector<pair<int,int>>>& g, int s) {
    int n = (int)g.size() - 1;
    vector<int> dist(n + 1, INF);
    deque<int> dq;
    dist[s] = 0;
    dq.push_front(s);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : g[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist;
}

// Prim：稠密图或邻接表均可使用；这里为邻接表版本。
// g[u] 存 (v,w)，无向图；不连通时 connected=false。
ll prim(const vector<vector<pair<int,ll>>>& g, bool& connected, int s = 1) {
    int n = (int)g.size() - 1;
    vector<char> vis(n + 1, false);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    pq.push({0, s});
    ll ans = 0;
    int cnt = 0;
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        ans += w;
        ++cnt;
        for (auto [v, cost] : g[u])
            if (!vis[v]) pq.push({cost, v});
    }
    connected = (cnt == n);
    return ans;
}

// Tarjan 求有向图强连通分量 SCC。编号 1..n。
// scc_id[u] 为 u 所属分量编号；分量编号按 Tarjan 出栈顺序生成。
struct TarjanSCC {
    int n, timer = 0, scc_cnt = 0;
    vector<vector<int>> g;
    vector<int> dfn, low, stk, in_stk, scc_id, scc_size;

    TarjanSCC(int n) : n(n), g(n+1), dfn(n+1), low(n+1),
        in_stk(n+1), scc_id(n+1) {}
    void add_edge(int u, int v) { g[u].push_back(v); }
    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in_stk[u] = 1;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stk[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            ++scc_cnt;
            int cnt = 0;
            while (true) {
                int v = stk.back(); stk.pop_back();
                in_stk[v] = 0;
                scc_id[v] = scc_cnt;
                ++cnt;
                if (v == u) break;
            }
            scc_size.push_back(cnt);
        }
    }
    void run() {
        for (int i = 1; i <= n; ++i)
            if (!dfn[i]) dfs(i);
    }
};

// Tarjan 求无向图桥与割点。注意用边编号处理重边。
// bridge_ids 保存桥的边编号；is_cut[u] 表示 u 是割点。
struct BridgeCut {
    int n, timer = 0, edge_cnt = 0;
    vector<vector<pair<int,int>>> g;
    vector<int> dfn, low, is_cut, bridge_ids;

    BridgeCut(int n) : n(n), g(n+1), dfn(n+1), low(n+1), is_cut(n+1) {}
    void add_edge(int u, int v) {
        ++edge_cnt;
        g[u].push_back({v, edge_cnt});
        g[v].push_back({u, edge_cnt});
    }
    void dfs(int u, int parent_edge = 0) {
        dfn[u] = low[u] = ++timer;
        int children = 0;
        for (auto [v, id] : g[u]) {
            if (id == parent_edge) continue;
            if (!dfn[v]) {
                ++children;
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) bridge_ids.push_back(id);
                if (parent_edge && low[v] >= dfn[u]) is_cut[u] = 1;
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (!parent_edge && children > 1) is_cut[u] = 1;
    }
    void run() {
        for (int i = 1; i <= n; ++i)
            if (!dfn[i]) dfs(i);
    }
};

// 树的直径（边权非负时可用两次树遍历；无权树也适用）。
// 返回最远点及距离；g 为无向树邻接表，节点 1..n。
pair<int,ll> tree_farthest(const vector<vector<pair<int,ll>>>& g, int s) {
    int n = (int)g.size() - 1;
    vector<ll> dist(n + 1, -1);
    vector<int> parent(n + 1, 0);
    vector<int> st = {s};
    dist[s] = 0;
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        for (auto [v, w] : g[u]) if (v != parent[u]) {
            parent[v] = u;
            dist[v] = dist[u] + w;
            st.push_back(v);
        }
    }
    int best = s;
    for (int i = 1; i <= n; ++i)
        if (dist[i] > dist[best]) best = i;
    return {best, dist[best]};
}

// ============================================================
