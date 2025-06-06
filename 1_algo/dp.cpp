
# 动态规划

## [最长公共子序列(LCS)](https://leetcode-cn.com/problems/longest-common-subsequence/)

- 题目描述

  - 给定两个字符串 text1 和 text2，返回这两个字符串的最长 公共子序列 的长度。如果不存在 公共子序列 ，返回 0 。一个字符串的 子序列 是指这样一个新的字符串：它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。

    例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是 "abcde" 的子序列。两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列。

- 思路

  <img src="imgs/algorithm/solution11.png" alt="solution11" style="zoom:80%;" />

- 代码实现

  ```c++
  int longestCommonSubsequence(string text1, string text2) {
      int n = text1.size();
      int m = text2.size();
      vector<vector<int> > dp(n+1, vector<int> (m+1, 0));

      for(int i = 1; i <= n; ++i) {
          for(int j = 1; j <= m; ++j) {
              if(text1[i-1] == text2[j-1]) {
                  dp[i][j] = dp[i-1][j-1] + 1;
              }
              else {
                  dp[i][j] = max(dp[i][j-1], dp[i-1][j]);
              }
          }
      }
      return dp[n][m];
  }
  ```

## 最长递增子序列(LIS)

- 最长递增子序列不唯一，但是最长大小唯一，可以使用动态规划

- 贪心+二分解法：如果我们要使上升子序列尽可能的长，则我们需要让序列上升得尽可能慢，因此我们希望每次在上升子序列最后加上的那个数尽可能的小

  ![solution2](imgs/algorithm/solution2.png)

- 代码

  ```c++
  // n2 解法
  int LIS(vector<int> & arr){
      if(arr.empty())
          return 0;
      int n = arr.size();
      vector<int> dp(n, 1);
      int maxLen = 0;
      for(int i = 1; i < n; i++) {
          for(int j = 0; j < i; j++){
              if(arr[j] < arr[i]){
                  dp[i] = max(dp[i], dp[j] + 1);
              }
          }
          maxLen = max(maxLen, dp[i]);
      }
      return maxLen;
  }

  // nlogn 解法，贪心+二分
  int lengthOfLIS(vector<int>& nums) {
      int n = (int)nums.size();
      if (n == 0) {
          return 0;
      }
      vector<int> d;
      d.push_back(nums[0]);
      for (int i = 1; i < n; ++i) {
          if (nums[i] > d.back()) {
              d.push_back(nums[i]);
          } else {
              auto pos = lower_bound(d.begin(), d.end(), nums[i]) - d.begin();
              d[pos] = nums[i];
          }
      }
      return int(d.size());
  }
  ```

## LIS 进阶

- 给定数组 arr，设长度为 n，输出 arr 的最长递增子序列。**如果有多个答案，请输出其中字典序最小的**。此时需要输出字典序中最小的子序列

- 解法：

  - 两步走：1. 求最大递增子序列长度；2. 求字典靠前子序列

- 第一步思路：

  - 贪心+二分：假设数组 arr 为`[2, 3, 1, 2, 3]`，**vec 数组里面存放递增子序列(存放长度为 i+1 的 LIS 的最后一个数为 vec[i])，maxLen 数组里存放以元素 i 结尾的最大递增子序列长度**，那么遍历数组 arr 并执行如下更新规则:

    1. 初始情况下，vec 为[2]，maxLen[1]
    2. 接下来遇到 3，由于 vec 最后一个元素小于 3，直接更新，vec 为[2,3]，maxLen[1,2]
    3. 接下来遇到 1，由于 vec**最后的元素大于 1**, 我们在 vec 中**查找大于等于 1 的第一个元素的下标，并用 1 替换之**，此时 vec 为[1,3], maxLen[1,2,1]
    4. 接下来遇到 2，由于 vec 最后的元素大于 2，我们在 vec 中查找大于等于 2 的第一个元素的下标，并用 2 替换之，此时 vec 为[1,2], maxLen[1,2,1,2]
    5. 接下来遇到 3，由于 vec 最后一个元素小于 3，直接更新，vec 为[1,2,3]，maxLen 为[1,2,1,2,3]
    6. 此时**vec 的大小就是整个序列中最长递增子序列的长度**（但是 vec 不一定是本题的最终解）

- 第二步思路：

  - 假设我们原始数组是 arr，得到的 maxLen 为`[1,2,3,1,3]`，最终输出结果为 res（字典序最小的最长递增子序列），res 的最后一个元素在 arr 中位置无庸置疑是`maxLen[i]==3`对应的下标，那么到底是`arr1[2]`还是`arr[4]`呢？如果是`arr[2]`，那么`arr[2]<arr[4]`，则`maxLen[4]=4`，与已知条件相悖。因此我们应该取`arr[4]`放在 res 的最后一个位置，**同长度，后面的一定比前面小，所以从后往前面填充 res**

