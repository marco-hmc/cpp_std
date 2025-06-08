// 查找最接近的K个数
//
// 题目描述：
// 给定一个排序数组，找到数组中第k个元素最近的m个数。
// 最接近是指差的绝对值最小。
//
// 例如：
// 数组 [1, 3, 5, 7, 9, 11, 13, 15]
// k = 3，表示第3个元素，即值为5的元素
// m = 2，表示寻找最接近的2个数
// 结果应该是 [3, 7]
//
// 注意：
// 如果有多个差值相同的元素，优先选择较小的元素。
// 返回的结果应该按照升序排列。

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>

// 查找最接近的m个数
std::vector<int> findClosestNumbers(const std::vector<int>& arr, int k, int m) {
    if (arr.empty() || k < 0 || k >= arr.size() || m <= 0) {
        return {};
    }

    int targetValue = arr[k];  // 目标值

    // 定义比较函数：绝对差值大的优先出队，如果差值相同，值大的优先出队
    auto cmp = [targetValue](int a, int b) {
        int diffA = std::abs(a - targetValue);
        int diffB = std::abs(b - targetValue);
        if (diffA != diffB) {
            return diffA < diffB;  // 小的差值优先保留
        }
        return a < b;  // 如果差值相同，小的值优先保留
    };

    // 使用自定义比较函数的优先队列（大顶堆）
    std::priority_queue<int, std::vector<int>, decltype(cmp)> maxHeap(cmp);

    // 将所有元素（除了目标值）加入堆
    for (int i = 0; i < arr.size(); i++) {
        if (i == k) continue;  // 跳过目标元素自身

        maxHeap.push(arr[i]);
        // 如果堆大小超过m，弹出最不接近的元素
        if (maxHeap.size() > m) {
            maxHeap.pop();
        }
    }

    // 将结果放入向量并排序
    std::vector<int> result;
    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    std::sort(result.begin(), result.end());

    return result;
}

// 测试findClosestNumbers函数的正确性
bool testValid() {
    // 测试用例1：基本测试
    std::vector<int> test1 = {1, 3, 5, 7, 9, 11, 13, 15};
    std::vector<int> expected1 = {3, 7};  // 与5最接近的两个数

    // 测试用例2：包含相等差值的情况
    std::vector<int> test2 = {1, 2, 3, 6, 9, 12};
    std::vector<int> expected2 = {
        1, 2};  // 与3最接近的两个数（2和6差值都是1，选择较小的2）

    // 测试用例3：边界情况
    std::vector<int> test3 = {1, 10, 100, 1000};
    std::vector<int> expected3 = {1, 100};  // 与10最接近的两个数

    // 测试用例4：错误参数
    std::vector<int> test4 = {1, 2, 3};
    std::vector<int> expected4 = {};  // k超出范围

    auto result1 = findClosestNumbers(test1, 2, 2);  // 第2个元素是5
    auto result2 = findClosestNumbers(test2, 2, 2);  // 第2个元素是3
    auto result3 = findClosestNumbers(test3, 1, 2);  // 第1个元素是10
    auto result4 = findClosestNumbers(test4, 5, 1);  // k超出范围

    bool test1Result = (result1 == expected1);
    bool test2Result = (result2 == expected2);
    bool test3Result = (result3 == expected3);
    bool test4Result = (result4 == expected4);

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

    std::cout << "测试用例2 (相等差值): " << (test2Result ? "通过" : "失败")
              << std::endl;
    std::cout << "测试用例3 (边界情况): " << (test3Result ? "通过" : "失败")
              << std::endl;
    std::cout << "测试用例4 (错误参数): " << (test4Result ? "通过" : "失败")
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
