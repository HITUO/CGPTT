// 拓扑排序
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