- 代码实现

  ```c++
  vector<int> LIS(vector<int> & arr){
      if(arr.empty())
          return {};
      vector<int> ret;	// 记录结果
      vector<int> maxLen;	// 记录以arr[i]为最后元素的LIS长度
      int n = arr.size();
      ret.emplace_back(arr[0]);
      maxLen.emplace_back(1);
      for(int i = 1; i < n; ++i){
          if(arr[i] > ret.back()){
              ret.emplace_back(arr[i]);
              maxLen.emplace_back(ret.size());
          }
          else{
              // 第一个大于等于arr[i]的位置
              int pos = lower_bound(ret.begin(), ret.end(), arr[i]) - ret.begin();	            				arr[pos] = arr[i];
              maxLen.emplace_back(pos + 1);
          }
      }
      for(int i = n - 1, j = ret.size(); i>=0; --i) {	// 从后往前填充
          if(maxLen[i] == j)
              ret[--j] = arr[i];
      }
      return ret;
  }
  ```

## [俄罗斯套娃信封问题](https://leetcode-cn.com/problems/russian-doll-envelopes/)

- 二维矩阵，与上面 LIS 有异曲同工之妙，只需把第一维排好序，然后在第二维求 LIS 即可。同样有两种复杂度解法

  ```c++
  // n2 解法
  int maxEnvelopes(vector<vector<int>>& envelopes) {
  	int n = envelopes.size();
      // 排序函数，在第一维相同情况下，第二维降序排列，能防止同时出现第一维度相同的互套的情况，很妙！
      auto comp = [](vector<int> &vec1, vector<int> &vec2) {
          return (vec1[0] < vec2[0]) || (vec1[0] == vec2[0] && vec1[1] > vec2[1]);
      };
      sort(envelopes.begin(), envelopes.end(), comp);
     	vector<int> dp(n, 1);
      int maxNum = 1;
      for(int i = 1; i < n; ++i) {
          for(int j = i-1; j >= 0; --j) {
              if(envelops[j][1] < envelopes[i][1]) {
                  dp[i] = max(dp[i], dp[j] + 1);
              }
          }
          maxNum = max(maxNum, dp[i]);
      }
      return maxNum;
  }

  // nlogn解法
  int maxEnvelopes(vector<vector<int>>& envelopes) {
  	int n = envelopes.size();
      auto comp = [](vector<int> &vec1, vector<int> &vec2) {
          return (vec1[0] < vec2[0]) || (vec1[0] == vec2[0] && vec1[1] > vec2[1]);
      };
      sort(envelopes.begin(), envelopes.end(), comp);
     	vector<int> vec;
      vec.emplace_back(envelopes[0][1]);
      for(int i = 1; i < n; ++i) {
          if(envelopes[i][1] > vec.back()) {
              vec.emplace_back(envelopes[i][1]);
          }
          else {
              auto pos = lower_bound(vec.begin(), vec.end(), envelopes[i][1]) - vec.begin();
              vec[pos] = envelopes[i][1];
          }
      }
      return int(vec.size());
  }
  ```

## 正则表达式匹配

- 给你一个字符串 s 和一个字符规律 p，请你来实现一个支持 '.' 和 '\*' 的正则表达式匹配

  '.' 匹配任意单个字符 '\*' 匹配零个或多个前面的那一个元素所谓匹配，是要涵盖 整个 字符串 s 的，而不是部分字符串

- 思路一：用$f[i][j]$表示 _s_ 的前 _i_ 个字符与 _p_ 中的前 _j_ 个字符是否能够匹配。考虑 _p_ 的第 _j_ 个字符的匹配情况

  - 如果*p*的第 _j_ 个字符是一个小写字母，那么我们必须在 _s_ 中匹配一个相同的小写字母，即

    $$f[i][j] = \begin{cases}&f[i-1][j-1], &s[i]=p[j];\\ &false, &s[i] \ne p[j]. \end{cases}$$

  - 字母 + 星号的组合在匹配的过程中，本质上只会有两种情况

    - 匹配 _s_ 末尾的一个字符，将该字符扔掉，而该组合还可以继续进行匹配；
    - 不匹配字符，将该组合扔掉，不再进行匹配

  - 可以得到字母 + 星号的组合的状态转移方程为

    $$f[i][j] = \begin{cases}&f[i-1][j] \; or\; f[i][j-2], &s[i]=p[j-1];\\ &f[i][j-2], &s[i] \ne p[j-1]. \end{cases}$$

  - 在任意情况下，只要*p*[*j*] 是 `.`，那么*p*[*j*] 一定成功匹配 _s_ 中的任意一个小写字母

  - 边界条件：两个空字符串是可以匹配的，$f[0][0] = $true

