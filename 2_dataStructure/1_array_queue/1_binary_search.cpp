// 二分查找
//
// 题目描述：
// 给定一个 n 个元素有序的（升序）整型数组 nums 和一个目标值 target，
// 写一个函数搜索 nums 中的 target，如果目标值存在返回下标，否则返回 -1。
//
// 示例：
// 输入: nums = [1,3,5,7,9,11,13], target = 7
// 输出: 3
//
// 输入: nums = [1,3,5,7,9,11,13], target = 8
// 输出: -1

#include <cassert>
#include <iostream>
#include <vector>

int binarySearch(std::vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;  // 修正：应为size()-1

    while (left <= right) {  // 修正：应为<=
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid;
        }

        if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

// 测试binarySearch函数的正确性
bool testValid() {
    // 测试用例
    std::vector<int> test1 = {1, 3, 5, 7, 9, 11, 13};
    int expected1 = 3;  // 对应元素7的位置

    std::vector<int> test2 = {1, 3, 5, 7, 9, 11, 13};
    int expected2 = -1;  // 查找不存在的元素8

    std::vector<int> test3 = {1, 3, 5, 7, 9, 11};
    int expected3 = 0;  // 查找第一个元素

    std::vector<int> test4 = {1, 3, 5, 7, 9, 11};
    int expected4 = 5;  // 查找最后一个元素

    // 检查结果是否符合预期
    bool test1Result = binarySearch(test1, 7) == expected1;
    bool test2Result = binarySearch(test2, 8) == expected2;
    bool test3Result = binarySearch(test3, 1) == expected3;
    bool test4Result = binarySearch(test4, 11) == expected4;

    // 输出测试结果
    std::cout << "测试用例1 (查找存在的元素7): "
              << (test1Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例2 (查找不存在的元素8): "
              << (test2Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例3 (查找第一个元素): "
              << (test3Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例4 (查找最后一个元素): "
              << (test4Result ? "通过" : "失败") << std::endl;

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
