// 一维、二维前缀和
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

vector<ll> prefix1d(const vector<ll>& a){int n=a.size()-1;vector<ll> p(n+1);for(int i=1;i<=n;i++)p[i]=p[i-1]+a[i];return p;}
vector<vector<ll>> prefix2d(const vector<vector<ll>>& a){int n=a.size()-1,m=a[0].size()-1;vector<vector<ll>> s(n+1,vector<ll>(m+1));for(int i=1;i<=n;i++)for(int j=1;j<=m;j++)s[i][j]=a[i][j]+s[i-1][j]+s[i][j-1]-s[i-1][j-1];return s;}
ll rect_sum(const vector<vector<ll>>& s,int x1,int y1,int x2,int y2){return s[x2][y2]-s[x1-1][y2]-s[x2][y1-1]+s[x1-1][y1-1];}
