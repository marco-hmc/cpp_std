// 查找最小的K个数
//
// 题目描述：
// 输入整数数组 arr，找出其中最小的 k 个数。例如，输入 [9,4,2,7,5,1,8,3,6,10,12,11]，
// 则最小的3个数是 [1,2,3]。
//
// 要求：使用优先队列（大顶堆/小顶堆）实现，尝试实现O(n log k)的时间复杂度。

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <vector>

std::vector<int> findSmallestNumbers(const std::vector<int>& arr, int k) {
    if (k <= 0 || k > arr.size()) {
        return {};
    }

    // 使用大顶堆
    std::priority_queue<int> maxHeap;

    // 先将前k个数加入堆
    for (int i = 0; i < k; i++) {
        maxHeap.push(arr[i]);
    }

    // 遍历剩余的数，如果小于堆顶，则替换堆顶
    for (int i = k; i < arr.size(); i++) {
        if (arr[i] < maxHeap.top()) {
            maxHeap.pop();
            maxHeap.push(arr[i]);
        }
    }

    // 将堆中的元素放入结果数组
    std::vector<int> result;
    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    // 对结果进行排序
    std::sort(result.begin(), result.end());

    return result;
}

// 测试findSmallestNumbers函数的正确性
bool testValid() {
    // 测试用例1：基本测试
    std::vector<int> test1 = {9, 4, 2, 7, 5, 1, 8, 3, 6, 10, 12, 11};
    std::vector<int> expected1 = {1, 2, 3};

    // 测试用例2：k等于数组长度
    std::vector<int> test2 = {5, 3, 7, 1};
    std::vector<int> expected2 = {1, 3, 5, 7};

    // 测试用例3：有重复元素
    std::vector<int> test3 = {4, 5, 1, 6, 2, 7, 3, 8, 1};
    std::vector<int> expected3 = {1, 1, 2};

    // 测试用例4：k为0或超出范围
    std::vector<int> test4 = {1, 2, 3};
    std::vector<int> expected4 = {};

    auto result1 = findSmallestNumbers(test1, 3);
    auto result2 = findSmallestNumbers(test2, 4);
    auto result3 = findSmallestNumbers(test3, 3);
    auto result4 = findSmallestNumbers(test4, 0);
    auto result5 = findSmallestNumbers(test4, 5);

    bool test1Result = (result1 == expected1);
    bool test2Result = (result2 == expected2);
    bool test3Result = (result3 == expected3);
    bool test4Result = (result4 == expected4);
    bool test5Result = (result5 == expected4);

    // 输出测试结果
    std::cout << "测试用例1 (基本测试): " << (test1Result ? "通过" : "失败")
              << std::endl;
    if (!test1Result) {
        std::cout << "预期: [";
        for (auto num : expected1) std::cout << num << " ";
        std::cout << "], 实际: [";
        for (auto num : result1) std::cout << num << " ";
        std::cout << "]" << std::endl;
    }

    std::cout << "测试用例2 (k等于数组长度): "
              << (test2Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例3 (有重复元素): " << (test3Result ? "通过" : "失败")
              << std::endl;
    std::cout << "测试用例4 (k为0): " << (test4Result ? "通过" : "失败")
              << std::endl;
    std::cout << "测试用例5 (k超出范围): " << (test5Result ? "通过" : "失败")
              << std::endl;

    return test1Result && test2Result && test3Result && test4Result &&
           test5Result;
}

int main() {
    if (testValid()) {
        std::cout << "所有测试通过!" << std::endl;
    } else {
        std::cout << "测试失败!" << std::endl;
    }
    return 0;
}
