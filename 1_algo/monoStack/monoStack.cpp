
#单调栈

## [移掉 K 位数字](https://leetcode-cn.com/problems/remove-k-digits/)

- 题目描述：给定一个以字符串表示的非负整数 num，移除这个数中的 k 位数字，使得剩下的数字最小

> 注意:
>
> num 的长度小于 10002 且 ≥ k。 num 不会包含任何前导零。
>
> 示例 1 :
>
> 输入: num = "1432219", k = 3 输出: "1219" 解释: 移除掉三个数字 4, 3, 和 2 形成一个新的最小的数字 1219。

- 解题思路：

  - 以题目中的 `num = 1432219， k = 3` 为例，我们需要返回一个长度为 4 的字符串，问题在于： 我们怎么才能求出这四个位置依次是什么呢？
  - 暴力法的话，我们需要枚举$C_n^{n - k}$ 种序列（其中 n 为数字长度），并逐个比较最大。这个时间复杂度是指数级别的，必须进行优化
  - 一个思路是利用数学知识
    - 对于两个数 `123a456` 和 `123b456`，如果 `a > b`， 那么数字 `123a456` 大于 数字 `123b456`，否则数字 `123a456` 小于等于数字 `123b456`。也就说，**两个相同位数的数字大小关系取决于第一个不同的数的大小**
    - 从左到右遍历
    - 对于每一个遍历到的元素，决定是**丢弃**还是**保留**
    - **遍历过程中，如果左边数字比当前数字小，则不能丢弃左边数字，否则导致数字变大**
  - 以`1432219`为例，于是得到解题流程为
    - 当前数字为 1，左边没有元素，保留
    - 当前数字为 4，左边为 1，保留左边元素
    - 以此类推
  - 然而需要注意的是，**如果给定的数字是一个单调递增的数字，那么我们的算法会永远选择不丢弃**。这与题目中要求的，我们要永远确保丢弃 k 个矛盾
  - 一个简单的思路就是：
    - 每次丢弃一次，k 减去 1。当 k 减到 0 ，我们可以提前终止遍历
    - **而当遍历完成，如果 k 仍然大于 0。不妨假设最终还剩下 x 个需要丢弃，那么我们需要选择删除末尾 x 个元素**
    - **刚才我们的关注点一直是丢弃，题目要求我们丢弃 k 个。反过来说，就是让我们保留 n - k 个元素，其中 n 为数字长度。 那么我们只需要按照上面的方法遍历完成之后，再截取前 n - k 个元素即可**

- 代码实现

  ```c++
  string removeKdigits(string num, int k) {
    if (k <= 0) return num;
    string ret;
    for (auto& ch : num) {
        while (k > 0 && !ret.empty() && ch < ret.back()) {
            // 保持ret中元素从栈底到栈顶递增
            ret.pop_back();
            --k;  // 剩余应丢弃数-1
        }
        ret.push_back(ch);
    }
    while (k-- > 0 &&
           !ret.empty())  // 清除尾部多余数字，并防止k>num.size()的情况
        ret.pop_back();
    int idx = 0;
    while (idx < ret.size() && ret[idx] == '0')  // 清除头部的'0'
        ++idx;
    ret = ret.substr(idx);
    return ret.empty() ? "0" : ret;
  }
  ```

## [去除重复字母](https://leetcode-cn.com/problems/remove-duplicate-letters/)

- 题目描述：给你一个仅包含小写字母的字符串，请你去除字符串中重复的字母，使得每个字母只出现一次。需保证返回结果的字典序最小（要求不能打乱其他字符的相对位置）

> 示例 1:
>
> 输入: "bcabc" 输出: "abc" 示例 2:
>
> 输入: "cbacdcbc" 输出: "acdb"

