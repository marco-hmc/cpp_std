
# DFS+回溯

## [删除无效的括号](https://leetcode-cn.com/problems/remove-invalid-parentheses/)

- 题目描述：给你一个由若干括号和字母组成的字符串 `s` ，删除最小数量的无效括号，使得输入的字符串有效。
- 返回所有可能的结果。答案可以按 **任意顺序** 返回

- 例子

  ```
  输入: "()())()"输出: ["()()()", "(())()"]
  ```

- 思路：

  - **如果当前遍历到的左括号的数目严格小于右括号的数目则表达式无效**
  - 因此，我们可以遍历一次输入字符串，统计「左括号」和「右括号」出现的次数
    - 当遍历到「右括号」的时候
      - 如果此时「左括号」的数量不为 0，因为 「右括号」可以与之前遍历到的「左括号」匹配，此时「左括号」出现的次数 -1
      - 如果此时「左括号」的数量为 0，「右括号」数量加 1
    - 当遍历到「左括号」的时候，「左括号」数量加 1
  - 通过这样的计数规则，最后「左括号」和「右括号」的数量就是**各自最少应该删除的数量**

- 代码实现

  ```c++
  vector<string> removeInvalidParentheses(string s) {
      if(s.empty())
          return {""};
      set<string> ret;	// hashset去重
      int leftRemove = 0, rightRemove = 0;    // 记录最少删除多少左括号和右括号
      int n = s.size();

      for(auto & ch: s){
          if(ch == '(')
              leftRemove++;
          else if(ch == ')') {
          	if(leftRemove == 0) // 多余右括号
              	rightRemove++;
              if(leftRemove > 0)  // 可以抵消一个左括号
                  leftRemove--;
          }
      }
      string path;
      dfs(ret, s, path, 0, 0, 0, leftRemove, rightRemove);
      return vector<string> (ret.begin(), ret.end());
  }

  void dfs(set<string> & ret, string &s, string &path, int idx,
           int left, int right, int leftRemove, int rightRemove)
  {
      if(idx == s.size()){
          if(leftRemove == 0 && rightRemove == 0) {
              ret.insert(path);
          }
          return;
      }

      char ch = s[idx];
      // 删除当前字符
      if(ch == '(' && leftRemove > 0)
          dfs(ret, s, path, idx+1, left, right, leftRemove-1, rightRemove);
      else if(ch == ')' && rightRemove > 0)
          dfs(ret, s, path, idx + 1, left, right, leftRemove, rightRemove-1);

      // 保留当前字符
      path.push_back(ch);
      if(ch != '(' && ch!=')')    // 字母直接保留
          dfs(ret, s, path, idx+1, left, right, leftRemove, rightRemove);
      else if(ch == '(')      // 保留左括号，left+1
          dfs(ret, s, path, idx+1, left+1, right, leftRemove, rightRemove);
      else if(left > right)     // 左括号数大于右括号数时才能保留右括号，right+1
          dfs(ret, s, path, idx+1, left, right + 1, leftRemove, rightRemove);

      path.pop_back();	// 回溯
  }
  ```

## [被围绕的区域](https://leetcode-cn.com/problems/surrounded-regions/)

## [串联字符串的最大长度](https://leetcode-cn.com/problems/maximum-length-of-a-concatenated-string-with-unique-characters/)

- 代码实现(妙)

  ```c++
  int maxLength(vector<string>& arr) {
      if(arr.empty())
          return 0;
      // 统计本身不含重复字符的字符串，每个元素表示字符串含有的字符
      vector<int> masks;
      for(auto str : arr){
          int mask = 0;
          for(auto ch : str) {
              ch -= 'a';
              // 重复，跳出
              if((mask >> ch) & 1) {
                  mask = 0;
                  break;
              }
              // 记录字符
              mask |= 1 << ch;
          }
          if(mask > 0) {
              masks.emplace_back(mask);
          }
      }

      // 计算mask中有多少个1
      auto count = [] (int mask)->int {
          int ret = 0;
          while(mask != 0) {
              if(mask & 1) {
                  ret++;
              }
              mask >>= 1;
          }
          return ret;
      };
      // dfs + 回溯
      int ret = 0;
      // mask记录不重复的字符
      function<void(int, int)> dfs = [&](int pos, int mask) {
          if(pos == masks.size()) {
              ret = max(ret, count(mask));
              return;
          }
          // 如果当前字符串的字符在之前没出现，可以选择当前字符串
          if((mask & masks[pos]) == 0) {
              dfs(pos + 1, mask | masks[pos]);
          }
          // 不选当前字符串
          dfs(pos + 1, mask);
      };

      dfs(0, 0);
      return ret;
  }
  ```

