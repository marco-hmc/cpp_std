
# 滑动窗口

## 最小覆盖子串

- 给出两个字符串$S$和$T$，要求在$\textbf{O}(n)$的时间复杂度内在中$S$找出最短的包含 $T$中所有字符的子串。例如：

  $S="ADOBECODEBANC"$ $T="ABC"$ 找出的最短子串为$BANC$

  注意：如果$S$中没有包含 $T$中所有字符的子串，返回空字符串 “”；满足条件的子串可能有很多，但是题目保证满足条件的最短的子串唯一

- 思路：可以用滑动窗口的思想解决这个问题。在滑动窗口类型的问题中都会有两个指针，一个用于「延伸」现有窗口的`right`指针，和一个用于「收缩」窗口的`left`指针。在任意时刻，只有一个指针运动，而另一个保持静止。我们在$S$上滑动窗口，通过移动`right`指针不断扩张窗口。当窗口包含`T`全部所需的字符后，如果能收缩，我们就收缩窗口直到得到最小窗口

- 优化：其实在$S$ 中，有的字符我们是不关心的，我们只关心$T$中出现的字符，我们可以先预处理 $S$，扔掉那些$T$中没有出现的字符，然后再做滑动窗口

- 代码

  ```c++
  string minWindow(string S, string T) {
      unordered_map<char, int> window, target;
      int left = 0, right =0, n=S.size();
      int start = 0, count = 0;
      int minLen = INT_MAX;
      for(auto & ch : T)
          ++target[ch];
      while(right<n){
          char ch = S[right];
          ++right;
          if(target.count(ch)>0) {  // 优化，仅统计T中出现的字符
              ++window[ch];
              if(window[ch] == target[ch])
                  ++count;
              while(count == int(target.size())) {
                  if(right-left < minLen) {
                      start = left;
                      minLen = right-left;
                  }
                  ch = S[left];
                  ++left;		// 右移缩小窗口
                  if(target.count(ch) > 0) {	// 当前left所指元素在T中，减1
                      if(window[ch] == target[ch])
                          --count;
                      --window[ch];
                  }
              }
          }
      }
      return minLen == INT_MAX?"":S.substr(start, minLen);
  }
  ```

## [长度最小的子数组](https://leetcode-cn.com/problems/minimum-size-subarray-sum/)

- 代码实现

  ```c++
  int minSubArrayLen(int target, vector<int>& nums) {
      int n = nums.size();
      int len = INT_MAX;
      int sum = 0;
      int left = 0, right = 0;
      while(right < n) {
          sum += nums[right];	// 前缀和
          while(sum >= target) {	// 缩小窗口，左指针右移
              len = min(len, right - left + 1);
              sum -= nums[left];
              ++left;
          }
          ++right;
      }
      return len == INT_MAX ? 0 : len;
  }
  ```

## [无重复字符的最长子串](https://leetcode-cn.com/problems/longest-substring-without-repeating-characters/)

- 实现代码

  ```c++
  int lengthOfLongestSubstring(string s) {
      if(s.empty()) {
          return 0;
      }
      // 记录前面出现的字符，值为对应下标
      unordered_map<char, int> record;
      int maxCount = 1;

      int left = 0, right = 0;
      int n = s.size();
      while(right < n) {
          auto ch = s[right];
          // 出现重复，统计前面left到right的长度，并更新left为重复字符前面坐标+1
          if(record.count(ch) > 0 && record[ch] >= left) {
              maxCount = max(maxCount, right - left);
              left = record[ch] + 1;	// 跳过重复字符位置前面所有字符
          }
          record[ch] = right;	// 插入/更新ch的位置
          ++right;
      }
      // 最后可能全部都不重复，统计最后一次
      maxCount = max(maxCount, right - left);
      return maxCount;
  }
  ```

## [串联所有单词的子串](https://leetcode-cn.com/problems/substring-with-concatenation-of-all-words/)

```c++
vector<int> findSubstring(string s, vector<string>& words) {
    int n = s.size();
    int wordLen = words[0].size();
    int wordNum = words.size();
    unordered_map<string, int> wordRec;
    for(auto &str:words) {
        ++wordRec[str];
    }
    vector<int> ret;
    int seqLen = wordLen * wordNum;
    // 多起点滑动窗口
    for(int k = 0; k < wordLen; ++k) {
        for(int i = k; i <= n - seqLen; i += wordLen) {
            unordered_map<string, int> rec;
            int j = i;
            int count = 0;
            while(j < i + seqLen) {
                string word = s.substr(j, wordLen);
                if(wordRec.find(word) == wordRec.end()) {
                    break;
                }
                ++rec[word];
                if(rec[word] == wordRec[word]) {
                    ++count;
                }
                else if(rec[word] > wordRec[word]) {
                    --count;
                    break;
                }
                j += wordLen;
            }
            if(count == wordRec.size()) {
                ret.push_back(i);
            }
        }
    }
    return ret;
}
```

## [可获得的最大点数](https://leetcode-cn.com/problems/maximum-points-you-can-obtain-from-cards/)

- 题目描述

  <img src="imgs/algorithm/problem1423.png" alt="problem1423" style="zoom:80%;" />

- 解题思路：

  - 滑动窗口：每次从开头或末尾拿一张卡牌，取 k 次，使点数和最大，相当于使中间剩余连续的 n-k 张卡牌点数之和最小，通过固定窗口大小，滑动过程统计最小和
  - 前缀和：问题转换为从开头取 i(i<=k)张卡牌，末尾取 k-i 张卡牌，使 sum[i] + sum[n] - sum[n-k-i]最大

- 代码实现

  ```c++
  // 滑动窗口
  int maxScore(vector<int>& cardPoints, int k) {
      int n = cardPoints.size();
      // 滑动窗口大小为 n-k
      int windowSize = n - k;
      // 选前 n-k 个作为初始值
      int sum = accumulate(cardPoints.begin(), cardPoints.begin() + windowSize, 0);
      int minSum = sum;
      for (int i = windowSize; i < n; ++i) {
          // 滑动窗口每向右移动一格，增加从右侧进入窗口的元素值，并减少从左侧离开窗口的元素值
          sum += cardPoints[i] - cardPoints[i - windowSize];
          minSum = min(minSum, sum);
      }
      return accumulate(cardPoints.begin(), cardPoints.end(), 0) - minSum;
  }

  // 前缀和
  int maxScore(vector<int>& cardPoints, int k) {
      int n = cardPoints.size();
      for(int i = 1; i < n; ++i) {
          cardPoints[i] += cardPoints[i-1];
      }
      if(n == k) {
          return cardPoints[n-1];
      }
      // 将maxSum初始化为i=0的值
      int maxSum = cardPoints[n-1] - cardPoints[n-1-k];
      // i 为 1...k
      for(int i = 1; i <= k; ++i) {
          int sum = cardPoints[i-1] + cardPoints[n-1] - cardPoints[n-1+i-k];
          maxSum = max(maxSum, sum);
      }
      return maxSum;
  }
  ```

