#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
