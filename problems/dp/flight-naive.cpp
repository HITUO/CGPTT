#include <bits/stdc++.h>
using namespace std;

using LL = long long;
const int INF = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 若评测要求文件输入输出，取消注释：
    // freopen("flight.in", "r", stdin);
    // freopen("flight.out", "w", stdout);

    int n;
    cin >> n;

    vector<int> d(n + 1);
    for (int i = 1; i <= n; ++i) cin >> d[i];

    int q;
    cin >> q;

    while (q--) {
        int k;
        cin >> k;

        vector<int> dp(n + 1, INF);
        dp[1] = 0;

        for (int i = 2; i <= n; ++i) {
            int L = max(1, i - k);
            for (int j = L; j < i; ++j) {
                int cost = (d[i] >= d[j]);
                dp[i] = min(dp[i], dp[j] + cost);
            }
        }

        cout << dp[n] << '\n';
    }

    return 0;
}