- 解题思路

  - 与上面题目不同，这道题没有一个全局的删除次数 k。而是对于每一个在字符串 s 中出现的字母 c 都有一个 k 值。这个 k 是 c 出现次数 - 1
  - 沿用上面的知识的话，我们首先要做的就是计算每一个字符的 k，可以用一个字典来描述这种关系，**其中 key 为 字符 c，value 为其出现的次数**
  - 具体算法：
    - **建立一个字典。其中 key 为 字符 c，value 为其出现的剩余次数**
    - 从左往右遍历字符串，**每次遍历到一个字符，其剩余出现次数 - 1**
    - 对于每一个字符，**如果其对应的剩余出现次数大于 1，我们可以选择丢弃（也可以选择不丢弃），否则不可以丢弃**
    - 是否**丢弃的标准和上面题目**类似。遍历过程中，如果**栈顶元素字典序比当前元素大且该元素剩余次数大于 1，那么我们选择丢弃该栈顶元素**
    - 由于题目是要求字母出现一次，我们可以在遍历的时候简单地判断其是否在栈上即可

- 代码实现

  ```c++
  string removeDuplicateLetters(string s) {
    unordered_map<char, int> remainder,
        visited;  // 记录剩余字母数和字母是否在栈中
    for (auto& ch : s) {
        remainder[ch]++;  // 统计剩余数
    }
    string ret;
    for (auto& ch : s) {
        if (visited[ch] == 0) {  // 当前字母不在栈中，可能可以入栈
            // 栈顶元素大于当前元素(逆序)，且栈顶元素剩余数大于0，则出栈。出栈将visited置0
            while (!ret.empty() && ch < ret.back() &&
                   remainder[ret.back()] > 0) {
                visited[ret.back()] = 0;
                ret.pop_back();
            }
            visited[ch] = 1;  // 入栈将visited置1
            ret.push_back(ch);
        }
        remainder[ch]--;  // 每遍历一次，剩余数减一
    }
    return ret;
  }
  ```

## [ 拼接最大数](https://leetcode-cn.com/problems/create-maximum-number/)

> 给定长度分别为 m 和 n 的两个数组，其元素由 0-9 构成，表示两个自然数各位上的数字。现在从这两个数组中选出 k (k <= m + n) 个数字拼接成一个新的数，要求从同一个数组中取出的数字保持其在原数组中的相对顺序。
>
> 求满足该条件的最大数。结果返回一个表示该最大数的长度为 k 的数组。

- 解题思路

  - 为了找到长度为 k 的最大数，需要从两个数组中分别选出最大的子序列，这两个子序列的长度之和为 k，然后将这两个子序列合并得到最大数。两个子序列的长度最小为 0，最大不能超过 k 且不能超过对应的数组长度
  - 令数组 $\textit{
    nums}1$的长度为 m，数组 $\textit{
    nums}2$的长度为 n，则需要从数组$ \textit{
    nums}1$中选出长度为 x 的子序列，以及从数组$ \textit{
    nums}2$ 中选出长度为 y 的子序列，其中 x+y=k，且满足 $0 \le x \le m$ 和 $0 \le y \le n$。**需要遍历所有可能的 x 和 y 的值**，**对于每一组 x 和 y 的值，得到最大数**。在整个过程中维护可以通过拼接得到的最大数
  - 对于每一组 x 和 y 的值，得到最大数的过程分成两步，**第一步是分别从两个数组中得到指定长度的最大子序列，第二步是将两个最大子序列合并**
  - **第一步可以通过单调栈实现**。单调栈满足**从栈底到栈顶的元素单调递减，从左到右遍历数组，遍历过程中维护单调栈内的元素**，需要**保证遍历结束之后单调栈内的元素个数等于指定的最大子序列的长度**。遍历结束之后，将从栈底到栈顶的元素依次拼接，即得到最大子序列
  - **第二步需要自定义比较方法**。首先比较两个子序列的当前元素，**如果两个当前元素不同，则选其中较大的元素作为下一个合并的元素**，**否则需要比较后面的所有元素才能决定选哪个元素作为下一个合并的元素**

