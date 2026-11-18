// Kruskal 最小生成树
// C++17；按题目数据范围调整类型、下标与常量。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

struct DSU{vector<int>p,s;DSU(int n):p(n+1),s(n+1,1){iota(p.begin(),p.end(),0);}int find(int x){return p[x]==x?x:p[x]=find(p[x]);}bool unite(int a,int b){a=find(a);b=find(b);if(a==b)return false;if(s[a]<s[b])swap(a,b);p[b]=a;s[a]+=s[b];return true;}};
struct Edge{int u,v;ll w;};
ll kruskal(int n,vector<Edge> e,bool& connected){sort(e.begin(),e.end(),[](auto&a,auto&b){return a.w<b.w;});DSU d(n);ll ans=0;int cnt=0;for(auto x:e)if(d.unite(x.u,x.v)){ans+=x.w;if(++cnt==n-1)break;}connected=n<=1||cnt==n-1;return ans;}
