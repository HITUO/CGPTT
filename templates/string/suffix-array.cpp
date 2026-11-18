#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
