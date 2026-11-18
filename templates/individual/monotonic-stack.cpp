// 单调栈：下一个更大元素
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

vector<int> next_greater(const vector<int>& a){int n=a.size();vector<int> ans(n,-1),st;for(int i=0;i<n;i++){while(!st.empty()&&a[st.back()]<a[i])ans[st.back()]=i,st.pop_back();st.push_back(i);}return ans;}