- 思路二：递归实现

  <img src="https://gitee.com/canqchen/cloudimg/raw/master/img/regex_recursive.png" alt="regex_recursive" style="zoom:80%;" />

- 代码实现

  ```c++
  // 动态规划
  bool isMatch(string s, string p) {
      int m = s.size();
      int n = p.size();

      auto matches = [&](int i, int j) {
          if (i == 0) {
              return false;
          }
          if (p[j - 1] == '.') {
              return true;
          }
          return s[i - 1] == p[j - 1];
      };

      vector<vector<bool> > f(m + 1, vector<bool>(n + 1, false));
      f[0][0] = true;
      for (int i = 0; i <= m; ++i) {
          for (int j = 1; j <= n; ++j) {
              if (p[j - 1] == '*') {
                  f[i][j] = f[i][j] | f[i][j - 2];	// 不匹配"字母+星号"组合
                  if (matches(i, j - 1)) {		// 星号前字母匹配，匹配"字母+星号"组合
                      f[i][j] = f[i][j] | f[i - 1][j];
                  }
              }
              else {
                  if (matches(i, j)) {
                      f[i][j] = f[i][j] | f[i - 1][j - 1];
                  }
              }
          }
      }
      return f[m][n];
  }

  // 递归实现
  bool isMatch(string s, string p) {
      if(s.empty() && p.empty())
          return true;
      if(p.empty())
          return false;
      int m = s.size(), n = p.size();
      if(n > 1 && p[1]=='*') {		// alpha + *
          if(m > 0 && (s[0] == p[0] || p[0]=='.')){	// 星号前字母匹配
              return isMatch(s.substr(1), p) 			 // 匹配大于等于一个alpha
                  || isMatch(s, p.substr(2));			// 不匹配alpha，丢弃 alpha + *
          }
          else{
              return isMatch(s, p.substr(2));  // 星号前字母不匹配，丢弃 alpha + *
          }
      }
      if(m > 0 && (s[0] == p[0] || p[0] == '.'))
          return isMatch(s.substr(1), p.substr(1));
      return false;
  }
  ```

## [完全平方数](https://leetcode-cn.com/problems/perfect-squares/)

- 题目描述

  <img src="imgs/algorithm/problem279.png" alt="problem279" style="zoom:80%;" />

- 解题代码

  ```c++
  int numSquares(int n) {
      if(n<=0)
          return 0;
      vector<int> dp(n + 1, INT_MAX);
      int max_idx = sqrt(n) + 1;
      vector<int> square_nums(max_idx);
      dp[0] = 0;
      for(int i = 1; i < max_idx; i++) {
          square_nums[i] = i*i;
      }

      for(int i = 1; i <= n; i++){
          for(int j = 1; j < max_idx; j++){
              if(i < square_nums[j])
                  break;
              dp[i] = min(dp[i], dp[i-square_nums[j]] + 1);
          }
      }
      return dp[n];
  }
  ```

## [目标和](https://leetcode-cn.com/problems/target-sum/)

- 题目描述

  <img src="imgs/algorithm/problem494.png" alt="problem494" style="zoom:80%;" />

- 解题思路

  - 回溯，暴力搜索
  - dp

  <img src="imgs/algorithm/solution494.png" alt="solution494" style="zoom:80%;" />

- 解题代码

  ```c++
  int findTargetSumWays(vector<int>& nums, int target) {
      int n = nums.size();
      int totalSum = 0;
      for(auto & num : nums){
          totalSum += num;
      }

      if(abs(target) > abs(totalSum)){
          return 0;
      }
      int len = totalSum * 2 + 1;
      vector<vector<int> > dp(n, vector<int> (len, 0));

      dp[0][nums[0] + totalSum] = 1;
      dp[0][-nums[0] + totalSum] += 1;
      for(int i = 1; i < n; i++) {
          for(int j = -totalSum; j <= totalSum; j++) {
              if(dp[i-1][j+totalSum] > 0){
                  dp[i][j - nums[i] + totalSum] += dp[i - 1][j + totalSum];
                  dp[i][j + nums[i] + totalSum] += dp[i - 1][j + totalSum];
              }
          }
      }
      return dp[n-1][target + totalSum];
  }

  // 根据思路优化
  int findTargetSumWays(vector<int>& nums, int target) {
      int sum = 0;
      for (int& num : nums) {
          sum += num;
      }
      int diff = sum - target;
      if (diff < 0 || diff % 2 != 0) {
          return 0;
      }
      int n = nums.size(), neg = diff / 2;
      vector<vector<int>> dp(n + 1, vector<int>(neg + 1));
      dp[0][0] = 1;
      for (int i = 1; i <= n; i++) {
          int num = nums[i - 1];
          for (int j = 0; j <= neg; j++) {
              dp[i][j] = dp[i - 1][j];
              if (j >= num) {
                  dp[i][j] += dp[i - 1][j - num];
              }
          }
      }
      return dp[n][neg];
  }
  ```

