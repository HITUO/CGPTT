// ST 表
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

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
