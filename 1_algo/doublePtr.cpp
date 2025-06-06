
#双指针

##** 下一个排列* *

        -题目描述：实现获取 下一个排列
            的函数，算法需要将给定数字序列重新排列成字典序中下一个更大的排列。如果不存在下一个更大的排列，则将数字重新排列成最小的排列（即升序排列）。必须原地修改，只允许使用额外常数空间。

    -
    思路：注意到下一个排列总是比当前排列要大，除非该排列已经是最大的排列。我们希望找到一种方法，能够找到一个大于当前序列的新序列，且变大的幅度尽可能小。具体地：

    -
    我们需要将一个** 左边的「较小数」与一个右边的「较大数」交换**，以能够让当前排列变大，从而得到下一个排列 -
    同时我们** 要让这个「较小数」尽量靠右，而「较大数」尽可能小**。当交换完成后，**「较大数」右边的数需要按照升序重新排列**。这样可以在保证新排列大于原来排列的情况下，** 使变大的幅度尽可能小* *
        -以排列 \[4, 5, 2, 6, 3, 1] 为例：

    -
    我们能找到的符合条件的一对「较小数」与「较大数」的组合为 2 与 3，满足「较小数」尽量靠右，而「较大数」尽可能小 -
    当我们完成交换后排列变为 \
        [4, 5, 3, 6, 2,
         1]，此时我们可以** 重排「较小数」右边的序列**，序列变为 \[4, 5, 3, 1,
                                                                   2, 6]

    - 代码实现

  ```c++ void nextPermutation(vector<int>& nums) {
    if (nums.size() <= 1) {
        return;
    }
    int n = nums.size();
    int smaller = n - 2;
    while (smaller >= 0 && nums[smaller] >= nums[smaller + 1]) {
        --smaller;
    }
    if (smaller >= 0) {
        int bigger = n - 1;
        while (bigger >= 0 && nums[bigger] <= nums[smaller]) {
            --bigger;
        }
        swap(nums[smaller], nums[bigger]);
    }
    reverse(nums.begin() + smaller + 1, nums.end());
}
  ```

## [三数之和](https://leetcode-cn.com/problems/3sum/)

- 代码实现

  ```c++
  vector<vector<int>> threeSum(vector<int>& nums) {
    if (nums.empty()) {
        return {};
    }
    vector<vector<int>> ret;
    int len = nums.size();
    sort(nums.begin(), nums.end());
    for (int i = 0; i < len; ++i) {  // 最小大于0，三数相加必大于0
        if (nums[i] > 0) {
            break;
        }
        // 去重
        if (i > 0 && nums[i] == nums[i - 1]) {
            continue;
        }
        int target = 0 - nums[i];  // 双指针右指针，往左移动
        int k = len - 1;
        for (int j = i + 1; j < len; ++j) {
            // 去重
            if (j > i + 1 && nums[j] == nums[j - 1]) {
                continue;
            }
            // 指针往左移动，直到满足条件，或者不存在三数组合
            while (k > j && nums[j] + nums[k] > target) {
                --k;
            }
            // 没有满足条件的三数，跳出本次循环
            if (k == j) {
                break;
            }
            // 满足条件
            if (nums[j] + nums[k] == target) {
                ret.push_back({nums[i], nums[j], nums[k]});
            }
        }
    }
    return ret;
  }
  ```

## [接雨水](https://leetcode-cn.com/problems/trapping-rain-water/)

- 本题可以用双指针和单调栈做，单调栈横向统计，而双指针则纵向统计。

