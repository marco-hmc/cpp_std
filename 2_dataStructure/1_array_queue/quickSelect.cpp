# 快速选择(Quick Select)

## [数组中的第 K 个最大元素](https://leetcode-cn.com/problems/kth-largest-element-in-an-array/)

<img src="imgs/algorithm/k-th.jpg" alt="k-th" style="zoom:80%;" />

- 代码实现

  ```c++
  int findKthLargest(vector<int>& nums, int k) {
      int n = nums.size();
      int left = 0, right = n-1;
      int ret = 0;
      int target = n - k;
      while(left <= right) {
          int idx = patition(nums, left, right);
          if(idx == target) {
              return nums[target];
          }
          else if(idx < target) {
              left = idx + 1;
          }
          else {
              right = idx - 1;
          }
      }
      return ret;
  }

  int patition(vector<int> & nums, int left, int right){
      int pivot = nums[left];
      while(left<right) {
          while(left<right && nums[right] >= pivot)
              right--;
          nums[left] = nums[right];
          while(left<right && nums[left]<=pivot)
              left++;
          nums[right] = nums[left];
      }
      nums[left] = pivot;
      return left;
  }
  ```