## [打家劫舍 II](https://leetcode-cn.com/problems/house-robber-ii/)

- 代码实现(不能同时抢劫第一间房子和最后一间，分两种情况)

  ```c++
  // 利用滚动数组，降低空间复杂度
  int rob(vector<int>& nums) {
      if(nums.empty())
          return 0;
      if(nums.size() == 1)
          return nums[0];
      if(nums.size() == 2)
          return max(nums[0], nums[1]);

      int n = nums.size();
  	// [0, n-2]
      int preMax = nums[0];
      int curMax1 = max(nums[0], nums[1]);
      for(int i = 2; i < n-1; ++i) {
          int tmp = max1;
          curMax1 = max(preMax + nums[i], curMax1);
          preMax = tmp;
      }
  	// [1, n-1]
      preMax = nums[1];
      int curMax2 = max(nums[1], nums[2]);
      for(int i = 3; i < n; ++i) {
          int tmp = curMax2;
          curMax2 = max(preMax + nums[i], curMax2);
          preMax = tmp;
      }
      return max(curMax1, curMax2);
  }
  ```

## [不同的二叉搜索树](https://leetcode-cn.com/problems/unique-binary-search-trees/)

## [最大整除子集](https://leetcode-cn.com/problems/largest-divisible-subset/)

- 重要思路：整除关系传递性

  - 若 a 整除 b，且 b 整除 c，则 a 整除 c，据此，有

    - 如果整数 a 是整除子集 S1 的最小整数 b 的约数（即 a 整除 b），那么可以将 a 添加到 S1 中得到一个更大的整除子集；
    - 如果整数 c 是整除子集 S2 的最大整数 d 的倍数（即 d 整除 c），那么可以将 c 添加到 S2

    <img src="imgs/algorithm/solution1.png" alt="solution1" style="zoom:80%;" />

  - 代码实现

    ```c++
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        if(nums.empty())
            return {};
        sort(nums.begin(), nums.end());
        int n = nums.size();
        // dp定义为以dp[i]为最大数的集合的元素数目
        // 初始化，每个集合至少包含自身，设为1
        vector<int> dp(n, 1);
        // 统计dp[i]为最大数的集合的元素数目
        for(int i = 1; i < n; ++i) {
            for(int j = 0; j < i; ++j) {
                if(nums[i] % nums[j] == 0) {	// 可将dp[i]纳入dp[j]所在集合
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
        }
        // 找到最大集合，并记录其下标
        int maxCount = dp[0], maxIdx = 0;
        for(int i = 1; i < n; ++i) {
            if(dp[i] > maxCount) {
                maxCount = dp[i];
                maxIdx = i;
            }
        }
        // 找到最大集合元素
        vector<int> ret;
        // 从后往前找，需要记录较大数用来判断是否该集合
        int larger = nums[maxIdx];
        for(int i = maxIdx; i >= 0; --i) {
            if(dp[i] == maxCount && larger % nums[i] == 0) {
                ret.push_back(nums[i]);
                // 用于寻找下一个
                larger = nums[i];
                --maxCount;
            }
        }
        return ret;
    }
    ```

## [最长回文子串](https://leetcode-cn.com/problems/longest-palindromic-substring/)

- 思路：

  - 动态规划

    <img src="imgs/algorithm/solution5.png" alt="solution5" style="zoom:67%;" />

  - 中心扩展

    <img src="imgs/algorithm/solution6.png" alt="solution6" style="zoom:67%;" />

