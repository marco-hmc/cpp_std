// 长度最小的子数组
//
// 题目描述：
// 给定一个含有 n 个正整数的数组和一个正整数 target 。
// 找出该数组中满足其和 ≥ target 的长度最小的连续子数组，并返回其长度。
// 如果不存在符合条件的子数组，返回 0 。
//
// 示例：
// 输入：target = 7, nums = [2,3,1,2,4,3]
// 输出：2
// 解释：子数组 [4,3] 是该条件下的长度最小的子数组。
//
// 输入：target = 4, nums = [1,4,4]
// 输出：1
//
// 输入：target = 11, nums = [1,1,1,1,1,1,1,1]
// 输出：0

#include <cassert>
#include <climits>
#include <iostream>
#include <numeric>
#include <vector>

// 优化版本，使用滑动窗口，不需要每次重新计算子数组和
int minSubArrayLen(int target, std::vector<int>& nums) {
    int left = 0;
    int sum = 0;
    int minLen = INT_MAX;

    for (int right = 0; right < nums.size(); right++) {
        sum += nums[right];

        while (sum >= target) {
            minLen = std::min(minLen, right - left + 1);
            sum -= nums[left];
            left++;
        }
    }

    return minLen == INT_MAX ? 0 : minLen;
}

// 测试minSubArrayLen函数的正确性
bool testValid() {
    // 测试用例
    std::vector<int> test1 = {2, 3, 1, 2, 4, 3};
    int target1 = 7;
    int expected1 = 2;  // 子数组[4,3]长度为2

    std::vector<int> test2 = {1, 4, 4};
    int target2 = 4;
    int expected2 = 1;  // 子数组[4]长度为1

    std::vector<int> test3 = {1, 1, 1, 1, 1, 1, 1, 1};
    int target3 = 11;
    int expected3 = 0;  // 不存在符合条件的子数组

    std::vector<int> test4 = {1, 2, 3, 4, 5};
    int target4 = 15;
    int expected4 = 5;  // 整个数组

    // 检查结果是否符合预期
    bool test1Result = minSubArrayLen(target1, test1) == expected1;
    bool test2Result = minSubArrayLen(target2, test2) == expected2;
    bool test3Result = minSubArrayLen(target3, test3) == expected3;
    bool test4Result = minSubArrayLen(target4, test4) == expected4;

    // 输出测试结果
    std::cout << "测试用例1 (target=7): " << (test1Result ? "通过" : "失败")
              << " 结果: " << minSubArrayLen(target1, test1) << std::endl;
    std::cout << "测试用例2 (target=4): " << (test2Result ? "通过" : "失败")
              << " 结果: " << minSubArrayLen(target2, test2) << std::endl;
    std::cout << "测试用例3 (target=11): " << (test3Result ? "通过" : "失败")
              << " 结果: " << minSubArrayLen(target3, test3) << std::endl;
    std::cout << "测试用例4 (target=15): " << (test4Result ? "通过" : "失败")
              << " 结果: " << minSubArrayLen(target4, test4) << std::endl;

    return test1Result && test2Result && test3Result && test4Result;
}

int main() {
    if (testValid()) {
        std::cout << "所有测试通过!" << std::endl;
    } else {
        std::cout << "测试失败!" << std::endl;
    }
    return 0;
}
