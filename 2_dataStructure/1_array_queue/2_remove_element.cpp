// 移除元素
//
// 题目描述：
// 给你一个数组 nums 和一个值 val，你需要原地移除所有数值等于 val 的元素，并返回移除后数组的新长度。
// 不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并原地修改输入数组。
// 元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。
//
// 示例：
// 输入：nums = [3,2,2,3], val = 3
// 输出：2, nums = [2,2,_,_]
// 解释：函数应该返回新的长度 2, 并且 nums 中的前两个元素均为 2。你不需要考虑数组中超出新长度后面的元素。
//
// 输入：nums = [0,1,2,2,3,0,4,2], val = 2
// 输出：5, nums = [0,1,3,0,4,_,_,_]
// 解释：函数应该返回新的长度 5, 并且 nums 中的前五个元素为 0, 1, 3, 0, 4。

#include <cassert>
#include <iostream>
#include <vector>

int removeElement(std::vector<int>& nums, int val) {
    int slowIdx = 0;
    for (int fastIdx = 0; fastIdx < nums.size(); fastIdx++) {
        if (nums[fastIdx] != val) {
            nums[slowIdx] = nums[fastIdx];
            slowIdx++;
        }
    }
    return slowIdx;
}

// 测试removeElement函数的正确性
bool testValid() {
    // 测试用例
    std::vector<int> test1 = {3, 2, 2, 3};
    std::vector<int> expected1 = {2, 2};
    int expectedLen1 = 2;

    std::vector<int> test2 = {0, 1, 2, 2, 3, 0, 4, 2};
    std::vector<int> expected2 = {0, 1, 3, 0, 4};
    int expectedLen2 = 5;

    std::vector<int> test3 = {1};
    std::vector<int> expected3 = {};
    int expectedLen3 = 0;

    std::vector<int> test4 = {1, 1, 1};
    std::vector<int> expected4 = {};
    int expectedLen4 = 0;

    // 检查结果
    int len1 = removeElement(test1, 3);
    int len2 = removeElement(test2, 2);
    int len3 = removeElement(test3, 1);
    int len4 = removeElement(test4, 1);

    bool test1Result = len1 == expectedLen1;
    bool test2Result = len2 == expectedLen2;
    bool test3Result = len3 == expectedLen3;
    bool test4Result = len4 == expectedLen4;

    // 检查数组内容
    for (int i = 0; i < len1 && test1Result; i++) {
        if (test1[i] != expected1[i]) test1Result = false;
    }

    for (int i = 0; i < len2 && test2Result; i++) {
        if (test2[i] != expected2[i]) test2Result = false;
    }

    // 输出测试结果
    std::cout << "测试用例1 (移除值为3的元素): "
              << (test1Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例2 (移除值为2的元素): "
              << (test2Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例3 (移除单元素数组中的元素): "
              << (test3Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例4 (移除所有元素): " << (test4Result ? "通过" : "失败")
              << std::endl;

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