- 代码实现

  ```c++
  vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
    int len1 = nums1.size();
    int len2 = nums2.size();
    if (k <= 0 || k > len1 + len2) return {};
    vector<int> ret(k, 0);
    int len = max(0, k - len2), maxlen = min(len1, k);
    // 拆分子串
    for (; len <= maxlen; len++) {  // 暴力枚举所有可能
        auto subvec1 = std::move(getMaxSub(nums1, len));
        auto subvec2 = std::move(getMaxSub(nums2, k - len));
        auto subvec = std::move(merge(subvec1, subvec2));
        if (comp(subvec, 0, ret, 0) > 0) {
            ret.swap(subvec);
        }
    }
    return ret;
  }
  // 与“移除k个数字”算法相同
  vector<int> getMaxSub(vector<int> & vec, int k) {
    if (k == 0) return {};
    vector<int> ret;
    int n = vec.size();
    int remainder = n - k;
    for (auto& num : vec) {
        while (!ret.empty() && num > ret.back() && remainder > 0) {
            ret.pop_back();
            remainder--;
        }
        if (ret.size() < k) {
            ret.push_back(num);
        } else
            remainder--;
    }
    return ret;
  }
  // 根据comp比较两个数大小的规则，合并两个数
  vector<int> merge(vector<int> & vec1, vector<int> & vec2) {
    int m = vec1.size(), n = vec2.size();
    int idx1 = 0, idx2 = 0;
    vector<int> ret(m + n);
    for (int i = 0; i < m + n; i++) {
        if (comp(vec1, idx1, vec2, idx2) > 0) {
            ret[i] = vec1[idx1++];
        } else {
            ret[i] = vec2[idx2++];
        }
    }
    return ret;
  }
  // 比较两个数大小，比较规则：先比较两个数从左到右首个不相同的数字，若全相同，位数多的大
  int comp(vector<int> & vec1, int idx1, vector<int> & vec2, int idx2) {
    int m = vec1.size(), n = vec2.size();
    while (idx1 < m && idx2 < n) {
        int diff = vec1[idx1++] - vec2[idx2++];
        if (diff != 0) {
            return diff;
        }
    }
    return (m - idx1) - (n - idx2);
  }
  ```

## [132 模式](https://leetcode-cn.com/problems/132-pattern/)

- 题目描述

  <img src="imgs/algorithm/problem456.png" alt="problem456" style="zoom:80%;" />

- 解题思路：

  - 三个下标未定，枚举三个下标，复杂度 O(n^3)可能会超时
  - 可以枚举 1，2，3 的方式，使用数据结构维护寻找另两个数

- 解题代码

  ```c++
  // 枚举2
  bool find132pattern(vector<int>& nums) {
    int n = nums.size();
    if (n < 3) {
        return false;
    }
    int minI = nums[0];
    multiset<int> rightK;
    for (int k = 2; k < n; ++k) {
        rightK.insert(nums[k]);
    }
    // 枚举2，j从1开始
    for (int j = 1; j < n - 1; ++j) {
        // 满足13，从右边找2
        if (nums[j] > minI) {
            auto it = rightK.upper_bound(minI);
            if (it != rightK.end() && *it < nums[j]) {
                return true;
            }
        }
        // 更新minI，删除不合题意的k
        minI = min(minI, nums[j]);
        rightK.erase(rightK.find(nums[j + 1]));
    }
    return false;
  }

  // 从后往前，枚举1
  bool find132pattern(vector<int>& nums) {
    int n = nums.size();
    stack<int> stk;
    int maxK = INT_MIN;
    stk.push(nums[n - 1]);
    for (int i = n - 2; i >= 0; --i) {
        // nums[i] 作为 1
        if (nums[i] < maxK) {
            return true;
        }
        // 维护 i...n-1的单调递减
        while (!stk.empty() && nums[i] > stk.top()) {
            maxK = stk.top();  // 记录i右侧能充当2的最大值，nums[i]可作为3
            stk.pop();
        }
        // 如果nums[i]<=maxK，可以不用进栈，因为未来被弹出也不会将maxK更新为更大的值
        if (nums[i] > maxK) {
            stk.push(nums[i]);
        }
    }
    return false;
  }
  ```
