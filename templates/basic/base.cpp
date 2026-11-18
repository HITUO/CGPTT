#include <bits/stdc++.h>
using namespace std;

// 0. 基础配置
// ============================================================

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

constexpr int INF = 0x3f3f3f3f;
constexpr ll LINF = (1LL << 62);

// 快速幂：计算 a^b % mod，要求 mod > 0、b >= 0。
ll mod_pow(ll a, ll b, ll mod) {
    ll ans = 1 % mod;
    a %= mod;
    while (b) {
        if (b & 1) ans = (__int128)ans * a % mod;
        a = (__int128)a * a % mod;
        b >>= 1;
    }
    return ans;
}

// gcd / lcm（注意 lcm 乘法可能溢出）。
ll gcd_ll(ll a, ll b) { return b ? gcd_ll(b, a % b) : abs(a); }
ll lcm_ll(ll a, ll b) { return a / gcd_ll(a, b) * b; }

// 坐标离散化：返回 x 在排序去重数组中的 1-based 排名。
template<class T>
int get_rank(const vector<T>& sorted_unique, const T& x) {
    return int(lower_bound(sorted_unique.begin(), sorted_unique.end(), x)
               - sorted_unique.begin()) + 1;
}

// ============================================================
