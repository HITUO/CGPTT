/*
 * CGPTT - C++17 常用算法模板合集
 *
 * 用法：
 * 1. 本文件是“模板仓库”，不是每题都要整体复制。
 * 2. 按需复制对应结构；同名函数/结构体不要重复粘贴。
 * 3. 默认图节点、数组区间采用 1-based；字符串下标采用 0-based。
 * 4. 编译示例：g++ -std=c++17 -O2 -pipe -static -s main.cpp
 */

#include <bits/stdc++.h>
using namespace std;

// ============================================================
// 0. 基础配置
// ============================================================

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

constexpr int INF = 0x3f3f3f3f;
constexpr ll LINF = (1LL << 62);

// 快速幂：计算 a^b % mod，要求 mod > 0、b >= 0。
ll mod_pow(ll a, ll b, ll mod) {
    ll ans = 1 % mod;
    a %= mod;
    while (b) {
        if (b & 1) ans = (__int128)ans * a % mod;
        a = (__int128)a * a % mod;
        b >>= 1;
    }
    return ans;
}

// gcd / lcm（注意 lcm 乘法可能溢出）。
ll gcd_ll(ll a, ll b) { return b ? gcd_ll(b, a % b) : abs(a); }
ll lcm_ll(ll a, ll b) { return a / gcd_ll(a, b) * b; }

// 坐标离散化：返回 x 在排序去重数组中的 1-based 排名。
template<class T>
int get_rank(const vector<T>& sorted_unique, const T& x) {
    return int(lower_bound(sorted_unique.begin(), sorted_unique.end(), x)
               - sorted_unique.begin()) + 1;
}

// ============================================================
// 1. 二分、双指针、前缀和与差分
// ============================================================

// 整数二分：找第一个满足 check(x) 的 x。
// 前提：区间内 check 具有 false...false,true...true 的单调性。
template<class Check>
ll first_true(ll l, ll r, Check check) {
    while (l < r) {
        ll mid = l + (r - l) / 2;
        if (check(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}

// 找最后一个满足 check(x) 的 x（true...true,false...false）。
template<class Check>
ll last_true(ll l, ll r, Check check) {
    while (l < r) {
        ll mid = l + (r - l + 1) / 2;
        if (check(mid)) l = mid;
        else r = mid - 1;
    }
    return l;
}

// 一维前缀和：预处理后区间 [l,r] 的和为 pre[r]-pre[l-1]。
// vector<ll> pre(n+1); for (int i=1;i<=n;i++) pre[i]=pre[i-1]+a[i];

// 二维前缀和：矩阵下标 1..n, 1..m。
// sum(x1,y1,x2,y2) = s[x2][y2]-s[x1-1][y2]-s[x2][y1-1]+s[x1-1][y1-1]。

// 一维差分：区间 [l,r] 加 v：d[l]+=v, d[r+1]-=v；最后做前缀和还原。

// 双指针常用框架：维护窗口 [l,r]，按题意扩展右端并收缩左端。
// int l=0; for (int r=0;r<n;r++) { add(a[r]); while (窗口不合法) remove(a[l++]); 更新答案; }

// ============================================================
// 2. 单调栈、单调队列、ST 表
// ============================================================

// 下一个更大元素（严格大于）：ans[i] 是右侧第一个 > a[i] 的下标；不存在为 -1。
vector<int> next_greater(const vector<int>& a) {
    int n = (int)a.size();
    vector<int> ans(n, -1), st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] < a[i]) {
            ans[st.back()] = i;
            st.pop_back();
        }
        st.push_back(i);
    }
    return ans;
}

// 滑动窗口最小值：窗口长度 k，返回每个窗口的最小值。
vector<int> sliding_min(const vector<int>& a, int k) {
    int n = (int)a.size();
    vector<int> ans;
    deque<int> q;
    for (int i = 0; i < n; ++i) {
        while (!q.empty() && q.front() <= i - k) q.pop_front();
        while (!q.empty() && a[q.back()] >= a[i]) q.pop_back();
        q.push_back(i);
        if (i >= k - 1) ans.push_back(a[q.front()]);
    }
    return ans;
}

// ST 表：静态区间最小值 O(1) 查询，O(n log n) 预处理。
// 不支持修改；查询区间必须非空。
struct SparseTableMin {
    int n = 0, K = 0;
    vector<int> lg;
    vector<vector<int>> st;

