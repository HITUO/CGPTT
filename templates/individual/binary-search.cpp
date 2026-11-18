// 整数二分
// C++17；按题目数据范围调整类型、下标与常量。
// check 函数必须满足对应单调性。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;

template<class Check> ll first_true(ll l,ll r,Check check){while(l<r){ll m=l+(r-l)/2;if(check(m))r=m;else l=m+1;}return l;}
template<class Check> ll last_true(ll l,ll r,Check check){while(l<r){ll m=l+(r-l+1)/2;if(check(m))l=m;else r=m-1;}return l;}