- 代码实现

  ```c++
  // dp
  string longestPalindrome(string s) {
      if(s.empty())
          return "";
      int n = s.size();
      int maxLen = 1, start = 0;
      vector<vector<bool> > dp(n, vector<bool> (n, false));
      for(int i = 0; i<n;i++)
          dp[i][i] = true;
      for(int i = n - 1; i >= 0; i--){
          for(int j = i + 1; j < n; j++){
              if(s[i] == s[j]){
                  if(j == i+1)
                      dp[i][j] = true;
                  else
                      dp[i][j] = dp[i+1][j-1];
              }
              else {
                  dp[i][j] = false;
              }

              if(dp[i][j] && j - i + 1 > maxLen) {
                  maxLen = j - i + 1;
                  start = i;
              }
          }
      }
      return s.substr(start, maxLen);
  }

  // 中心扩展
  string longestPalindrome(string s) {
      if(s.empty())
          return "";
      int n = s.size();

      auto expand = [s = s, n = n](int left, int right)->pair<int, int> {
          while(left >= 0 && right < n && s[left] == s[right]) {
              ++right;
              --left;
          }
          // 回退到回文串首尾
          return {left + 1, right - 1};
      };

      int maxLen = 1, start = 0;
      for(int i = 0; i < n; ++i) {
          auto [left1, right1] = expand(i, i);
          auto [left2, right2] = expand(i, i + 1);
          if(right1 - left1 + 1 > maxLen) {
              start = left1;
              maxLen = right1 - left1 + 1;
          }
          if(right2 - left2 + 1 > maxLen) {
              start = left2;
              maxLen = right2 - left2 + 1;
          }
      }
      return s.substr(start, maxLen);
  }
  ```

## [最长回文子序列](https://leetcode-cn.com/problems/longest-palindromic-subsequence/)

- 思路

  <img src="imgs/algorithm/solution516.png" alt="solution516" style="zoom:80%;" />

  - 其他思路：将字符串反转，然后求两个字符串的最长公共子序列

- 解题代码

  ```c++
  int longestPalindromeSubseq(string s) {
      int n = s.size();
      vector<vector<int> > dp(n, vector<int> (n, 0));
      for(int i = n-1; i >= 0; --i) {
          dp[i][i] = 1;
          for(int j = i + 1; j < n; ++j) {
              if(s[i] == s[j]) {
                  dp[i][j] = dp[i+1][j-1] + 2;
              }
              else {
                  dp[i][j] = max(dp[i+1][j], dp[i][j-1]);
              }
          }
      }
      return dp[0][n-1];
  }
  ```

## [分割回文串 II](https://leetcode-cn.com/problems/palindrome-partitioning-ii/)

- 思路

  <img src="imgs/algorithm/solution8.png" alt="solution8" style="zoom:67%;" />

- 代码实现

  ```c++
  int minCut(string s) {
      if(s.empty()) {
          return 0;
      }
      int n = s.size();
      // 先统计回文串
      vector<vector<bool> > isPam(n, vector<bool>(n, false));
      for(int i = 0; i < n; ++i) {
          isPam[i][i] = true;
      }
      for(int i = n - 2; i >= 0; --i) {
          for(int j = i + 1; j < n; ++j) {
              if(s[j] == s[i]) {
                  if(j == i + 1){
                      isPam[i][j] = true;
                  }
                  else {
                      isPam[i][j] = isPam[i+1][j-1];
                  }
              }
          }
      }

      // 简单写法
      /*
        vector<vector<bool> > isPam(n, vector<bool>(n, true));
        for(int i = n - 2; i >= 0; --i) {
            for(int j = i + 1; j < n; ++j) {
              isPam[i][j] = (s[i] == s[j]) && isPam[i+1][j-1];
            }
        }
        */

      vector<int> dp(n, INT_MAX);
      for(int i = 0; i < n; ++i) {
          // 注意，容易漏掉整个串为回文串情况
          if(isPam[0][i]) {
              dp[i] = 0;
          }
          else {
              for(int j = 0; j < i; ++j) {
                  if(isPam[j + 1][i]) {
                      dp[i] = min(dp[i], dp[j] + 1);
                  }
              }
          }
      }
      return dp[n-1];
  }
  ```

## [编辑距离](https://leetcode-cn.com/problems/edit-distance/)

- 题目描述

  <img src="imgs/algorithm/problem72.png" alt="problem72" style="zoom:80%;" />

- 代码及思路

  ```c++
  int minDistance(string word1, string word2) {
      int m = word1.size(), n = word2.size();
      // dp[i][j]表示word1[0...i-1]转换成word2[0...j-1]所使用的最少操作
      vector<vector<int> > dp(m+1, vector<int> (n+1, 0));
      // 初始化边界条件
      // word1为空(0个字符)，转换成word2[0...i-1]只需插入i次
      for(int i = 1; i <= n; i++)
          dp[0][i] = i;
      // word2为空(0个字符)，转换成word1[0...i-1]只需插入i次
      for(int i = 1; i <= m; i++)
          dp[i][0] = i;

      for(int i = 1; i <= m; i++) {
          for(int j = 1; j <= n; j++) {
              // 两个字符相同，转换次数与之前字串相同
              if(word1[i-1] == word2[j-1]) {
                  dp[i][j] = dp[i-1][j-1];
              }
              else {
                  // dp[i][j]可以由
                  //     dp[i][j-1] 插入一个字符
                  //     dp[i-1][j] 删除一个字符
                  //     dp[i-1][j-1] 替换一个字符
                  // 转化得到。最少操作数取最小值
                  dp[i][j] = min(min(dp[i][j-1], dp[i-1][j]), dp[i-1][j-1]) + 1;
              }
          }
      }
      return dp[m][n];
  }
  ```

