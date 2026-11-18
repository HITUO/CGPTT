// 差分数组
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

void range_add(vector<ll>& d,int l,int r,ll v){d[l]+=v;d[r+1]-=v;}
vector<ll> restore(const vector<ll>& d,int n){vector<ll>a(n+1);for(int i=1;i<=n;i++)a[i]=a[i-1]+d[i];return a;}
