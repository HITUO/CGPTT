// 双指针滑动窗口
// C++17；按题目数据范围调整类型、下标与常量。
// 示例依赖元素非负；使用双指针前要证明窗口单调性。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

// 示例：非负数组中，统计和 <= limit 的子数组数。
ll count_subarrays(const vector<int>& a,ll limit){int l=0;ll sum=0,ans=0;for(int r=0;r<(int)a.size();r++){sum+=a[r];while(l<=r&&sum>limit)sum-=a[l++];ans+=r-l+1;}return ans;}
