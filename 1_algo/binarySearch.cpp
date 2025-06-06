
# 二分查找

## [寻找两个正序数组的中位数](https://leetcode-cn.com/problems/median-of-two-sorted-arrays/)

- 题目描述：给定两个大小分别为 `m` 和 `n` 的正序（从小到大）数组 `nums1` 和 `nums2`。请你找出并返回这两个正序数组的 **中位数**

- 代码实现

  ```c++
  int getKthElement(const vector<int>& nums1, const vector<int>& nums2, int k) {
      /* 主要思路：要找到第 k (k>1) 小的元素，那么就取 pivot1 = nums1[k/2-1] 和 pivot2 = nums2[k/2-1] 进行比较
       * 这里的 "/" 表示整除
       * nums1 中小于等于 pivot1 的元素有 nums1[0 .. k/2-2] 共计 k/2-1 个
       * nums2 中小于等于 pivot2 的元素有 nums2[0 .. k/2-2] 共计 k/2-1 个
       * 取 pivot = min(pivot1, pivot2)，两个数组中小于等于 pivot 的元素共计不会超过 (k/2-1) + (k/2-1) <= k-2 个
       * 这样 pivot 本身最大也只能是第 k-1 小的元素
       * 如果 pivot = pivot1，那么 nums1[0 .. k/2-1] 都不可能是第 k 小的元素。把这些元素全部 "删除"，剩下的作为新的 nums1 数组
       * 如果 pivot = pivot2，那么 nums2[0 .. k/2-1] 都不可能是第 k 小的元素。把这些元素全部 "删除"，剩下的作为新的 nums2 数组
       * 由于我们 "删除" 了一些元素（这些元素都比第 k 小的元素要小），因此需要修改 k 的值，减去删除的数的个数
       */

      int m = nums1.size();
      int n = nums2.size();
      int index1 = 0, index2 = 0;

      while (true) {
          // 边界情况
          if (index1 == m) {
              return nums2[index2 + k - 1];
          }
          if (index2 == n) {
              return nums1[index1 + k - 1];
          }
          if (k == 1) {
              return min(nums1[index1], nums2[index2]);
          }

          // 正常情况
          int newIndex1 = min(index1 + k / 2 - 1, m - 1);
          int newIndex2 = min(index2 + k / 2 - 1, n - 1);
          int pivot1 = nums1[newIndex1];
          int pivot2 = nums2[newIndex2];
          if (pivot1 <= pivot2) {
              k -= newIndex1 - index1 + 1;	// k必须这样更新，因为newIdx可能会越界
              index1 = newIndex1 + 1;
          }
          else {
              k -= newIndex2 - index2 + 1;
              index2 = newIndex2 + 1;
          }
      }
  }

  double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
      int totalLength = nums1.size() + nums2.size();
      if (totalLength % 2 == 1) {
          return getKthElement(nums1, nums2, (totalLength + 1) / 2);
      }
      else {
          return (getKthElement(nums1, nums2, totalLength / 2) \
                  + getKthElement(nums1, nums2, totalLength / 2 + 1)) / 2.0;
      }
  }
  ```

## [搜索旋转排序数组](https://leetcode-cn.com/problems/search-in-rotated-sorted-array/)

- 二分查找，分类讨论比较复杂

## [绝对差值和](https://leetcode-cn.com/problems/minimum-absolute-sum-difference/)

- 题目描述

  <img src="imgs/algorithm/problem1818.png" alt="problem1818" style="zoom: 80%;" />

- 思路

  <img src="imgs/algorithm/solution7.png" alt="solution7" style="zoom: 80%;" />

- 代码实现

  ```c++
  int mod = 1e9 + 7;
  int minAbsoluteSumDiff(vector<int>& nums1, vector<int>& nums2) {
      int n = nums1.size();
      vector<int> record(nums1);
      sort(record.begin(), record.end());
      int maxVal = INT_MIN, ret = 0;
      for(int i = 0; i < n; ++i) {
          int diff = abs(nums1[i] - nums2[i]);
          int idx = lower_bound(record.begin(), record.end(), nums2[i]) - record.begin();
          if(idx < n) {
              maxVal = max(maxVal, diff - abs(record[idx] - nums2[i]));
          }
          if(idx > 0) {
              maxVal = max(maxVal, diff - abs(record[idx - 1] - nums2[i]));
          }
          ret = (ret + diff) % mod;
      }
      ret = (ret - maxVal + mod) % mod;
      return ret;
  }
  ```

