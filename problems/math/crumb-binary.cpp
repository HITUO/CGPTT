#include <bits/stdc++.h>
using namespace std;

using LL = long long;

void solve() {
    LL n, k;
    cin >> n >> k;

    // x 的合法上界：放 3 条的小鱼干盘子不能超过 k / 3。
    LL l = 0, r = k / 3;

    // 若最大的 x 仍不能让非空盘子数不超过 n，则无解。
    if (k - 2 * r > n) {
        cout << -1 << '\n';
        return;
    }

    // 二分第一个满足 k - 2*x <= n 的 x。
    while (l < r) {
        LL mid = l + (r - l) / 2;

        if (k - 2 * mid <= n) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    LL x = l;
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
