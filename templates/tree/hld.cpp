#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 1. 树链剖分 HLD：路径点权修改、路径区间求和
// 建树后调用 build(root)，再调用 path_query / point_set。
// ============================================================
struct HLD {
    int n, timer = 0;
    vector<vector<int>> g;
    vector<int> parent, depth, sz, heavy, top, dfn, rid;
    vector<ll> val, seg;

    HLD(int n) : n(n), g(n+1), parent(n+1), depth(n+1),
        sz(n+1), heavy(n+1), top(n+1), dfn(n+1), rid(n+1),
        val(n+1), seg(4*n+4) {}

    void add_edge(int u, int v) {
        g[u].push_back(v); g[v].push_back(u);
    }
    void set_value(int u, ll x) { val[u] = x; }

    void dfs1(int u, int p) {
        parent[u] = p;
        depth[u] = depth[p] + 1;
        sz[u] = 1;
        int best = 0;
        for (int v : g[u]) if (v != p) {
            dfs1(v, u);
            sz[u] += sz[v];
            if (sz[v] > best) best = sz[v], heavy[u] = v;
        }
    }
    void dfs2(int u, int tp) {
        top[u] = tp;
        dfn[u] = ++timer;
        rid[timer] = u;
        if (heavy[u]) dfs2(heavy[u], tp);
        for (int v : g[u])
            if (v != parent[u] && v != heavy[u]) dfs2(v, v);
    }
    void build_seg(int p, int l, int r, const vector<ll>& a) {
        if (l == r) { seg[p] = a[l]; return; }
        int m = (l+r)/2;
        build_seg(p*2,l,m,a); build_seg(p*2+1,m+1,r,a);
        seg[p] = seg[p*2] + seg[p*2+1];
    }
    void build(int root = 1) {
        dfs1(root, 0);
        dfs2(root, root);
        vector<ll> a(n+1);
        for (int i=1;i<=n;i++) a[dfn[i]] = val[i];
        build_seg(1,1,n,a);
    }
    void set_seg(int p,int l,int r,int x,ll v) {
        if(l==r){seg[p]=v;return;}
        int m=(l+r)/2;
        if(x<=m) set_seg(p*2,l,m,x,v);
        else set_seg(p*2+1,m+1,r,x,v);
        seg[p]=seg[p*2]+seg[p*2+1];
    }
    ll query_seg(int p,int l,int r,int ql,int qr) const {
        if(ql<=l && r<=qr) return seg[p];
        int m=(l+r)/2; ll ans=0;
        if(ql<=m) ans+=query_seg(p*2,l,m,ql,qr);
        if(qr>m) ans+=query_seg(p*2+1,m+1,r,ql,qr);
        return ans;
    }
    void point_set(int u,ll x) { set_seg(1,1,n,dfn[u],x); }
    ll path_query(int u,int v) const {
        ll ans=0;
        while(top[u]!=top[v]){
            if(depth[top[u]]<depth[top[v]]) swap(u,v);
            ans+=query_seg(1,1,n,dfn[top[u]],dfn[u]);
            u=parent[top[u]];
        }
        if(depth[u]>depth[v]) swap(u,v);
        ans+=query_seg(1,1,n,dfn[u],dfn[v]);
        return ans;
    }
};

// ============================================================
