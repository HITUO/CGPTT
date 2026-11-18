// Floyd 多源最短路
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

void floyd_warshall(vector<vector<ll>>& d) {
    int n = (int)d.size() - 1;
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                if (d[i][k] != LINF && d[k][j] != LINF)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}
