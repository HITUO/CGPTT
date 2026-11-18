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

// 8. 动态规划常见状态转移提示（按题目改写）
// ============================================================

// 01 背包（每件物品最多选一次）：
// for (int i=1;i<=n;i++)
//   for (int j=V;j>=w[i];j--)
//     dp[j]=max(dp[j],dp[j-w[i]]+val[i]);
//
// 完全背包（每件物品可选无限次）：
// for (int i=1;i<=n;i++)
//   for (int j=w[i];j<=V;j++)
//     dp[j]=max(dp[j],dp[j-w[i]]+val[i]);
//
// 最长上升子序列 O(n log n)：
// vector<int> tail;
// for (int x:a) {
//   auto it=lower_bound(tail.begin(),tail.end(),x);
//   if (it==tail.end()) tail.push_back(x); else *it=x;
// }
// 严格递增使用 lower_bound；最长不下降子序列通常使用 upper_bound。

// ============================================================
