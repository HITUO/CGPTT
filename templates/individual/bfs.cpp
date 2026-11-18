// BFS 无权图最短路
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
