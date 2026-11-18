#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll ADV_INF = (1LL << 60);

// 4. Dinic 最大流：有向网络，容量非负；max_flow(s,t)。
// ============================================================
struct Dinic {
    struct Edge { int to, rev; ll cap; };
    int n;
    vector<vector<Edge>> g;
    vector<int> level, cur;
    Dinic(int n):n(n),g(n+1),level(n+1),cur(n+1){}
    void add_edge(int u,int v,ll cap) {
        Edge a{v,(int)g[v].size(),cap};
        Edge b{u,(int)g[u].size(),0};
        g[u].push_back(a); g[v].push_back(b);
    }
    bool bfs(int s,int t) {
        fill(level.begin(),level.end(),-1);
        queue<int> q; q.push(s); level[s]=0;
        while(!q.empty()){
            int u=q.front();q.pop();
            for(auto &e:g[u]) if(e.cap>0&&level[e.to]==-1){
                level[e.to]=level[u]+1;q.push(e.to);
            }
        }
        return level[t]!=-1;
    }
    ll dfs(int u,int t,ll f) {
        if(u==t||f==0) return f;
        for(int &i=cur[u];i<(int)g[u].size();++i){
            Edge &e=g[u][i];
            if(e.cap>0&&level[e.to]==level[u]+1){
                ll got=dfs(e.to,t,min(f,e.cap));
                if(got){e.cap-=got;g[e.to][e.rev].cap+=got;return got;}
            }
        }
        return 0;
    }
    ll max_flow(int s,int t) {
        ll flow=0;
        while(bfs(s,t)){
            fill(cur.begin(),cur.end(),0);
            while(ll f=dfs(s,t,ADV_INF)) flow+=f;
        }
        return flow;
    }
};

// ============================================================
