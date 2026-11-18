// Bellman-Ford
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