## [戳气球](https://leetcode-cn.com/problems/burst-balloons/)

## [恢复空格](https://leetcode-cn.com/problems/re-space-lcci/)(Trie+dp)

- 题目描述

  <img src="imgs/algorithm/problem171-3.png" alt="problem171-3" style="zoom: 80%;" />

- 解题思路，Trie+dp

  <img src="imgs/algorithm/solution17-13.png" alt="solution17-13" style="zoom:80%;" />

- 解题代码

  ```c++
  // 暴力解法：hash
  int respace(vector<string>& dictionary, string sentence) {
      unordered_set<string> dict(dictionary.begin(), dictionary.end());
      int n = sentence.size();
      vector<int> dp(n+1, INT_MAX);
      dp[0] = 0;
      for(int i = 1; i <= n; ++i) {
          dp[i] = dp[i-1] + 1;
          for(int j = 0; j < i; ++j) {
              string tmp = move(sentence.substr(j, i - j));
              if(dict.count(tmp)) {
                  dp[i] = min(dp[i], dp[j]);
              }
          }
      }
      return dp[n];
  }

  // 在hash内层循环中，判断j...i的字符串是否存在时，后缀都是相同的，
  // 所以使用前缀树，将字符串倒序插入前缀树，进行优化，内层遍历从后往前
  class Trie {
  public:
      Trie* next[26] = {nullptr};
      bool isEnd;

      Trie() {
          isEnd = false;
      }

      void insert(string s) {
          Trie* curPos = this;

          for (int i = s.length() - 1; i >= 0; --i) {
              int t = s[i] - 'a';
              if (curPos->next[t] == nullptr) {
                  curPos->next[t] = new Trie();
              }
              curPos = curPos->next[t];
          }
          curPos->isEnd = true;
      }
  };

  class Solution {
  public:
      int respace(vector<string>& dictionary, string sentence) {
          int n = sentence.length(), inf = 0x3f3f3f3f;

          Trie* root = new Trie();
          for (auto& word: dictionary) {
              root->insert(word);
          }

          vector<int> dp(n + 1, inf);
          dp[0] = 0;
          for (int i = 1; i <= n; ++i) {
              dp[i] = dp[i - 1] + 1;

              Trie* curPos = root;
              // 从后往前进行遍历，当不存在后缀时，尽早跳出进行优化
              for (int j = i; j >= 1; --j) {
                  int t = sentence[j - 1] - 'a';
                  if (curPos->next[t] == nullptr) {
                      break;
                  } else if (curPos->next[t]->isEnd) {
                      dp[i] = min(dp[i], dp[j - 1]);
                  }
                  if (dp[i] == 0) {
                      break;
                  }
                  curPos = curPos->next[t];
              }
          }
          return dp[n];
      }
  };
  ```

## [丑数 II](https://leetcode-cn.com/problems/ugly-number-ii/)

- 解题代码

  ```c++
  int nthUglyNumber(int n) {
      vector<int> dp(n + 1);
      dp[1] = 1;
      int p2 = 1, p3 = 1, p5 = 1;
      for (int i = 2; i <= n; i++) {
          int num2 = dp[p2] * 2, num3 = dp[p3] * 3, num5 = dp[p5] * 5;
          dp[i] = min(min(num2, num3), num5);
          if (dp[i] == num2) {
              p2++;
          }
          if (dp[i] == num3) {
              p3++;
          }
          if (dp[i] == num5) {
              p5++;
          }
      }
      return dp[n];
  }
  ```

## [超级丑数](https://leetcode-cn.com/problems/super-ugly-number/)

- 解题代码：

  ```c++
  int nthSuperUglyNumber(int n, vector<int>& primes) {
      vector<int> dp(n + 1);
      dp[1] = 1;
      int m = primes.size();
      vector<int> pointers(m, 1);
      for (int i = 2; i <= n; i++) {
          vector<int> nums(m);
          int minNum = INT_MAX;
          for (int j = 0; j < m; j++) {
              nums[j] = dp[pointers[j]] * primes[j];
              minNum = min(minNum, nums[j]);
          }
          dp[i] = minNum;
          for (int j = 0; j < m; j++) {
              if (minNum == nums[j]) {
                  pointers[j]++;
              }
          }
      }
      return dp[n];
  }
  ```

## [等差数列划分 II - 子序列](https://leetcode-cn.com/problems/arithmetic-slices-ii-subsequence/)

- 题目描述

  ![problem446](imgs/algorithm/problem446.png)

