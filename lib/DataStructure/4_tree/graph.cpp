
# 图

## [网络延迟时间(Dijkstra)](https://leetcode-cn.com/problems/network-delay-time/)

- 题目描述

  <img src="imgs/algorithm/problem743.png" alt="problem743" style="zoom:80%;" />

- 思路：根据题意，从节点 k 发出的信号，到达节点 x 的时间就是节点 k 到节点 x 的最短路的长度。因此我们需要求出节点 k 到其余所有点的最短路，其中的最大值就是答案。若存在从 k 出发无法到达的点，则返回 -1

  - 使用 Dijkstra 算法求从 k 到 x 的单源最短路径：
    - Dijkstra 算法采用的是一种**贪心**的策略，声明一个数组 dis 来保存源点到各个顶点的最短距离和一个**保存已经找到了最短路径的顶点的集合**：T，初始时，源点 s 的路径权重被赋为 0 （dist[s] = 0）。若对于顶点 s 存在能**直接到达的边**（s,m），则把 dist[m]设为 w(s, m)，同时把所有其他（s**不能直接到达的**）顶点的**路径长度设为无穷大**。初始时，集合 T 只有顶点 s
    - 然后，**从 dist 数组选择最小值**，则该值就是源点 s 到该值对应的顶点的最短路径，并且把该点加入到 T 中，OK，此时完成一个顶点，然后，**需要看看新加入的顶点是否可以到达其他顶点并且看看通过该顶点到达其他点的路径长度是否比源点直接到达短**，如果是，那么就**替换这些顶点在 dist 中的值**
    - 再从 dis 中找出最小值，重复上述动作，直到 T 中包含了图的所有顶点

  <img src="imgs/algorithm/solution743.png" alt="solution743" style="zoom:80%;" />

- 解题代码

  ```c++
  // 朴素dijkstra
  class Solution {
  public:
      int networkDelayTime(vector<vector<int>> &times, int n, int k) {
          const int inf = INT_MAX / 2;	// 防止上溢
          // 邻接矩阵建图
          vector<vector<int>> g(n, vector<int>(n, inf));
          for (auto &t : times) {
              int x = t[0] - 1, y = t[1] - 1;	// 编号-1存储
              g[x][y] = t[2];		// x-->y
          }
  		// 记录源点(k)到其他各节点的最短距离
          vector<int> dist(n, inf);
          dist[k - 1] = 0;
          // 记录已经确定最短路径的节点
          vector<int> used(n);
          for (int i = 0; i < n; ++i) {
              int x = -1;
              // 遍历未确定节点
              for (int y = 0; y < n; ++y) {
                  // 从未确定节点中选出距离源点最近的节点
                  if (!used[y] && (x == -1 || dist[y] < dist[x])) {
                      x = y;
                  }
              }
              used[x] = true;
              // 用x更新所有x能到达的y，dist[y] = min(dist[x]+dist[x->y], dist[y])
              for (int y = 0; y < n; ++y) {
                  dist[y] = min(dist[y], dist[x] + g[x][y]);
              }
          }

          int ans = *max_element(dist.begin(), dist.end());
          return ans == inf ? -1 : ans;
      }
  };

  // 优化：邻接表+优先队列
  class Solution {
  public:
  	using pii = pair<int, int>;
      int networkDelayTime(vector<vector<int>>& times, int n, int k) {
          int inf = 0x3f3f3f3f;
          vector<vector<pii> > eg(n+1);
          /* 预分配空间，减少稠密图时多次插入造成空间重新分配的消耗 */
          for(auto& E: eg)
              E.reserve(n+1);

          // 前驱节点-<后继节点，时间>
          for(auto& x: times){
              eg[x[0]].emplace_back(x[1], x[2]);
          }

          int ans = 0;
          vector<int> vis(n+1, inf); // 记录源点到其他所有节点的最短距离

          // 小根堆，寻找「未确定节点」中与源点距离最近的节点
          priority_queue<pii, vector<pii>, greater<pii> > q;
          q.emplace(0, k);
          vis[k] = 0;
          while(!q.empty()) {
              // time为源点到当前cur节点的最小距离
              auto [time, cur] = q.top();
              q.pop();
              // 源点到cur的当前路径非最短路径 ******
              if(vis[cur] < time)
                  continue;
              ans = max(ans, time);
              // 从cur出发更新cur能到达的所有节点的最小距离
              for(auto [next, nextTime]: eg[cur]) {
                  // 更新结果存到vis中，同时加入队列
                  int tmpTime = nextTime + time;
                  if(vis[next] > tmpTime) {
                      q.emplace(tmpTime, next);
                      vis[next] = tmpTime;
                  }
              }
          }
          // 有节点未被遍历，vis的值为inf
          return find(begin(vis)+1, end(vis), inf) == end(vis) ? ans : -1;
      }
  };
  ```

## [访问所有节点的最短路径](https://leetcode-cn.com/problems/shortest-path-visiting-all-nodes/)

- 题目描述

  <img src="imgs/algorithm/problem847.png" alt="problem847" style="zoom: 80%;" />

- 解题思路

  <img src="imgs/algorithm/solution847.png" alt="solution847" style="zoom:80%;" />

- 解题代码

  ```c++
  class Solution {
  public:
      int shortestPathLength(vector<vector<int>>& graph) {
          int n = graph.size();
          // 三元组表示每个节点，包含
          // 1. 节点编号
          // 2. 已遍历节点(mask表示，已遍历的位置为1)
          // 3. 到目前节点的路径长度
          queue<tuple<int, int, int>> q;
          // 记录已遍历的路径，防止重复遍历
          vector<vector<int>> seen(n, vector<int>(1 << n));
          for (int i = 0; i < n; ++i) {
              q.emplace(i, 1 << i, 0);
              seen[i][1 << i] = true;
          }

          int ans = 0;
          while (!q.empty()) {
              auto [u, mask, dist] = q.front();
              q.pop();
              // mask = (1 << n) - 1表示全部节点都已被遍历
              if (mask == (1 << n) - 1) {
                  ans = dist;
                  break;
              }
              // 搜索相邻的节点
              for (int v: graph[u]) {
                  // 将 mask 的第 v 位置为 1
                  int mask_v = mask | (1 << v);
                  // 当前路径没被搜索过，入队
                  if (!seen[v][mask_v]) {
                      q.emplace(v, mask_v, dist + 1);
                      seen[v][mask_v] = true;
                  }
              }
          }
          return ans;
      }
  };
  ```