- 代码实现：

  ```c++
  // 单调栈解法
  int trap(vector<int>& height) {
    int len = height.size();
    if (len <= 2) {
        return 0;
    }
    stack<int> idxs;
    int volume = 0;
    for (int i = 0; i < len; ++i) {
        // 当前值大于栈顶，出栈
        while (!idxs.empty() && height[i] > height[idxs.top()]) {
            // 低水位
            int low = height[idxs.top()];
            idxs.pop();
            // 出栈后，栈空，不能储水
            if (idxs.empty()) {
                break;
            }
            // 计算与低水位差值，为储水矩形的高
            int h = min(height[idxs.top()], height[i]) - low;
            // 储水矩形的宽
            int width = i - idxs.top() - 1;
            volume += h * width;
        }
        idxs.push(i);
    }
    return volume;
  }
   // 双指针：统计当前遍历柱子是否能储水，即左右两边同时有没有更高的柱子，有就能储水
  int trap(vector<int> & height) {
    int len = height.size();
    if (len <= 2) {
        return 0;
    }
    int volume = 0;
    int leftMax = 0, rightMax = 0;
    int left = 0, right = len - 1;
    while (left < right) {
        // 记录左边最高柱子高度
        leftMax = max(leftMax, height[left]);
        // 记录右边最高柱子高度
        rightMax = max(rightMax, height[right]);
        // 当前遍历左柱子小于右柱子，leftMax>=height[left]都满足下式
        if (height[left] < height[right]) {
            volume += leftMax - height[left];  // >= 0
            ++left;
        } else {  // 当前遍历左柱子大于等于右柱子，rightMax>=height[right]都满足下式
            volume += rightMax - height[right];  // >= 0
            --right;
        }
    }
    return volume;
  }
  ```

## [至少有 K 个重复字符的最长子串](https://leetcode-cn.com/problems/longest-substring-with-at-least-k-repeating-characters/)

- 思路

  <img src="imgs/algorithm/solution3.png" alt="solution3" style="zoom:67%;" />

  <img src="imgs/algorithm/solution4.png" alt="solution4" style="zoom:67%;" />

- 代码实现

  ```c++
  int longestSubstring(string s, int k) {
    int n = s.size();
    int maxLen =
        0;  // 枚举字符类型数目，限定滑动窗口中能出现的最大字符类型数，数目为1到26
    // 在每个字符类型数目限定下找到局部最优，更新全局最优
    for (int alphaLimit = 1; alphaLimit <= min(n, 26); ++alphaLimit) {
        vector<int> alpha(26, 0);  // 统计窗口中字符类型和数目
        int left = 0, right = 0;   // 滑动窗口初始值
        int total = 0;             // 窗口总字符类型数量
        int satisfied = 0;  // 窗口中满足数量>=k的字符类型数目
        while (right < n) {
            int idx = s[right] - 'a';
            // 向右移动，计数+1
            ++alpha[idx];
            // 若+1后，数目为1，为新增字符，total+1
            if (alpha[idx] == 1) {
                ++total;
            }
            // 若+1后，数目为k，为新的满足条件的字符，satisfied + 1
            if (alpha[idx] == k) {
                ++satisfied;
            }
            // 在字符数约束下，左移left，使满足total <= alphaLimit
            while (total > alphaLimit) {
                idx = s[left++] - 'a';
                --alpha[idx];
                // 从k减为k - 1, satisfied - 1
                if (alpha[idx] == k - 1) {
                    --satisfied;
                }
                // 减为0，total - 1
                if (alpha[idx] == 0) {
                    --total;
                }
            }
            // 在满足字符约束，且满足每种字符数量都满足>=k情况下，更新长度
            if (satisfied == total) {
                maxLen = max(maxLen, right - left + 1);
            }
            ++right;
        }
    }
    return maxLen;
  }
  ```

## [最小覆盖子串](https://leetcode-cn.com/problems/minimum-window-substring/)

- 给你一个字符串 `s` 、一个字符串 `t` 。返回 `s` 中涵盖 `t` 所有字符的最小子串。如果 `s` 中不存在涵盖 `t` 所有字符的子串，则返回空字符串 `""`

  - **注意：**
    - 对于 `t` 中重复字符，我们寻找的子字符串中该字符数量必须不少于 `t` 中该字符数量
    - 如果 `s` 中存在这样的子串，我们保证它是唯一的答案

