// Prim 最小生成树
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
