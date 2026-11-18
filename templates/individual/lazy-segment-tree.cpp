// 懒标记线段树：区间加、区间求和
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
