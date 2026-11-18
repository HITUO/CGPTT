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

// 1. 二分、双指针、前缀和与差分
// ============================================================

// 整数二分：找第一个满足 check(x) 的 x。
// 前提：区间内 check 具有 false...false,true...true 的单调性。
template<class Check>
ll first_true(ll l, ll r, Check check) {
    while (l < r) {
        ll mid = l + (r - l) / 2;
        if (check(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}

// 找最后一个满足 check(x) 的 x（true...true,false...false）。
template<class Check>
ll last_true(ll l, ll r, Check check) {
    while (l < r) {
        ll mid = l + (r - l + 1) / 2;
        if (check(mid)) l = mid;
        else r = mid - 1;
    }
    return l;
}

// 一维前缀和：预处理后区间 [l,r] 的和为 pre[r]-pre[l-1]。
// vector<ll> pre(n+1); for (int i=1;i<=n;i++) pre[i]=pre[i-1]+a[i];

// 二维前缀和：矩阵下标 1..n, 1..m。
// sum(x1,y1,x2,y2) = s[x2][y2]-s[x1-1][y2]-s[x2][y1-1]+s[x1-1][y1-1]。

// 一维差分：区间 [l,r] 加 v：d[l]+=v, d[r+1]-=v；最后做前缀和还原。

// 双指针常用框架：维护窗口 [l,r]，按题意扩展右端并收缩左端。
// int l=0; for (int r=0;r<n;r++) { add(a[r]); while (窗口不合法) remove(a[l++]); 更新答案; }

// ============================================================
