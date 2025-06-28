
# 前缀树

## [实现 Trie (前缀树)](https://leetcode-cn.com/problems/implement-trie-prefix-tree/)

- 代码实现

  ```c++
  struct TrieTreeNode {
      unordered_map<char, TrieTreeNode *> children;
      string word;

      void add(string str) {
          TrieTreeNode * node = this;
          for(auto & ch : str) {
              if(node->children[ch] == nullptr) {
                  node->children[ch] = new TrieTreeNode();
              }
              node = node->children[ch];
          }
          node->word = str;
      }

      bool find(string str) {
          TrieTreeNode node = this;
          for(auto & ch : str) {
              if(node->children.count(ch) > 0) {
                  node = children[ch];
              }
              else {
                  return false;
              }
          }
          return node->word == str;
      }
  };
  ```

## [最小高度树](https://leetcode-cn.com/problems/minimum-height-trees/)

- 题目描述

  <img src="imgs/algorithm/problem310.png" alt="problem310" style="zoom:80%;" />

- 解题思路：

  - 暴力枚举每个节点为根的树的高度，取最小值时的根节点
  - 看图（题目介绍里面的图）分析一下，发现，越是靠里面的节点越有可能是最小高度树。**度为 1 的节点为叶节点**，从边缘开始，先找到所有度为 1 的节点，然后**把所有度为 1 的节点进队列**，然后不断地 bfs，最后找到的就是两边同时向中间靠近的节点，那么这个中间节点就相当于把整个距离二分了，那么它当然就是到两边距离最小的点，也就是**到其他叶子节点最近的节点**

- 代码实现

  ```c++
  // 暴力法，超时
  vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
      vector<vector<int> > graph(n, vector<int>());
      for(auto &vec : edges) {
          graph[vec[0]].push_back(vec[1]);
          graph[vec[1]].push_back(vec[0]);
      }
      vector<int> ret;
      int minHeight = INT_MAX;
      for(int root = 0; root < n; ++root) {
          queue<int> q;
          q.push(root);
          unordered_set<int> visited;
          int height = 0;
          while(!q.empty()) {
              int size = q.size();
              ++height;
              while(size--) {
                  int cur = q.front();
                  q.pop();
                  visited.insert(cur);
                  for(const int & next : graph[cur]) {
                      if(!visited.count(next)) {
                          q.push(next);
                      }
                  }
              }
          }
          if(height < minHeight) {
              ret.clear();
              ret.push_back(root);
              minHeight = height;
          }
          else if(height == minHeight) {
              ret.push_back(root);
          }
      }
      return ret;
  }

  // 拓扑+bfs
  vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
      if(n == 1) {
          return {0};
      }
      vector<vector<int> > graph(n, vector<int>());
      vector<int> degree(n, 0);	// 记录节点的度
      for(auto &vec : edges) {
          graph[vec[0]].push_back(vec[1]);
          graph[vec[1]].push_back(vec[0]);
          ++degree[vec[0]];
          ++degree[vec[1]];
      }
      queue<int> q;
      for(int i = 0; i < n; ++i) {
          if(degree[i] == 1) {
              q.push(i);
          }
      }
      vector<int> ret;  // 记录最后一次遍历的结果，即最内层的节点集合
      while(!q.empty()) {
          ret.clear();
          int size = q.size();
          while(size--) {
              int cur = q.front();
              q.pop();
              ret.push_back(cur); // 记录遍历结果
              for(auto &next : graph[cur]) {
                  --degree[next];
                  if(degree[next] == 1) {	// 度为1，可以加入队列，进行下一层遍历
                      q.push(next);
                  }
              }
          }
      }
      return ret;
  }
  ```

## [多次搜索](https://leetcode-cn.com/problems/multi-search-lcci/)

- 题目描述

  <img src="imgs/algorithm/problem17-17.png" alt="problem17-17" style="zoom:80%;" />

- 解题思路

  - 对 smalls 建 trie 树，其中每个树节点的 smId 记录对应的 smalls 数组中的下标，用于查找时，记录对应 smalls[smIdx]出现位置
  - 遍历 big 的所有后缀，并在 trie 树中查找后缀。对于查找路径上经过的所有有效 smId（smId 有效值为大于等于 0 的数），将后缀的起始 id 加入到 smId 对应的 ret 中

- 解题代码

  ```c++
  class Solution {
  public:
      vector<vector<int>> multiSearch(string big, vector<string>& smalls) {
          int n = smalls.size();
          vector<vector<int> > ret(n);
          Trie t;
          for(int i = 0; i < n; ++i) {
              if(!smalls[i].empty()) {
                  t.insert(smalls[i], i);
              }
          }
          for(int i = 0; i < big.size(); ++i) {
              string subWord(std::move(big.substr(i)));
              t.find(subWord, ret, i);
          }
          return ret;
      }
  private:
      struct Trie {
          vector<Trie *> children;
          int smIdx;
          Trie() : children(26, nullptr), smIdx(-1){}

          void insert(string & word, int smallIdx) {
              auto cur = this;
              for(auto ch : word) {
                  auto next = ch - 'a';
                  if(cur->children[next] == nullptr) {
                      cur->children[next] = new Trie();
                  }
                  cur = cur->children[next];
              }
              cur->smIdx = smallIdx;
          }

          void find(string & word, vector<vector<int> > &ret, int startIdx) {
              auto cur = this;
              for(auto& ch : word) {
                  auto next = ch - 'a';
                  if(cur->children[next] == nullptr) {
                      return;
                  }
                  if(cur->children[next]->smIdx != -1) {
                      ret[cur->children[next]->smIdx].push_back(startIdx);
                  }
                  cur = cur->children[next];
              }
          }
      };
  };
  ```