    void build(const vector<int>& a) { // a 使用 1-based
        n = (int)a.size() - 1;
        K =  __lg(max(1, n)) + 1;
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i / 2] + 1;
        st.assign(K, vector<int>(n + 1, INF));
        for (int i = 1; i <= n; ++i) st[0][i] = a[i];
        for (int j = 1; j < K; ++j)
            for (int i = 1; i + (1 << j) - 1 <= n; ++i)
                st[j][i] = min(st[j-1][i], st[j-1][i + (1 << (j-1))]);
    }
    int query(int l, int r) const {
        int j = lg[r - l + 1];
        return min(st[j][l], st[j][r - (1 << j) + 1]);
    }
};

// ============================================================
// 3. 并查集、树状数组、线段树
// ============================================================

struct DSU {
    vector<int> fa, sz;
    DSU(int n = 0) { init(n); }
    void init(int n) {
        fa.resize(n + 1);
        iota(fa.begin(), fa.end(), 0);
        sz.assign(n + 1, 1);
    }
    int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        fa[b] = a; sz[a] += sz[b];
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
};

// 树状数组：单点加、前缀和、区间和，1-based。
struct Fenwick {
    int n;
    vector<ll> bit;
    Fenwick(int n = 0) : n(n), bit(n + 1, 0) {}
    void add(int x, ll v) {
        for (; x <= n; x += x & -x) bit[x] += v;
    }
    ll sum_prefix(int x) const {
        ll s = 0;
        for (; x > 0; x -= x & -x) s += bit[x];
        return s;
    }
    ll query(int l, int r) const {
        return sum_prefix(r) - sum_prefix(l - 1);
    }
};

// 普通线段树：区间求和、单点赋值，1-based。
struct SegmentTree {
    int n;
    vector<ll> tr;
    SegmentTree(const vector<ll>& a) {
        n = (int)a.size() - 1;
        tr.assign(4 * n + 4, 0);
        build(1, 1, n, a);
    }
    void build(int p, int l, int r, const vector<ll>& a) {
        if (l == r) { tr[p] = a[l]; return; }
        int m = (l + r) / 2;
        build(p*2, l, m, a); build(p*2+1, m+1, r, a);
        push_up(p);
    }
    void push_up(int p) { tr[p] = tr[p*2] + tr[p*2+1]; }
    void set_point(int p, int l, int r, int x, ll v) {
        if (l == r) { tr[p] = v; return; }
        int m = (l + r) / 2;
        if (x <= m) set_point(p*2, l, m, x, v);
        else set_point(p*2+1, m+1, r, x, v);
        push_up(p);
    }
    ll query(int p, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return tr[p];
        int m = (l + r) / 2;
        ll ans = 0;
        if (ql <= m) ans += query(p*2, l, m, ql, qr);
        if (qr > m) ans += query(p*2+1, m+1, r, ql, qr);
        return ans;
    }
};

// 懒标记线段树：区间加、区间求和，1-based。
struct LazySegmentTree {
    struct Node { ll sum = 0, lazy = 0; };
    int n;
    vector<Node> tr;
    LazySegmentTree(const vector<ll>& a) {
        n = (int)a.size() - 1;
        tr.resize(4 * n + 4);
        build(1, 1, n, a);
    }
    void pull(int p) { tr[p].sum = tr[p*2].sum + tr[p*2+1].sum; }
    void apply(int p, int l, int r, ll v) {
        tr[p].sum += (r - l + 1) * v;
        tr[p].lazy += v;
    }
    void push(int p, int l, int r) {
        if (!tr[p].lazy || l == r) return;
        int m = (l + r) / 2;
        apply(p*2, l, m, tr[p].lazy);
        apply(p*2+1, m+1, r, tr[p].lazy);
        tr[p].lazy = 0;
    }
    void build(int p, int l, int r, const vector<ll>& a) {
        if (l == r) { tr[p].sum = a[l]; return; }
        int m = (l + r) / 2;
        build(p*2, l, m, a); build(p*2+1, m+1, r, a);
        pull(p);
    }
    void add(int p, int l, int r, int ql, int qr, ll v) {
        if (ql <= l && r <= qr) { apply(p, l, r, v); return; }
        push(p, l, r);
        int m = (l + r) / 2;
        if (ql <= m) add(p*2, l, m, ql, qr, v);
        if (qr > m) add(p*2+1, m+1, r, ql, qr, v);
        pull(p);
    }
    ll query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p].sum;
        push(p, l, r);
        int m = (l + r) / 2;
        ll ans = 0;
        if (ql <= m) ans += query(p*2, l, m, ql, qr);
        if (qr > m) ans += query(p*2+1, m+1, r, ql, qr);
        return ans;
    }
};

// ============================================================
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

// ============================================================
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
// 6. 字符串：KMP、Z 函数、Trie、滚动哈希
// ============================================================

