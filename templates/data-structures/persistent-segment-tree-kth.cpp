#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
