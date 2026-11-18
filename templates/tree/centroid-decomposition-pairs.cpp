#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 6. 点分治：统计带权树中距离 <= K 的无序点对数量
// 边权要求非负；O(n log^2 n) 常见实现。
// ============================================================
struct CentroidPairs {
    int n;
    ll K, answer=0;
    vector<vector<pair<int,ll>>> g;
    vector<int> sz;
    vector<char> removed;
    CentroidPairs(int n,ll K):n(n),K(K),g(n+1),sz(n+1),removed(n+1){}
    void add_edge(int u,int v,ll w){g[u].push_back({v,w});g[v].push_back({u,w});}
    void calc_size(int u,int p){
        sz[u]=1;
        for(auto [v,w]:g[u]) if(v!=p&&!removed[v]){
            calc_size(v,u);sz[u]+=sz[v];
        }
    }
    int get_centroid(int u,int p,int total){
        for(auto [v,w]:g[u]) if(v!=p&&!removed[v])
            if(sz[v]>total/2) return get_centroid(v,u,total);
        return u;
    }
    void collect(int u,int p,ll d,vector<ll>& ds){
        if(d>K) return;
        ds.push_back(d);
        for(auto [v,w]:g[u]) if(v!=p&&!removed[v])
            collect(v,u,d+w,ds);
    }
    ll count_pairs(vector<ll>& a){
        sort(a.begin(),a.end());
        ll res=0;
        int l=0,r=(int)a.size()-1;
        while(l<r){
            if(a[l]+a[r]<=K) res+=r-l,++l;
            else --r;
        }
        return res;
    }
    void decompose(int entry){
        calc_size(entry,0);
        int c=get_centroid(entry,0,sz[entry]);
        removed[c]=1;
        vector<ll> all{0};
        for(auto [v,w]:g[c]) if(!removed[v]){
            vector<ll> part;
            collect(v,c,w,part);
            answer-=count_pairs(part);
            all.insert(all.end(),part.begin(),part.end());
        }
        answer+=count_pairs(all);
        for(auto [v,w]:g[c]) if(!removed[v]) decompose(v);
    }
    ll solve(int root=1){
        answer=0;
        decompose(root);
        return answer;
    }
};

// ============================================================