// KMP 前缀函数 pi[i]：字符串 s[0..i] 的最长相等真前后缀长度。
vector<int> prefix_function(const string& s) {
    int n = (int)s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

// KMP 匹配：返回模式串 pat 在文本 text 中所有匹配起点（0-based）。
vector<int> kmp_match(const string& text, const string& pat) {
    vector<int> ans;
    if (pat.empty()) return ans;
    auto pi = prefix_function(pat);
    for (int i = 0, j = 0; i < (int)text.size(); ++i) {
        while (j > 0 && text[i] != pat[j]) j = pi[j-1];
        if (text[i] == pat[j]) ++j;
        if (j == (int)pat.size()) {
            ans.push_back(i - j + 1);
            j = pi[j-1];
        }
    }
    return ans;
}

// Z 函数：z[i] 表示 s[i..] 与 s[0..] 的最长公共前缀长度；z[0]=n。
vector<int> z_function(const string& s) {
    int n = (int)s.size();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i-l]);
        while (i + z[i] < n && s[z[i]] == s[i+z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    if (n) z[0] = n;
    return z;
}

// 小写英文字母 Trie：插入、查询完整单词。
struct Trie {
    struct Node {
        int ch[26]{};
        bool terminal = false;
    };
    vector<Node> t = vector<Node>(1);
    void insert(const string& s) {
        int p = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!t[p].ch[x]) {
                t[p].ch[x] = (int)t.size();
                t.emplace_back();
            }
            p = t[p].ch[x];
        }
        t[p].terminal = true;
    }
    bool contains(const string& s) const {
        int p = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!t[p].ch[x]) return false;
            p = t[p].ch[x];
        }
        return t[p].terminal;
    }
};

// ============================================================
// 7. 数学与数论
// ============================================================

// 线性筛：返回 [2,n] 内所有素数。
vector<int> linear_sieve(int n) {
    vector<int> primes, lp(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        if (!lp[i]) lp[i] = i, primes.push_back(i);
        for (int p : primes) {
            if (p > lp[i] || 1LL * i * p > n) break;
            lp[i*p] = p;
        }
    }
    return primes;
}

// 扩展欧几里得：求 x,y 使 ax+by=gcd(a,b)。
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// 模逆元：仅当 gcd(a,mod)=1 时存在；mod 通常为正数。
ll mod_inverse(ll a, ll mod) {
    ll x, y;
    ll g = exgcd(a, mod, x, y);
    if (g != 1) return -1;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// 组合数预处理：要求 mod 为素数且 n < mod。
struct Combinations {
    ll mod;
    vector<ll> fac, ifac;
    Combinations(int n, ll mod) : mod(mod), fac(n+1), ifac(n+1) {
        fac[0] = 1;
        for (int i = 1; i <= n; ++i) fac[i] = fac[i-1] * i % mod;
        ifac[n] = mod_pow(fac[n], mod-2, mod);
        for (int i = n; i >= 1; --i) ifac[i-1] = ifac[i] * i % mod;
    }
    ll C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return fac[n] * ifac[k] % mod * ifac[n-k] % mod;
    }
};

// ============================================================
// 8. 动态规划常见状态转移提示（按题目改写）
// ============================================================

// 01 背包（每件物品最多选一次）：
// for (int i=1;i<=n;i++)
//   for (int j=V;j>=w[i];j--)
//     dp[j]=max(dp[j],dp[j-w[i]]+val[i]);
//
// 完全背包（每件物品可选无限次）：
// for (int i=1;i<=n;i++)
//   for (int j=w[i];j<=V;j++)
//     dp[j]=max(dp[j],dp[j-w[i]]+val[i]);
//
// 最长上升子序列 O(n log n)：
// vector<int> tail;
// for (int x:a) {
//   auto it=lower_bound(tail.begin(),tail.end(),x);
//   if (it==tail.end()) tail.push_back(x); else *it=x;
// }
// 严格递增使用 lower_bound；最长不下降子序列通常使用 upper_bound。

// ============================================================
// 9. 常用 STL 提醒
// ============================================================

// 排序：sort(a.begin(), a.end());
// 去重（先排序）：a.erase(unique(a.begin(), a.end()), a.end());
// 二分：lower_bound / upper_bound，要求区间有序。
// 堆：priority_queue<int> 大根堆；
//     priority_queue<int, vector<int>, greater<int>> 小根堆。
// 哈希表：unordered_map / unordered_set；需要稳定最坏复杂度时考虑 map / set。
// 位运算：x&1 判断奇偶；x&(x-1) 删除最低位 1；__builtin_popcount(unsigned x) 统计 1 的个数。
// 注意：对负数右移、移位位数达到类型宽度、signed 溢出等行为要谨慎。

// 本文件不提供 main()，复制到题目代码后自行编写 main。
