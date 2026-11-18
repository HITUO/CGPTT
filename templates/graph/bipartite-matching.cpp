#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 5. 二分图最大匹配：匈牙利 DFS 增广路，左侧 1..n，右侧 1..m。
// 适合中小规模；复杂图可换 Hopcroft-Karp。
// ============================================================
struct BipartiteMatching {
    int n,m;
    vector<vector<int>> g;
    vector<int> matchR,vis;
    int stamp=0;
    BipartiteMatching(int n,int m):n(n),m(m),g(n+1),matchR(m+1),vis(m+1){}
    void add_edge(int u,int v){g[u].push_back(v);}
    bool dfs(int u){
        for(int v:g[u]) if(vis[v]!=stamp){
            vis[v]=stamp;
            if(!matchR[v]||dfs(matchR[v])){matchR[v]=u;return true;}
        }
        return false;
    }
    int max_matching(){
        int ans=0;
        for(int u=1;u<=n;u++){++stamp;if(dfs(u))++ans;}
        return ans;
    }
};

// ============================================================
