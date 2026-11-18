#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 12. 矩阵快速幂：方阵乘法取模；用于线性递推等。
// ============================================================
using Matrix = vector<vector<ll>>;
Matrix matrix_multiply(const Matrix& a,const Matrix& b,ll mod){
    int n=(int)a.size();
    Matrix c(n,vector<ll>(n,0));
    for(int i=0;i<n;i++)
        for(int k=0;k<n;k++) if(a[i][k])
            for(int j=0;j<n;j++)
                c[i][j]=(c[i][j]+(__int128)a[i][k]*b[k][j])%mod;
    return c;
}
Matrix matrix_power(Matrix a,ll exp,ll mod){
    int n=(int)a.size();
    Matrix ans(n,vector<ll>(n,0));
    for(int i=0;i<n;i++) ans[i][i]=1%mod;
    while(exp){
        if(exp&1) ans=matrix_multiply(ans,a,mod);
        a=matrix_multiply(a,a,mod);
        exp>>=1;
    }
    return ans;
}

/*
 * 使用提醒：
 * - HLD 的 dfs 为递归实现，极深链可能爆栈；可改迭代版。
 * - 主席树适用于静态数组区间第 k 小；动态修改需要其他结构。
 * - Treap 的优先级随机，期望复杂度 O(log n)，不是严格最坏保证。
 * - Dinic 的性能取决于网络结构；边容量应非负。
 * - 点分治模板统计距离 <= K 的点对，要求树边权非负。
 * - 莫队是离线算法，查询必须提前读入；add/remove 要与维护量匹配。
 * - AC 自动机只适配小写 a-z；其他字符集需改字符映射。
 * - 高斯消元使用 double，浮点误差场景需按题目调整 eps。
 * - 后缀数组当前倍增排序实现为 O(n log^2 n)；需要更快可换基数排序。
 */
