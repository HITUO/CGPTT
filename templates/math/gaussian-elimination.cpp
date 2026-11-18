#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 11. 高斯消元：求 n 元实数线性方程组唯一解
// a 是 n 行 n+1 列增广矩阵；返回 false 表示无唯一解（无解或多解）。
// ============================================================
bool gaussian_elimination(vector<vector<double>>& a, vector<double>& x,
                          double eps=1e-9){
    int n=(int)a.size();
    for(int col=0,row=0;col<n&&row<n;col++){
        int sel=row;
        for(int i=row;i<n;i++)
            if(fabs(a[i][col])>fabs(a[sel][col])) sel=i;
        if(fabs(a[sel][col])<eps) continue;
        swap(a[sel],a[row]);
        double div=a[row][col];
        for(int j=col;j<=n;j++) a[row][j]/=div;
        for(int i=0;i<n;i++) if(i!=row){
            double factor=a[i][col];
            for(int j=col;j<=n;j++) a[i][j]-=factor*a[row][j];
        }
        ++row;
    }
    x.assign(n,0);
    for(int i=0;i<n;i++){
        int pivot=-1;
        for(int j=0;j<n;j++) if(fabs(a[i][j])>eps){pivot=j;break;}
        if(pivot==-1){
            if(fabs(a[i][n])>eps) return false;
        }else{
            if(pivot!=i) return false; // 这里仅接受唯一解的标准行阶梯结果
            x[pivot]=a[i][n];
        }
    }
    // 检查每个变量列均有主元
    vector<int> has(n,0);
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++) if(fabs(a[i][j])>eps){has[j]=1;break;}
    for(int v:has) if(!v) return false;
    return true;
}

// ============================================================
