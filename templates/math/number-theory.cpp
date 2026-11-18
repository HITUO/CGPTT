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

// 7. 数学与数论
// ============================================================

// 线性筛：返回 [2,n] 内所有素数。
vector<int> linear_sieve(int n) {
    vector<int> primes, lp(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        if (!lp[i]) lp[i] = i, primes.push_back(i);
        for (int p : primes) {
            if (p > lp[i] || 1LL * i * p > n) break;
            lp[i*p] = p;
        }
    }
    return primes;
}

// 扩展欧几里得：求 x,y 使 ax+by=gcd(a,b)。
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// 模逆元：仅当 gcd(a,mod)=1 时存在；mod 通常为正数。
ll mod_inverse(ll a, ll mod) {
    ll x, y;
    ll g = exgcd(a, mod, x, y);
    if (g != 1) return -1;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// 组合数预处理：要求 mod 为素数且 n < mod。
struct Combinations {
    ll mod;
    vector<ll> fac, ifac;
    Combinations(int n, ll mod) : mod(mod), fac(n+1), ifac(n+1) {
        fac[0] = 1;
        for (int i = 1; i <= n; ++i) fac[i] = fac[i-1] * i % mod;
        ifac[n] = mod_pow(fac[n], mod-2, mod);
        for (int i = n; i >= 1; --i) ifac[i-1] = ifac[i] * i % mod;
    }
    ll C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return fac[n] * ifac[k] % mod * ifac[n-k] % mod;
    }
};

// ============================================================
