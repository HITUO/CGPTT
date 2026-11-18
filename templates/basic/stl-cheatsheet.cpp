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

// 9. 常用 STL 提醒
// ============================================================

// 排序：sort(a.begin(), a.end());
// 去重（先排序）：a.erase(unique(a.begin(), a.end()), a.end());
// 二分：lower_bound / upper_bound，要求区间有序。
// 堆：priority_queue<int> 大根堆；
//     priority_queue<int, vector<int>, greater<int>> 小根堆。
// 哈希表：unordered_map / unordered_set；需要稳定最坏复杂度时考虑 map / set。
// 位运算：x&1 判断奇偶；x&(x-1) 删除最低位 1；__builtin_popcount(unsigned x) 统计 1 的个数。
// 注意：对负数右移、移位位数达到类型宽度、signed 溢出等行为要谨慎。

// 本文件不提供 main()，复制到题目代码后自行编写 main。
