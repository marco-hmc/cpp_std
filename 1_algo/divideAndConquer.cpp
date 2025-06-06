
# 分治

## [至少有 K 个重复字符的最长子串](https://leetcode-cn.com/problems/longest-substring-with-at-least-k-repeating-characters/)

- 思路：

  - 遍历找到哪些字符出现次数少于 k
  - 再次遍历找到哪些不满足条件的位置（就是字符出现次数少于 k），就把字符串拆成更小的一部分
  - 更小一部分里找满足条件的长度
  - 调用结束：找不到任何不满足条件位置，就是当前字符串整个就是满足条件的
  - 返回结果： 在每一部分调用里记录最大的长度，就是结果

- 代码实现

  ```c++
  class Solution {
  public:
      int longestSubstring(string s, int k) {
          int n = s.size();
          if(k == 1) {
              return n;
          }
          int maxLen = 0;
          dfs(s, 0, n-1, maxLen, k);
          return maxLen;
      }

      void dfs(string &s, int left, int right, int &maxLen, int k) {
          if(left > right) {
              return;
          }
          int len = right - left + 1;
          // 剪枝
          if(maxLen >= len || len < k) {
              return;
          }
          // 统计当前字串字符类型及数量
          vector<int> count(26);
          for(int i = left; i <= right; ++i) {
              ++count[s[i] - 'a'];
          }
          // 寻找分割点(字符数<k)
          int sep = -1;
          for(int i = left; i <= right; ++i) {
              if(count[s[i] - 'a'] < k) {
                  sep = i;
                  break;
              }
          }
          // 不存在分割点，字串满足条件，更新最大长度
          if(sep == -1) {
              maxLen = max(maxLen, len);
              return;
          }
          // 根据分割点分治
          dfs(s, left, sep - 1, maxLen, k);
          dfs(s, sep + 1, right, maxLen, k);
      }
  };
  ```
