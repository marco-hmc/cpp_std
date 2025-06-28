
# [并查集](https://zhuanlan.zhihu.com/p/93647900)

## 朴素版本实现

```c++
class UnionFind {
public:
    UnionFind(int n) : parent(std::move(vector<int>(n))) {
        // 将每个节点的父节点设为自身
        for(int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    // 递归寻找某个节点的根节点(代表节点)
    int find(int node) {
        if(node < 0 || node > parent.size()) {
            return -1;
        }
        // 父节点等于自身，为根节点
        if(parent[node] == node) {
            return node;
        }
        // 往上寻找根节点
        return find(parent[node]);
    }

    // 合并两个节点，找到他们各自的根节点，然后令一个根节点为另一个根节点的父节点
    void merge(int node1, int node2) {
        int root1 = find(node1);
        int root2 = find(node2);
        // 令root1为root2的父节点
        parent[root2] = root1;
    }
private:
    vector<int> parent;
};
```

- 存在问题：可能会形成一条长长的**链**，随着链越来越长，想要从底部找到根节点会变得越来越难

## 路径压缩实现

- 既然我们只关心一个元素对应的**根节点**，那我们希望**每个元素到根节点的路径尽可能短，最好只需要一步**，形成以下效果

  <img src="imgs/algorithm/unionfind1.jpg" alt="unionfind1" style="zoom:50%;" />

- 只要我们在查询的过程中，**把沿途的每个节点的父节点都设为根节点**即可

  ```c++
  int find(int node) {
  	if(node == parent[node]) {
  		return node;
  	}
      parent[node] = find(parent[node]); // 将当前节点的父节点设为根节点
      return parent[node];	// 返回父节点(根节点)
  }
  ```

- 存在问题：有些人可能有一个误解，以为路径压缩优化后，并查集始终都是一个**菊花图**（只有两层的树的俗称）。但其实，由于路径压缩只在查询时进行，也**只压缩一条路径**，所以并查集最终的结构仍然可能是比较复杂的

## 按秩合并实现

- 增加一个统计节点秩的数组 rank[]记录**每个根节点对应的树的深度**（如果不是根节点，其 rank 相当于以它作为根节点的**子树**的深度）

- 一开始，把所有元素的 rank（**秩**）设为 1。合并时比较两个根节点，**把 rank 较小者往较大者上合并**。**如果两个根节点的深度相同，新的根节点深度+1**

- 路径压缩和按秩合并如果一起使用，时间复杂度接近$\textbf{O}(n)$，但可**能会破坏 rank 的准确性**

  ```c++
  class UnionFind {
  public:
      UnionFind(int n) : parent(std::move(vector<int>(n))),
      rank(std::move(vector<int>(n))) {
          // 将每个节点的父节点设为自身
          for(int i = 0; i < n; ++i) {
              parent[i] = i;
              rank[i] = 1;
          }
      }

      // 递归寻找某个节点的根节点(代表节点)
      int find(int node) {
          // 父节点等于自身，为根节点
          if(parent[node] == node) {
              return node;
          }
          // 往上寻找根节点
          return find(parent[node]);
      }

      // 合并两个节点，找到他们各自的根节点，然后令rank大的根节点成为rank小的根节点的父节点
      // 如果两个根节点的rank相同，新的根节点rank+1
      void merge(int node1, int node2) {
          int root1 = find(node1);
          int root2 = find(node2);
          if(root1 != root2){
              if(rank[root1] < rank[root2]) {
                  swap(root1, root2) // rank大的根节点成为rank小的根节点的父节点
              }
  			parent[root1] = root2;
              if(rank[root1] == rank[root2]) {
                  ++rank[root1];	// root1的rank+1
              }
          }
      }
  private:
      vector<int> parent;
      vector<int> rank;
  };
  ```

## [账户合并](https://leetcode-cn.com/problems/accounts-merge/)

```c++
/*
    思路：
    根据题意可知：
    	1. 存在相同邮箱的账号一定属于同一个人
    	2. 名称相同的账户不一定属于同一个人

    由于名称相同无法判断为同1人，所以只能使用邮箱是否相同来判断是否为同一人。
    这样建立并查集就比较简单了：
    	1. 先初始化每个账户为1个连通分量
    	2. 遍历每个账户下的邮箱，判断该邮箱是否在其他账户下出现
    	3. 如果未出现，继续
    	4. 如果账户A、B下出现了相同的邮箱email，那么将账户A和账户B两个连通分量进行合并
    	5. 最后遍历并查集中每个连通分量，将所有连通分量内部账户的邮箱全部合并(相同的去重，不同的合并)
    	6. 结束
    */
class UnionFind {
    public:
    vector<int> parent;  // 记录节点的根
    vector<int> rank;  // 记录根节点的深度（用于优化）
    UnionFind(int n): parent(vector<int>(n)), rank(vector<int>(n)) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        // 压缩方式：直接指向根节点
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    // 按秩合并
    void merge(int x, int y) {
        int rootx = find(x);
        int rooty = find(y);
        if (rootx != rooty) {
            if (rank[rootx] < rank[rooty]) {
                swap(rootx, rooty);
            }
            parent[rooty] = rootx;
            if (rank[rootx] == rank[rooty])
                rank[rootx] += 1;
        }
    }
};

class Solution {
    public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        vector<vector<string> > res;

        // 作用：存储每个邮箱属于哪个账户，同时在遍历邮箱时，判断邮箱是否出现过
        // 格式：<邮箱，账户下标>
        unordered_map<string, int> mailToIdx;
        int n = accounts.size();

        UnionFind uf(n);
        for (int i = 0; i < n; i++) {
            int m = accounts[i].size();
            for (int j = 1; j < m; j++) {
                string mail = accounts[i][j];
                if (mailToIdx.find(mail) == mailToIdx.end()) {
                    mailToIdx[mail] = i;	// 记录邮箱首次出现的idx
                } else {
                    uf.merge(i, mailToIdx[mail]);	// 邮箱之前出现过，合并连通分量
                }
            }
        }

        // 作用：存储每个账户下的邮箱
        // 格式：<账户id, 邮箱列表>
        // 注意：这里的key必须是账户id，不能是账户名称，名称可能相同，会造成覆盖
        unordered_map<int, vector<string> > idxToMails;
        // 遍历所有连通分量，将同一连通分量(同一账户)内的邮箱合并
        for (auto& [mail, idx] : mailToIdx) {
            idxToMails[uf.find(idx)].emplace_back(mail);
        }
        // 将合并的邮箱排序，加上名称
        for (auto& [idx, mailVec] : idxToMails){
            sort(mailVec.begin(), mailVec.end());
            vector<string> tmp(1, accounts[idx][0]);
            tmp.insert(tmp.end(), mailVec.begin(), mailVec.end());
            res.emplace_back(tmp);
        }
        return res;
    }
};
```