- 代码实现

  ```c++
  string minWindow(string s, string t) {
    unordered_map<char, int> window, target;
    int n = s.size();
    // 记录t中所有字符，用于快速查询
    for (auto& ch : t) {
        ++target[ch];
    }
    int left = 0, right = 0, minLen = INT_MAX;
    int count = 0;  // 统计窗口中有多少在t中的字符
    int start = 0;  // 记录字串起点
    while (right < n) {
        char ch = s[right];
        right++;
        if (target.count(ch) > 0) {  // 在t中出现
            window[ch]++;            // 当前窗口中统计出现次数+1
            if (target[ch] ==
                window[ch]) {  // 当前窗口满足t中ch字符数量，count+1
                ++count;
            }
            // left右移，缩减窗口大小，同时统计最小长度和起点
            while (count == target.size()) {
                if (right - left < minLen) {
                    minLen = right - left;
                    start = left;
                }
                --window[s[left]];  // 将s[left]移出window
                // s[left]移除后，不满足window中s[left]大于等于t中s[left]数目
                if (target.count(s[left]) > 0 &&
                    window[s[left]] < target[s[left]]) {
                    --count;
                }
                ++left;
            }
        }
    }
    return minLen == INT_MAX ? "" : s.substr(start, minLen);
  }
  ```

## [环形数组是否存在循环](https://leetcode-cn.com/problems/circular-array-loop/)

## [快乐数](https://leetcode-cn.com/problems/happy-number/)

- 题目描述

  <img src="imgs/algorithm/problem202.png" alt="problem202" style="zoom:67%;" />

- 快慢指针判环

  ```c++
  bool isHappy(int n) {
    auto getNext = [](int num) -> int {
        int ret = 0;
        while (num != 0) {
            int tmp = num % 10;
            ret += tmp * tmp;
            num /= 10;
        }
        return ret;
    };

    int slow = getNext(n), fast = getNext(slow);
    while (fast != 1 && slow != fast) {
        slow = getNext(slow);
        fast = getNext(getNext(fast));
    }
    return fast == 1;
  }
  ```

## [等差数列划分](https://leetcode-cn.com/problems/arithmetic-slices/)

- 解题代码

  ```c++
  int numberOfArithmeticSlices(vector<int>& nums) {
    int len = nums.size();
    if (len < 3) {
        return 0;
    }
    // left记录起点，right记录终点
    int left = 0, right = 1;
    int ret = 0;
    int diff = nums[1] - nums[0];
    while (right < len) {
        // 不满足等差数组条件，统计left...right等差数列数
        if (nums[right] - nums[right - 1] != diff) {
            // 长度大于等于3才记录
            if (right - left >= 3) {
                int tmpLen = right - left;
                int count = (2 + tmpLen - 1) * (tmpLen - 2) / 2;
                ret += tmpLen * (tmpLen - 2) - count;
            }
            // 更新diff和left
            left = right - 1;
            diff = nums[right] - nums[left];
        }
        ++right;
    }
    // 最后可能right==len，长度大于3，则记录
    if (right - left >= 3) {
        int tmpLen = right - left;
        int count = (2 + tmpLen - 1) * (tmpLen - 2) / 2;
        ret += tmpLen * (tmpLen - 2) - count;
    }
    return ret;
  }

  // dp
  // 思路：dp[i]为以nums[i]结尾的等差数列个数
  // 若nums[i] - nums[i-1]==nums[i-1] - nums[i-2], dp[i] = dp[i-1] + 1
  // 表示nums[i-1]结尾的等差数列都可以以nums[i]结尾，并且加+1(nums[i-2],nums[i-1],nums[i])
  // 否则dp[i] = 0。最终结果为dp数组的和
  // 优化dp[i]只与dp[i-1]有关，可以使用一个变量记录之前状态
  int numberOfArithmeticSlices(vector<int>& nums) {
    int len = nums.size();
    if (len < 3) {
        return 0;
    }
    int diff = nums[1] - nums[0];
    int pre = 0;
    int ret = 0;
    for (int i = 2; i < len; ++i) {
        if (nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]) {
            ++pre;
        } else {
            pre = 0;
            diff = nums[i] - nums[i - 1];
        }
        ret += pre;
    }
    return ret;
  }
  ```
