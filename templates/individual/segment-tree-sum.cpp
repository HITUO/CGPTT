// 线段树：单点赋值、区间求和
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
