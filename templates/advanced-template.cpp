/*
 * CGPTT - C++17 进阶算法模板
 *
 * 包含：
 * 树链剖分、可持久化线段树/主席树、Treap、Dinic、二分图匹配、
 * 点分治、莫队、AC 自动机、Manacher、后缀数组、高斯消元、矩阵快速幂。
 *
 * 约定：除字符串外，图和数组节点通常使用 1-based。
 * 每个模板独立使用；复制时检查命名冲突、数据范围与题目要求。
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll ADV_INF = (1LL << 60);

// ============================================================
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
// 2. 可持久化线段树 / 主席树：静态区间第 k 小
// 先离散化原数组；每个前缀建立一个版本。
// query(l,r,k) 返回离散化后的值，不是排名。
// ============================================================
struct PersistentKth {
    struct Node { int l=0, r=0, sum=0; };
    vector<Node> tr;
    vector<int> roots, values;
    int m = 0;

    PersistentKth() { tr.push_back(Node()); }

    int update(int old,int l,int r,int pos) {
        int now=(int)tr.size();
        tr.push_back(tr[old]);
        ++tr[now].sum;
        if(l<r){
            int mid=(l+r)/2;
            if(pos<=mid) tr[now].l=update(tr[old].l,l,mid,pos);
            else tr[now].r=update(tr[old].r,mid+1,r,pos);
        }
        return now;
    }
    void build(const vector<int>& a) { // a 使用 1-based
        int n=(int)a.size()-1;
        values.assign(a.begin()+1,a.end());
        sort(values.begin(),values.end());
        values.erase(unique(values.begin(),values.end()),values.end());
        m=(int)values.size();
        roots.assign(n+1,0);
        tr.clear(); tr.push_back(Node());
        for(int i=1;i<=n;i++){
            int p=lower_bound(values.begin(),values.end(),a[i])-values.begin()+1;
            roots[i]=update(roots[i-1],1,m,p);
        }
    }
    int kth_rank(int x,int y,int l,int r,int k) const {
        if(l==r) return l;
        int cnt=tr[tr[y].l].sum-tr[tr[x].l].sum;
        int mid=(l+r)/2;
        if(k<=cnt) return kth_rank(tr[x].l,tr[y].l,l,mid,k);
        return kth_rank(tr[x].r,tr[y].r,mid+1,r,k-cnt);
    }
    int query(int l,int r,int k) const {
        if(l<1 || r>=(int)roots.size() || l>r ||
           k<1 || k>r-l+1) throw out_of_range("invalid kth query");
        int rank=kth_rank(roots[l-1],roots[r],1,m,k);
        return values[rank-1];
    }
};

// ============================================================
// 3. 随机 Treap：支持 multiset 的插入、删除一个、排名、第 k 小、前驱后继
// 值域为 int；随机优先级。竞赛使用时注意随机种子与递归深度。
// ============================================================
struct TreapMultiset {
    struct Node { int key, pri, cnt, sz, l, r; };
    vector<Node> t{{0,0,0,0,0,0}};
    int root=0;
    mt19937 rng{(uint32_t)chrono::steady_clock::now().time_since_epoch().count()};

    int size(int p) const { return p ? t[p].sz : 0; }
    void pull(int p) { if(p) t[p].sz=size(t[p].l)+size(t[p].r)+t[p].cnt; }
    int make_node(int x) {
        t.push_back({x,(int)rng(),1,1,0,0});
        return (int)t.size()-1;
    }
    void split(int p,int key,int& a,int& b) { // a: <=key, b: >key
        if(!p){a=b=0;return;}
        if(t[p].key<=key){a=p;split(t[p].r,key,t[p].r,b);pull(a);}
        else{b=p;split(t[p].l,key,a,t[p].l);pull(b);}
    }
    int merge(int a,int b) {
        if(!a||!b) return a|b;
        if(t[a].pri>t[b].pri){t[a].r=merge(t[a].r,b);pull(a);return a;}
        t[b].l=merge(a,t[b].l);pull(b);return b;
    }
    void insert(int x) {
        int a,b,c;
        split(root,x,a,c);
        split(a,x-1,a,b);
        if(b) ++t[b].cnt, pull(b);
        else b=make_node(x);
        root=merge(merge(a,b),c);
    }
    void erase_one(int x) {
        int a,b,c;
        split(root,x,a,c);
        split(a,x-1,a,b);
        if(b){
            if(t[b].cnt>1) --t[b].cnt,pull(b);
            else b=merge(t[b].l,t[b].r);
        }
        root=merge(merge(a,b),c);
    }
    int rank(int x) { // x 的排名（比 x 小的元素个数 + 1）
        int p=root, ans=1;
        while(p){
            if(x<=t[p].key) p=t[p].l;
            else ans+=size(t[p].l)+t[p].cnt,p=t[p].r;
        }
        return ans;
    }
    int kth(int k) const { // k 从 1 开始；非法返回 INT_MIN
        if(k<1||k>size(root)) return INT_MIN;
        int p=root;
        while(p){
            int left=size(t[p].l);
            if(k<=left) p=t[p].l;
            else if(k<=left+t[p].cnt) return t[p].key;
            else k-=left+t[p].cnt,p=t[p].r;
        }
        return INT_MIN;
    }
    int predecessor(int x) const { // 严格小于 x 的最大值
        int p=root, ans=INT_MIN;
        while(p){if(t[p].key<x) ans=t[p].key,p=t[p].r; else p=t[p].l;}
        return ans;
    }
    int successor(int x) const { // 严格大于 x 的最小值
        int p=root, ans=INT_MAX;
        while(p){if(t[p].key>x) ans=t[p].key,p=t[p].l; else p=t[p].r;}
        return ans;
    }
};

// ============================================================
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
// 7. 莫队：静态区间询问离线排序框架
// 需要自行实现 add(value)、remove(value)、维护当前答案。
// ============================================================
struct MoQuery { int l,r,id; };
template<class Add,class Remove,class GetAnswer>
auto run_mo(int n, vector<MoQuery> queries, Add add, Remove remove,
            GetAnswer get_answer) {
    int block=max(1,(int)sqrt(n));
    sort(queries.begin(),queries.end(),[&](const MoQuery& a,const MoQuery& b){
        int x=a.l/block,y=b.l/block;
        if(x!=y) return x<y;
        return (x&1)?a.r>b.r:a.r<b.r;
    });
    using Answer=decltype(get_answer());
    vector<Answer> ans(queries.size());
    int L=1,R=0;
    for(auto q:queries){
        while(L>q.l) add(--L);
        while(R<q.r) add(++R);
        while(L<q.l) remove(L++);
        while(R>q.r) remove(R--);
        ans[q.id]=get_answer();
    }
    return ans;
}

// ============================================================
// 8. AC 自动机：多模式串匹配；小写字母字符集。
// build 后可扫描文本；每个节点 fail 指向最长真后缀对应节点。
// 本模板给出状态转移，不含每个模式串出现次数统计的专门优化。
// ============================================================
struct AhoCorasick {
    struct Node { int ch[26]{}; int fail=0; };
    vector<Node> t{Node()};
    void insert(const string& s){
        int p=0;
        for(char c:s){
            int x=c-'a';
            if(!t[p].ch[x]){t[p].ch[x]=(int)t.size();t.emplace_back();}
            p=t[p].ch[x];
        }
    }
    void build(){
        queue<int> q;
        for(int c=0;c<26;c++) if(t[0].ch[c]) q.push(t[0].ch[c]);
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int c=0;c<26;c++){
                int v=t[u].ch[c];
                if(v){t[v].fail=t[t[u].fail].ch[c];q.push(v);}
                else t[u].ch[c]=t[t[u].fail].ch[c];
            }
        }
    }
    int step(int state,char c) const {return t[state].ch[c-'a'];}
};

// ============================================================
// 9. Manacher：最长回文子串长度，O(n)，返回原串中的最长长度。
// ============================================================
int manacher_longest(const string& s){
    string t="^#";
    for(char c:s){t+=c;t+='#';}
    t+='$';
    vector<int> p(t.size());
    int center=0,right=0,best=0;
    for(int i=1;i+1<(int)t.size();i++){
        int mir=2*center-i;
        if(i<right) p[i]=min(right-i,p[mir]);
        while(t[i+1+p[i]]==t[i-1-p[i]]) ++p[i];
        if(i+p[i]>right) center=i,right=i+p[i];
        best=max(best,p[i]);
    }
    return best;
}

// ============================================================
// 10. 后缀数组：倍增排序 + 高度数组（Kasai），O(n log n)
// sa 为后缀起点 0-based；height[i]=LCP(sa[i],sa[i-1])，height[0]=0。
// ============================================================
struct SuffixArray {
    string s;
    vector<int> sa, rk, height;
    explicit SuffixArray(string str):s(move(str)){build();}
    void build(){
        int n=(int)s.size();
        sa.resize(n);rk.resize(n);height.assign(n,0);
        iota(sa.begin(),sa.end(),0);
        for(int i=0;i<n;i++) rk[i]=(unsigned char)s[i];
        vector<int> tmp(n);
        for(int k=1;k<n;k<<=1){
            auto cmp=[&](int a,int b){
                if(rk[a]!=rk[b]) return rk[a]<rk[b];
                int ra=a+k<n?rk[a+k]:-1;
                int rb=b+k<n?rk[b+k]:-1;
                return ra<rb;
            };
            sort(sa.begin(),sa.end(),cmp);
            if(n) tmp[sa[0]]=0;
            for(int i=1;i<n;i++) tmp[sa[i]]=tmp[sa[i-1]]+(cmp(sa[i-1],sa[i])?1:0);
            rk=tmp;
            if(n&&rk[sa.back()]==n-1) break;
        }
        vector<int> pos(n);
        for(int i=0;i<n;i++) pos[sa[i]]=i;
        int h=0;
        for(int i=0;i<n;i++){
            int r=pos[i];
            if(r==0) continue;
            int j=sa[r-1];
            while(i+h<n&&j+h<n&&s[i+h]==s[j+h]) ++h;
            height[r]=h;
            if(h) --h;
        }
        rk=move(pos);
    }
};

// ============================================================
// 11. 高斯消元：求 n 元实数线性方程组唯一解
// a 是 n 行 n+1 列增广矩阵；返回 false 表示无唯一解（无解或多解）。
// ============================================================
bool gaussian_elimination(vector<vector<double>>& a, vector<double>& x,
                          double eps=1e-9){
    int n=(int)a.size();
    for(int col=0,row=0;col<n&&row<n;col++){
        int sel=row;
        for(int i=row;i<n;i++)
            if(fabs(a[i][col])>fabs(a[sel][col])) sel=i;
        if(fabs(a[sel][col])<eps) continue;
        swap(a[sel],a[row]);
        double div=a[row][col];
        for(int j=col;j<=n;j++) a[row][j]/=div;
        for(int i=0;i<n;i++) if(i!=row){
            double factor=a[i][col];
            for(int j=col;j<=n;j++) a[i][j]-=factor*a[row][j];
        }
        ++row;
    }
    x.assign(n,0);
    for(int i=0;i<n;i++){
        int pivot=-1;
        for(int j=0;j<n;j++) if(fabs(a[i][j])>eps){pivot=j;break;}
        if(pivot==-1){
            if(fabs(a[i][n])>eps) return false;
        }else{
            if(pivot!=i) return false; // 这里仅接受唯一解的标准行阶梯结果
            x[pivot]=a[i][n];
        }
    }
    // 检查每个变量列均有主元
    vector<int> has(n,0);
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++) if(fabs(a[i][j])>eps){has[j]=1;break;}
    for(int v:has) if(!v) return false;
    return true;
}

// ============================================================
// 12. 矩阵快速幂：方阵乘法取模；用于线性递推等。
// ============================================================
using Matrix = vector<vector<ll>>;
Matrix matrix_multiply(const Matrix& a,const Matrix& b,ll mod){
    int n=(int)a.size();
    Matrix c(n,vector<ll>(n,0));
    for(int i=0;i<n;i++)
        for(int k=0;k<n;k++) if(a[i][k])
            for(int j=0;j<n;j++)
                c[i][j]=(c[i][j]+(__int128)a[i][k]*b[k][j])%mod;
    return c;
}
Matrix matrix_power(Matrix a,ll exp,ll mod){
    int n=(int)a.size();
    Matrix ans(n,vector<ll>(n,0));
    for(int i=0;i<n;i++) ans[i][i]=1%mod;
    while(exp){
        if(exp&1) ans=matrix_multiply(ans,a,mod);
        a=matrix_multiply(a,a,mod);
        exp>>=1;
    }
    return ans;
}

/*
 * 使用提醒：
 * - HLD 的 dfs 为递归实现，极深链可能爆栈；可改迭代版。
 * - 主席树适用于静态数组区间第 k 小；动态修改需要其他结构。
 * - Treap 的优先级随机，期望复杂度 O(log n)，不是严格最坏保证。
 * - Dinic 的性能取决于网络结构；边容量应非负。
 * - 点分治模板统计距离 <= K 的点对，要求树边权非负。
 * - 莫队是离线算法，查询必须提前读入；add/remove 要与维护量匹配。
 * - AC 自动机只适配小写 a-z；其他字符集需改字符映射。
 * - 高斯消元使用 double，浮点误差场景需按题目调整 eps。
 * - 后缀数组当前倍增排序实现为 O(n log^2 n)；需要更快可换基数排序。
 */
