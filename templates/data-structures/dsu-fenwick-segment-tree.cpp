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