- 思路：dp+哈希

  ![solution446](imgs/algorithm/solution446.png)

- 解题代码

  ```c++
  int numberOfArithmeticSlices(vector<int>& nums) {
      int n = nums.size();
      int ret = 0;
      vector<unordered_map<long long, int> > dp(n);
      for(int i = 0; i < n; ++i) {
          for(int j = 0; j < i; ++j) {
              long long diff = 0LL + nums[i] - nums[j];
              auto it = dp[j].find(diff);
              auto count = it == dp[j].end() ? 0 : it->second;
              ret += count;
              dp[i][diff] += count + 1;
          }
      }
      return ret;
  }
  ```

## [交错字符串](https://leetcode-cn.com/problems/interleaving-string/)

- 题目描述

  <img src="imgs/algorithm/problem97.png" alt="problem97" style="zoom:80%;" />

- 解题思路

  <img src="imgs/algorithm/solution97.png" alt="solution97" style="zoom:80%;" />

- 代码实现

  ```c++

  bool isInterleave(string s1, string s2, string s3) {
      auto f = vector < vector <int> > (s1.size() + 1, vector <int> (s2.size() + 1, false));

      int n = s1.size(), m = s2.size(), t = s3.size();

      if (n + m != t) {
          return false;
      }

      f[0][0] = true;
      for (int i = 0; i <= n; ++i) {
          for (int j = 0; j <= m; ++j) {
              int p = i + j - 1;
              if (i > 0) {
                  f[i][j] |= (f[i - 1][j] && s1[i - 1] == s3[p]);
              }
              if (j > 0) {
                  f[i][j] |= (f[i][j - 1] && s2[j - 1] == s3[p]);
              }
          }
      }

      return f[n][m];
  }

  // 滚动数组优化
  bool isInterleave(string s1, string s2, string s3) {
      auto f = vector <int> (s2.size() + 1, false);

      int n = s1.size(), m = s2.size(), t = s3.size();

      if (n + m != t) {
          return false;
      }

      f[0] = true;
      for (int i = 0; i <= n; ++i) {
          for (int j = 0; j <= m; ++j) {
              int p = i + j - 1;
              if (i > 0) {
                  f[j] &= (s1[i - 1] == s3[p]);
              }
              if (j > 0) {
                  f[j] |= (f[j - 1] && s2[j - 1] == s3[p]);
              }
          }
      }

      return f[m];
  }
  ```

## [出界的路径数](https://leetcode-cn.com/problems/out-of-boundary-paths/)

- 题目描述

  <img src="imgs/algorithm/problem576.png" alt="problem576" style="zoom:80%;" />

- 解题思路

  - 记忆化搜索：自上而下

  - 动态规划：自下而上

    <img src="imgs/algorithm/solution576.png" alt="solution576" style="zoom:80%;" />

- 解题代码

  ```c++
  int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
      if(maxMove == 0) {
          return 0;
      }

      vector<pair<int, int> > directions{{1,0}, {-1, 0}, {0, 1}, {0, -1}};
  	// 记录状态
      vector<vector<vector<int>> > rec(maxMove + 1, vector<vector<int> > (m, vector<int>(n, -1)));
      function<int(int,int,int)> dfs = [&](int row, int col, int move) {
          if(row < 0 || row >= m || col < 0 || col >= n) {
              return 1;
          }
          // move为0，还没到达界外，返回0
          if(move == 0) {
              return 0;
          }
          // 已搜索过此状态
          if(rec[move][row][col] != -1) {
              return rec[move][row][col];
          }
          int count = 0;
          for(auto [x, y] : directions) {
              count += dfs(row + x, col + y, move - 1);
              count %= 1000000007;
          }
          // 记录状态
          rec[move][row][col] = count;
          return count;
      };
      return dfs(startRow, startColumn, maxMove);
  }

  // dp
  int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
      constexpr int MOD = 1'000'000'007;
      vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
      int outCounts = 0;
      vector<vector<vector<int>>> dp(maxMove + 1, vector<vector<int>>(m, vector<int>(n)));
      dp[0][startRow][startColumn] = 1;
      for (int i = 0; i < maxMove; i++) {
          for (int j = 0; j < m; j++) {
              for (int k = 0; k < n; k++) {
                  int count = dp[i][j][k];
                  if (count > 0) {
                      for (auto &direction : directions) {
                          int j1 = j + direction[0], k1 = k + direction[1];
                          if (j1 >= 0 && j1 < m && k1 >= 0 && k1 < n) {
                              dp[i + 1][j1][k1] = (dp[i + 1][j1][k1] + count) % MOD;
                          } else {
                              outCounts = (outCounts + count) % MOD;
                          }
                      }
                  }
              }
          }
      }
      return outCounts;
  }
  ```

