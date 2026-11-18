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

// 2. 单调栈、单调队列、ST 表
// ============================================================

// 下一个更大元素（严格大于）：ans[i] 是右侧第一个 > a[i] 的下标；不存在为 -1。
vector<int> next_greater(const vector<int>& a) {
    int n = (int)a.size();
    vector<int> ans(n, -1), st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] < a[i]) {
            ans[st.back()] = i;
            st.pop_back();
        }
        st.push_back(i);
    }
    return ans;
}

// 滑动窗口最小值：窗口长度 k，返回每个窗口的最小值。
vector<int> sliding_min(const vector<int>& a, int k) {
    int n = (int)a.size();
    vector<int> ans;
    deque<int> q;
    for (int i = 0; i < n; ++i) {
        while (!q.empty() && q.front() <= i - k) q.pop_front();
        while (!q.empty() && a[q.back()] >= a[i]) q.pop_back();
        q.push_back(i);
        if (i >= k - 1) ans.push_back(a[q.front()]);
    }
    return ans;
}

// ST 表：静态区间最小值 O(1) 查询，O(n log n) 预处理。
// 不支持修改；查询区间必须非空。
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

// ============================================================
