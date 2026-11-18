// 单调队列：滑动窗口最小值
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

vector<int> sliding_min(const vector<int>& a,int k){vector<int> ans;deque<int> q;for(int i=0;i<(int)a.size();i++){while(!q.empty()&&q.front()<=i-k)q.pop_front();while(!q.empty()&&a[q.back()]>=a[i])q.pop_back();q.push_back(i);if(i>=k-1)ans.push_back(a[q.front()]);}return ans;}