## [二叉搜索树序列](https://leetcode-cn.com/problems/bst-sequences-lcci/)

- 代码实现：本质上**求 BST 的任意遍历方式得到的序列集合**。隐含条件：**遍历了父节点才能遍历其子节点**

  ```c++
  vector<vector<int>> BSTSequences(TreeNode* root)
  {
      if(root == nullptr)
          return {};
      vector<vector<int> > ret;
      vector<int> path;
      deque<TreeNode*> que;
      que.push_front(root);
      dfs(que, ret, path);
      return ret;
  }

  void dfs(deque<TreeNode*> &que, vector<vector<int> > &ret, vector<int> &path) {
      if(que.empty()) {
          ret.push_back(path);
          return;
      }
      // 当前层能选择遍历的节点数，同一层的节点插入顺序任意
      int n = que.size();
      while(n--) {
          auto node = que.front();
          que.pop_front();
          path.emplace_back(node->val);
          // 记录下一轮的所有可能选择
          if(node->left!=nullptr)
              que.push_back(node->left);
          if(node->right!=nullptr)
              que.push_back(node->right);

          // 进入下一轮
          dfs(que, ret, path);

  		// 本轮结束，将回溯，需要复原返回
          if(node->right != nullptr)
              que.pop_back();
          if(node->left != nullptr)
              que.pop_back();
          // 此处不能push_front原因是：上面出队是front，已经被先选择遍历了，就要从队尾入队，防止重复遍历
          que.push_back(node);
          path.pop_back();
      }
  }
  ```

## [单词转换](https://leetcode-cn.com/problems/word-transformer-lcci/)

- 题目与“单词接龙”相似，但是本题要求路径，而“单词接龙”求最短路径，所以此题用 dfs 解决，且需要剪枝，不然会超时

- 解题代码

  ```c++
  vector<string> findLadders(string beginWord, string endWord, vector<string>& wordList) {
      // 判断两个单词能否互相转换
      auto check = [](string & str1, string &str2)->bool {
          if(str1.size() != str2.size()) {
              return false;
          }
          int diff = 0;
          for(int i = 0; i < str1.size(); ++i) {
              if(str1[i] != str2[i]) {
                  ++diff;
              }
              if(diff > 1) {
                  return false;
              }
          }
          return diff == 1;
      };
      vector<string> ret;
      int n = wordList.size();
      // 记录已经遍历过的单词
      vector<bool> visited(n, false);
      function<bool(string &)> dfs = [&](string &word) {
          if(word == endWord) {
              ret.push_back(word);
              return true;
          }
          ret.push_back(word);
          for(int i = 0; i < n; ++i) {
              if(visited[i]) {
                  continue;
              }
              if(check(word, wordList[i])) {
                  visited[i] = true;
                  if(dfs(wordList[i])) {
                      return true;
                  }
                  // 此处没有将visited[i]设为false是为了剪枝
                  // 因为代码执行到此处表明沿着wordList[i]搜索到不了endWord
                  // 所以后面无需再转换到此分支进行搜索
              }
          }
          ret.pop_back();
          return false;
      };

      dfs(beginWord);
      return ret;
  }
  ```

## [单词拆分 II](https://leetcode-cn.com/problems/word-break-ii/)

- 题目描述

  <img src="imgs/algorithm/problem140.png" alt="problem140" style="zoom:80%;" />

- 解题思路：dfs+记忆化搜索+回溯

  <img src="imgs/algorithm/solution140.png" alt="solution140" style="zoom:80%;" />

- 解题代码

  ```c++
  vector<string> wordBreak(string s, vector<string>& wordDict) {
      int n = s.size();
      unordered_set<string> dict(wordDict.begin(), wordDict.end());
      // 用于记录int下标开始的子串，所有可能的划分情况
      unordered_map<int, vector<string> > rec;
      function<void(int)> dfs = [&](int idx) {
          // 已经搜索过，剪枝返回
          if(rec.count(idx)) {
              return;
          }
          // 到达最后，注意要把rec[idx]设为空串，返回上层拼接
          if(idx == n) {
              rec[idx] = {""};
              return;
          }
          for(int i = idx + 1; i <= n; ++i) {
              string word = std::move(s.substr(idx, i - idx));
              if(dict.count(word)) {
                  dfs(i); // 获得i后所有可能，自顶向下
                  // 返回后组装
                  for(auto & str : rec[i]) {
                      rec[idx].push_back(str.empty() ? word : word + " " + str);
                  }
              }
          }
      };
      dfs(0);
      return rec[0];
  }
  ```
