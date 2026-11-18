#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
