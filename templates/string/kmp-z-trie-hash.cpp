#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
using vi = vector<int>;
using vll = vector<ll>;
constexpr int INF = 0x3f3f3f3f;
constexpr ll LINF = (1LL << 62);

// 6. 字符串：KMP、Z 函数、Trie、滚动哈希
// ============================================================

// KMP 前缀函数 pi[i]：字符串 s[0..i] 的最长相等真前后缀长度。
vector<int> prefix_function(const string& s) {
    int n = (int)s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

// KMP 匹配：返回模式串 pat 在文本 text 中所有匹配起点（0-based）。
vector<int> kmp_match(const string& text, const string& pat) {
    vector<int> ans;
    if (pat.empty()) return ans;
    auto pi = prefix_function(pat);
    for (int i = 0, j = 0; i < (int)text.size(); ++i) {
        while (j > 0 && text[i] != pat[j]) j = pi[j-1];
        if (text[i] == pat[j]) ++j;
        if (j == (int)pat.size()) {
            ans.push_back(i - j + 1);
            j = pi[j-1];
        }
    }
    return ans;
}

// Z 函数：z[i] 表示 s[i..] 与 s[0..] 的最长公共前缀长度；z[0]=n。
vector<int> z_function(const string& s) {
    int n = (int)s.size();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i-l]);
        while (i + z[i] < n && s[z[i]] == s[i+z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    if (n) z[0] = n;
    return z;
}

// 小写英文字母 Trie：插入、查询完整单词。
struct Trie {
    struct Node {
        int ch[26]{};
        bool terminal = false;
    };
    vector<Node> t = vector<Node>(1);
    void insert(const string& s) {
        int p = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!t[p].ch[x]) {
                t[p].ch[x] = (int)t.size();
                t.emplace_back();
            }
            p = t[p].ch[x];
        }
        t[p].terminal = true;
    }
    bool contains(const string& s) const {
        int p = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!t[p].ch[x]) return false;
            p = t[p].ch[x];
        }
        return t[p].terminal;
    }
};

// 双模滚动哈希：用于快速比较子串哈希；哈希存在极低概率碰撞。
// 下标 0-based，get(l,r) 查询闭区间 [l,r]。
struct DoubleHash {
    static constexpr ll mod1 = 1000000007LL, mod2 = 1000000009LL;
    static constexpr ll base = 911382323LL;
    vector<ll> h1, h2, p1, p2;
    DoubleHash(const string& s) {
        int n = (int)s.size();
        h1.assign(n+1, 0); h2.assign(n+1, 0);
        p1.assign(n+1, 1); p2.assign(n+1, 1);
        for (int i = 0; i < n; ++i) {
            int x = (unsigned char)s[i] + 1;
            h1[i+1] = (h1[i] * base + x) % mod1;
            h2[i+1] = (h2[i] * base + x) % mod2;
            p1[i+1] = p1[i] * base % mod1;
            p2[i+1] = p2[i] * base % mod2;
        }
    }
    pair<ll,ll> get(int l, int r) const {
        ll x = (h1[r+1] - h1[l] * p1[r-l+1] % mod1 + mod1) % mod1;
        ll y = (h2[r+1] - h2[l] * p2[r-l+1] % mod2 + mod2) % mod2;
        return {x, y};
    }
};

// ============================================================
