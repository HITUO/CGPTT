# C++ 模板库（C++17）

本目录将常用与进阶算法按主题拆分。每个源文件都可以单独打开、复制到题目代码中；请先阅读文件顶部的适用条件、下标约定和依赖说明。

## 分类目录

| 目录 | 内容 |
|---|---|
| [basic](basic/) | 基础配置、二分、前缀和、差分、STL |
| [data-structures](data-structures/) | 并查集、树状数组、线段树、单调结构、主席树、Treap、莫队 |
| [graph](graph/) | 图遍历、最短路、生成树、连通性、网络流、二分图匹配 |
| [tree](tree/) | LCA、树链剖分、点分治 |
| [dp](dp/) | 常见动态规划状态转移片段 |
| [string](string/) | KMP、Z、Trie、哈希、AC 自动机、Manacher、后缀数组 |
| [math](math/) | 数论、高斯消元、矩阵快速幂 |
| [individual](individual/) | 原有独立模板，继续保留 |

## 常用模板入口

- 基础：[基础配置](basic/base.cpp) · [二分/前缀和/差分](basic/binary-prefix-difference.cpp) · [STL](basic/stl-cheatsheet.cpp)
- 数据结构：[单调结构与 ST 表](data-structures/monotonic-stack-queue-sparse-table.cpp) · [并查集/树状数组/线段树](data-structures/dsu-fenwick-segment-tree.cpp) · [主席树](data-structures/persistent-segment-tree-kth.cpp) · [Treap](data-structures/treap-multiset.cpp) · [莫队](data-structures/mo-algorithm.cpp)
- 图论：[常用图算法](graph/graph-algorithms.cpp) · [Dinic](graph/dinic-max-flow.cpp) · [二分图匹配](graph/bipartite-matching.cpp)
- 树：[LCA](tree/lca-binary-lifting.cpp) · [树链剖分](tree/hld.cpp) · [点分治](tree/centroid-decomposition-pairs.cpp)
- 字符串：[KMP/Z/Trie/哈希](string/kmp-z-trie-hash.cpp) · [AC 自动机](string/aho-corasick.cpp) · [Manacher](string/manacher.cpp) · [后缀数组](string/suffix-array.cpp)
- 数学：[数论](math/number-theory.cpp) · [高斯消元](math/gaussian-elimination.cpp) · [矩阵快速幂](math/matrix-fast-power.cpp)
- DP：[常见 DP 片段](dp/common-dp-snippets.cpp)

## 使用与编译

```bash
g++ -std=c++17 -O2 -pipe main.cpp -o main
./main
```

- 每个模板文件不一定包含 `main()`，复制到题目程序后自行编写。
- 同一文件中可能包含一组紧密相关的算法；不同文件可独立选用。
- 多数图和数组下标为 1-based，字符串下标通常为 0-based。
- 使用前检查数据范围、边权限制、取模条件和递归深度。
- 旧版 [C++17 综合模板](cpp17-template.cpp) 与 [进阶模板合集](advanced-template.cpp) 保留作备份；原有 [individual](individual/) 文件也不删除。

## 推荐学习顺序

基础与 STL → 二分/前缀和/差分 → 单调结构 → 并查集/树状数组/线段树 → BFS/DFS/最短路/MST → DP → LCA → 字符串与数论 → 网络流、主席树、树链剖分等进阶内容。
