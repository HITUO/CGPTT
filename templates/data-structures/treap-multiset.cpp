#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