## [不同的子序列](https://leetcode-cn.com/problems/distinct-subsequences/)

- 题目描述

  <img src="imgs/algorithm/problem115.png" alt="problem115" style="zoom:80%;" />

- 解题思路：[「手画图解」详解两种解法：记忆化递归、动态规划 | 115.不同的子序列 - 不同的子序列 - 力扣（LeetCode） (leetcode-cn.com)](https://leetcode-cn.com/problems/distinct-subsequences/solution/shou-hua-tu-jie-xiang-jie-liang-chong-ji-4r2y/)

- 解题代码

  ```c++
  int numDistinct1(string s, string t) {
      int sLen = s.size(), tLen = t.size();
      if(sLen < tLen) {
          return 0;
      }
  	// 判断极端case越界
      if(sLen == tLen) {
          if(s == t)
              return 1;
          return 0;
      }

      vector<vector<long> > dp(sLen + 1, vector<long>(tLen + 1, 0));
      for(int i = 0; i <= sLen; ++i) {
          dp[i][0] = 1;
      }
      for(int j = 1; j <= tLen; ++j) {
          dp[0][j] = 0;
      }
      for(int i = 1; i <= sLen; ++i) {
          for(int j = 1; j <= tLen; ++j) {
              dp[i][j] = dp[i-1][j];
              if(s[i-1] == t[j-1]) {
                  dp[i][j] += dp[i-1][j-1];
              }
          }
      }
      return dp[sLen][tLen];
  }
  ```

## [学生出勤记录 II](https://leetcode-cn.com/problems/student-attendance-record-ii/)

- 题目描述

  <img src="imgs/algorithm/problem552.png" alt="problem552" style="zoom:80%;" />

- 解题思路

  <img src="imgs/algorithm/solution552.png" alt="solution552" style="zoom:80%;" />

  - 状态机

    <img src="imgs/algorithm/solution552.png" alt="solution552" style="zoom:80%;" />

- 解题代码

  ```c++
  int checkRecord(int n) {
      int mod = 1000000007;
      vector<vector<vector<int> > > dp(n + 1, vector<vector<int>>(2, vector<int>(3)));
      dp[0][0][0] = 1;
      for(int i = 1; i <= n; ++i) {
         // 第i天以p结尾
          for(int j = 0; j <=1; ++j) {
              for(int k = 0; k <= 2; ++k) {
                  dp[i][j][0] = (dp[i][j][0] + dp[i-1][j][k]) % mod;
              }
          }
          // 第i天以A结尾
          for(int k = 0; k <= 2; ++k) {
             dp[i][1][0] = (dp[i][1][0] + dp[i-1][0][k]) % mod;
          }
          // 第i天以L结尾
          for(int j = 0; j <= 1; ++j) {
              for(int k = 0; k <= 1; ++k) {
                  dp[i][j][k + 1] = (dp[i][j][k + 1] + dp[i-1][j][k]) % mod;
              }
          }
      }
      int ret = 0;
      for(int j = 0; j <= 1; ++j) {
          for(int k = 0; k <= 2; ++k) {
              ret = (ret + dp[n][j][k]) % mod;
          }
      }
      return ret;
  }

  // 空间优化
  int checkRecord(int n) {
      constexpr int MOD = 1'000'000'007;
      int dp[2][3];  // A 的数量，结尾连续 L 的数量
      memset(dp, 0, sizeof(dp));
      dp[0][0] = 1;
      for (int i = 1; i <= n; i++) {
          int dpNew[2][3];  // A 的数量，结尾连续 L 的数量
          memset(dpNew, 0, sizeof(dpNew));
          // 以 P 结尾的数量
          for (int j = 0; j <= 1; j++) {
              for (int k = 0; k <= 2; k++) {
                  dpNew[j][0] = (dpNew[j][0] + dp[j][k]) % MOD;
              }
          }
          // 以 A 结尾的数量
          for (int k = 0; k <= 2; k++) {
              dpNew[1][0] = (dpNew[1][0] + dp[0][k]) % MOD;
          }
          // 以 L 结尾的数量
          for (int j = 0; j <= 1; j++) {
              for (int k = 1; k <= 2; k++) {
                  dpNew[j][k] = (dpNew[j][k] + dp[j][k - 1]) % MOD;
              }
          }
          memcpy(dp, dpNew, sizeof(dp));
      }
      int sum = 0;
      for (int j = 0; j <= 1; j++) {
          for (int k = 0; k <= 2; k++) {
              sum = (sum + dp[j][k]) % MOD;
          }
      }
      return sum;
  }
  ```

## [摆动序列](https://leetcode-cn.com/problems/wiggle-subsequence/)
