
# BFS

## [单词接龙](https://leetcode-cn.com/problems/word-ladder/)

- 题目描述

  <img src="imgs/algorithm/problem127.png" alt="problem127" style="zoom:80%;" />

- 解题思路

  <img src="imgs/algorithm/solution127-1.png" alt="solution127-1" style="zoom:80%;" />

  <img src="imgs/algorithm/solution127-2.png" alt="solution127-2" style="zoom:80%;" />

- 解题代码

  ```c++
  class Solution {
  public:
      //确定俩个单词能否互相转换
      bool exchange_word(const string& a, const string& b) {
          int misMatch = 0;
          int len = a.size();
          for (int i = 0; i < len; ++i) {
              if (a[i] != b[i])
                  ++misMatch;
              if (misMatch > 1) return false;     //如果>1直接返回
          }
          return misMatch == 1;
      }

      int ladderLength(string beginWord, string endWord, vector<string>& wordList) {

          unordered_set<string> word_set(wordList.begin(), wordList.end());
          if (word_set.count(endWord) == 0)
              return 0;

          queue<string> q;
          q.push(beginWord);

          //必须消除已经是树节点的单词
          word_set.erase(beginWord);
          int steps = 1;

          while (!q.empty()) {
              int size = q.size();                //记住当前队列的长度
              while(size--) {    //for循环pop出队列所有元素
                  auto str = q.front();
                  q.pop();
                  vector<string> tmp;             //保存str能够转换的单词
                  for (const string& word : word_set) {
                      if (exchange_word(str, word)) {
                          if (word == endWord)
                              return steps + 1;
                          tmp.push_back(word);
                          q.push(word);
                      }
                  }
                  // tmp中的单词已经是树中的节点,我们需要erase它使得后续的树节点不重复
                  // 在这里删除是防止上面遍历迭代器失效
                  for (const auto& str : tmp)
                      word_set.erase(str);
              }
              ++steps;
          }
          return 0;
      }
  };
  ```

## [被围绕的区域](https://leetcode-cn.com/problems/surrounded-regions/)

- 题目描述

  <img src="imgs/algorithm/problem130.png" alt="problem130" style="zoom: 80%;" />

- 解题思路

  <img src="imgs/algorithm/solution130.png" alt="solution130" style="zoom:80%;" />

- 代码实现

  ```c++
  void solve(vector<vector<char>>& board) {
      if(board.empty())
          return;
      int rows = board.size(), cols = board[0].size();

      queue<pair<int, int> > pos;
      for(int i = 0; i < rows; ++i){
          if(board[i][0] == 'O')
              pos.push({i, 0});
          if(board[i][cols-1] == 'O')
              pos.push({i, cols-1});
      }

      for(int i = 1; i < cols - 1; ++i){
          if(board[0][i] == 'O')
              pos.push({0, i});
          if(board[rows-1][i] == 'O')
              pos.push({rows-1, i});
      }

      vector<pair<int, int> > directions {{1,0}, {-1, 0}, {0, 1}, {0, -1}};
      while(!pos.empty()) {
          auto cur = pos.front();
          pos.pop();
          int x = cur.first;
          int y = cur.second;
          board[x][y] = 'Y';
          for(auto &[h, v] : directions) {
              //
              if(check(board, rows, cols, x+h, y+v)) {
               	pos.push({x+h, y+v});
              }
          }
      }

      for(int i = 0; i<rows; ++i){
          for(int j = 0; j<cols; ++j) {
              if(board[i][j] == 'O')
                  board[i][j] = 'X';
              else if(board[i][j] == 'Y')
                  board[i][j] = 'O';
          }
      }
  }

  bool check(vector<vector<char> > &board,  int rows, int cols, int x, int y){
      if(x >= rows || y >= cols || x < 0 || y < 0 || board[x][y] != 'O')
          return false;
      return true;
  }
  ```

## [网格中的最短路径](https://leetcode-cn.com/problems/shortest-path-in-a-grid-with-obstacles-elimination/)(重点)

- 题目描述

  <img src="imgs/algorithm/problem1293.png" alt="problem1293" style="zoom:80%;" />

- 解题思路

  <img src="imgs/algorithm/solution1293.png" alt="solution1293" style="zoom:80%;" />

  - 优化：这里是直接看 visited\[i]\[j][rest] 但是假如重复访问 i,j 点的时候，如果已访问过的 rest 值更大，就不需要再访问了。因为 rest 大的情况肯定比 rest 小的情况走的远

- 解题代码

  ```c++
  struct Nagato {
      int x, y;
      int rest;
      Nagato(int _x, int _y, int _r): x(_x), y(_y), rest(_r) {}
  };

  class Solution {
  private:
      static constexpr int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  public:
      int shortestPath(vector<vector<int>>& grid, int k) {
          int m = grid.size(), n = grid[0].size();
          if (m == 1 && n == 1) {
              return 0;
          }
  		// 缩小搜索空间
          k = min(k, m + n - 3);

          // 记录状态，第三维很重要，记录x,y位置，还能消除多少个障碍，
          // 由于i,j节点可能被多次遍历，每次剩余数不同代表不同状态
          bool visited[m][n][k + 1];
          memset(visited, false, sizeof(visited));
          queue<Nagato> q;
          q.emplace(0, 0, k);
          visited[0][0][k] = true;

          for (int step = 1; q.size() > 0; ++step) {
              int cnt = q.size();
              for (int _ = 0; _ < cnt; ++_) {
                  Nagato cur = q.front();
                  q.pop();
                  for (int i = 0; i < 4; ++i) {
                      int nx = cur.x + dirs[i][0];
                      int ny = cur.y + dirs[i][1];
                      if (nx >= 0 && nx < m && ny >= 0 && ny < n) {
                          if (grid[nx][ny] == 0 && !visited[nx][ny][cur.rest]) {
                              if (nx == m - 1 && ny == n - 1) {
                                  return step;
                              }
                              q.emplace(nx, ny, cur.rest);
                              visited[nx][ny][cur.rest] = true;
                          }
                          else if (grid[nx][ny] == 1 && cur.rest > 0
                                   && !visited[nx][ny][cur.rest - 1]) {
                              q.emplace(nx, ny, cur.rest - 1);
                              visited[nx][ny][cur.rest - 1] = true;
                          }
                      }
                  }
              }
          }
          return -1;
      }
  };
  ```

