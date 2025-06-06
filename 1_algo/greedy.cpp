
# 贪心算法

## [跳跃游戏 II](https://leetcode-cn.com/problems/jump-game-ii/)

- 反向查找出发位置

  - 我们的目标是到达数组的最后一个位置，因此我们可以考虑最后一步跳跃前所在的位置，该位置通过跳跃能够到达最后一个位置

  - 如果有多个位置通过跳跃都能够到达最后一个位置，那么我们应该如何进行选择呢？直观上来看，我们可以「贪心」地选择距离最后一个位置最远的那个位置，也就是对应下标最小的那个位置。因此，我们可以从左到右遍历数组，选择第一个满足要求的位置

  - 找到最后一步跳跃前所在的位置之后，我们继续贪心地寻找倒数第二步跳跃前所在的位置，以此类推，直到找到数组的开始位置

    ```c++
    // On2解法
    int jump(vector<int>& nums) {
        int n = nums.size();
        int count = 0;
        int pos = n - 1;
        while(pos > 0) {
            // 从左到右找到第一个能到达pos的位置(离pos最远)
            for(int i = 0; i < pos; ++i) {
                if(i + nums[i] >= pos) {
                    ++count;
                    pos = i;	// 更新pos到离pos最远的位置
                    break;
                }
            }
        }
        return count;
    }
    ```

- 正向查找可到达的最大位置

  - 如果我们「贪心」地进行正向查找，每次找到可到达的最远位置，就可以在线性时间内得到最少的跳跃次数

  - 每次在上次能跳到的范围（end）内选择一个能跳的最远的位置（也就是能跳到 maxFar 位置的点）作为下次的起跳点

    ```c++
    // On解法
    int jump(vector<int> & nums) {
        int maxFar = 0; // 目前能跳到的最远位置
        int step = 0;   // 跳跃次数
        int end = 0;    // 上次跳跃可达范围右边界（下次的最右起跳点）
        for (int i = 0; i < nums.size() - 1; i++) {
            // 统计到达end前，能跳到最远的位置
            max_far = max(maxFar, i + nums[i]);
            // 到达上次跳跃能到达的右边界了
            if (i == end) {
                end = maxFar;  // 目前能跳到的最远位置变成了下次起跳位置的有边界
                step++;         // 进入下一次跳跃
            }
        }
        return step;
    }
    ```

## [得到子序列的最少操作次数](https://leetcode-cn.com/problems/minimum-operations-to-make-a-subsequence/)

- 题目描述

  <img src="imgs/algorithm/problem1713.png" alt="problem1713" style="zoom:80%;" />

- 解题思路

  <img src="imgs/algorithm/solution10.png" alt="solution10" style="zoom:80%;" />

- 代码实现

  ```c++
  int minOperations(vector<int>& target, vector<int>& arr) {
      unordered_map<int, int> rec;
      int nt = target.size();

      for(int i = 0; i < nt; ++i) {
          rec[target[i]] = i;
      }

      vector<int> vec;
      for(auto &val : arr) {
          if(rec.find(val) != rec.end()) {
              int idx = rec[val];
              auto it = lower_bound(vec.begin(), vec.end(), idx);
              // vec前面出现大于等于idx的，用idx替换，使上升速度最缓
              if(it != vec.end()) {
                  *it = idx;
              }
              else {
                  // idx最大，直接加入
                  vec.push_back(idx);
              }
          }
      }

      return nt - vec.size();
  }
  ```

## [ 任务调度器](https://leetcode-cn.com/problems/task-scheduler/)
