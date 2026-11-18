#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct SegmentTree {
    int n;
    vector<int> tr;

    SegmentTree(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tr.assign(4 * n + 4, INF);
    }

    void update(int p, int l, int r, int x, int v) {
        if (l == r) {
            tr[p] = v;
            return;
        }
        int mid = (l + r) >> 1;
        if (x <= mid) update(p << 1, l, mid, x, v);
        else update(p << 1 | 1, mid + 1, r, x, v);
        tr[p] = min(tr[p << 1], tr[p << 1 | 1]);
    }

    void update(int x, int v) {
        update(1, 1, n, x, v);
    }

    int query(int p, int l, int r, int ql, int qr) const {
        if (ql > r || qr < l || ql > qr) return INF;
        if (ql <= l && r <= qr) return tr[p];
        int mid = (l + r) >> 1;
        return min(query(p << 1, l, mid, ql, qr),
                   query(p << 1 | 1, mid + 1, r, ql, qr));
    }

    int query(int l, int r) const {
        if (l > r) return INF;
        return query(1, 1, n, l, r);
    }
};

struct Node {
    int index;
    int value;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 若评测要求文件输入输出，取消注释：
    // freopen("flight.in", "r", stdin);
    // freopen("flight.out", "w", stdout);

    int n;
    cin >> n;

    vector<int> d(n + 1);
    vector<int> values;
    values.reserve(n);

    for (int i = 1; i <= n; ++i) {
        cin >> d[i];
        values.push_back(d[i]);
    }

    // 高度坐标压缩
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());

    int m = (int)values.size();
    vector<int> rankOf(n + 1);

    for (int i = 1; i <= n; ++i) {
        rankOf[i] = int(lower_bound(values.begin(), values.end(), d[i])
                        - values.begin()) + 1;
    }

    int q;
    cin >> q;

    while (q--) {
        int k;
        cin >> k;

        SegmentTree seg(m);
        vector<deque<Node>> bucket(m + 1);
        vector<int> dp(n + 1, INF);

        dp[1] = 0;
        int expired = 1;

        // 插入一个已经算出 dp 的位置。
        // 同一高度的队列按下标递增、dp 值严格递增维护。
        auto add = [&](int idx) {
            int r = rankOf[idx];
            auto &dq = bucket[r];

            while (!dq.empty() && dq.back().value >= dp[idx]) {
                dq.pop_back();
            }

            int oldMin = dq.empty() ? INF : dq.front().value;
            dq.push_back({idx, dp[idx]});

            int newMin = dq.front().value;
            if (newMin != oldMin) {
                seg.update(r, newMin);
            }
        };

        // 删除所有下标小于 L 的过期位置。
        auto removeExpired = [&](int L) {
            while (expired < L) {
                int r = rankOf[expired];
                auto &dq = bucket[r];

                int oldMin = dq.empty() ? INF : dq.front().value;

                // 若该候选仍在队首，它现在过期；若已被支配弹出，则无需处理。
                if (!dq.empty() && dq.front().index == expired) {
                    dq.pop_front();
                }

                int newMin = dq.empty() ? INF : dq.front().value;
                if (newMin != oldMin) {
                    seg.update(r, newMin);
                }

                ++expired;
            }
        };

        for (int i = 2; i <= n; ++i) {
            int L = max(1, i - k);

            // 窗口调整为 [L, i-1]
            removeExpired(L);
            add(i - 1);

            int r = rankOf[i];

            // d[j] <= d[i]：本次疲劳 +1
            int bestLE = seg.query(1, r);

            // d[j] > d[i]：本次疲劳 +0
            int bestGT = seg.query(r + 1, m);

            dp[i] = min(bestLE == INF ? INF : bestLE + 1, bestGT);
        }

        cout << dp[n] << '\n';
    }

    return 0;
}
