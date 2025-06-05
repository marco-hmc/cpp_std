#include <iostream>
#include <vector>

// 1. [LeetCode Union-Find(并查集)
// 专题(一)](https://blog.csdn.net/guo15331092/article/details/78702686?tdsourcetag=s_pctim_aiomsg)
// - ***什么是并查集***

//   ​
//   看看这个[视频](https://www.bilibili.com/video/BV1Wv4y1f7n1?spm_id_from=333.337.search-card.all.click)你就有很清楚的了解了.简单来说并查集有3步,初始化,求并集,查询.

class UnionFind {
  private:
    std::vector<int> parent;
    std::vector<int> rank;

  public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }

    bool isConnected(int x, int y) { return find(x) == find(y); }
};
int main() {
    int n = 10;  // number of elements

    UnionFind uf(n);

    // Perform some union operations
    uf.unite(0, 1);
    uf.unite(2, 3);
    uf.unite(4, 5);
    uf.unite(6, 7);
    uf.unite(8, 9);

    // Check if elements are connected
    std::cout << "Is 0 connected to 1? "
              << (uf.isConnected(0, 1) ? "Yes" : "No") << '\n';
    std::cout << "Is 2 connected to 3? "
              << (uf.isConnected(2, 3) ? "Yes" : "No") << '\n';
    std::cout << "Is 4 connected to 5? "
              << (uf.isConnected(4, 5) ? "Yes" : "No") << '\n';
    std::cout << "Is 6 connected to 7? "
              << (uf.isConnected(6, 7) ? "Yes" : "No") << '\n';
    std::cout << "Is 8 connected to 9? "
              << (uf.isConnected(8, 9) ? "Yes" : "No") << '\n';
    std::cout << "Is 0 connected to 2? "
              << (uf.isConnected(0, 2) ? "Yes" : "No") << '\n';

    return 0;
}