// 树状数组
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
