#include <bits/stdc++.h>
using namespace std;

using LL = long long;

void solve() {
    LL n, k;
    cin >> n >> k;

    // x：放 3 条小鱼干的盘子数量。
    // 为了让非空盘子数量 k - 2*x 最大，x 应尽可能小。
    LL x = 0;
    if (k > n) {
        x = (k - n + 1) / 2;  // ceil((k - n) / 2)
    }

    // x 不能超过所有小鱼干都按 3 条一盘时的数量。
    if (x > k / 3) {
        cout << -1 << '\n';
        return;
    }

    LL non_empty = k - 2 * x;
    cout << n - non_empty << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 若评测要求文件输入输出，取消下面两行注释：
    // freopen("crumb.in", "r", stdin);
    // freopen("crumb.out", "w", stdout);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}
