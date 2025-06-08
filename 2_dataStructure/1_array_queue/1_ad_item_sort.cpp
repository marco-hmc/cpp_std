/*
    广告项目排序
    题目描述：
    实现一个函数，按照如下规则对广告项目进行排序：
    1. 按照priority升序排列
    2. 如果priority一样大，则按照score降序排列
    3. 如果priority和score都一样，则保持原有顺序（稳定排序）
    AdItem结构如下：
    struct AdItem {
        std::string ad_id;   // 广告ID
        int priority;        // 优先级
        int score;           // 分数
    };
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct AdItem {
    std::string ad_id;
    int priority;
    int score;

    AdItem(std::string id, int p, int s) : ad_id(id), priority(p), score(s) {}

    bool operator==(const AdItem& other) const {
        return ad_id == other.ad_id && priority == other.priority &&
               score == other.score;
    }
};

std::vector<AdItem> sortAdItems(std::vector<AdItem> items) {
    std::stable_sort(items.begin(), items.end(),
                     [](const AdItem& a, const AdItem& b) {
                         if (a.priority != b.priority) {
                             return a.priority < b.priority;
                         }
                         return a.score > b.score;
                     });

    return items;
}

void printAdItems(const std::vector<AdItem>& items) {
    for (const auto& item : items) {
        std::cout << "ID: " << item.ad_id << ", Priority: " << item.priority
                  << ", Score: " << item.score << std::endl;
    }
    std::cout << std::endl;
}

bool testValid() {
    std::vector<AdItem> test1 = {
        {"A1", 3, 100}, {"A2", 1, 80}, {"A3", 2, 90}, {"A4", 1, 95}};

    std::vector<AdItem> expected1 = {
        {"A4", 1, 95}, {"A2", 1, 80}, {"A3", 2, 90}, {"A1", 3, 100}};

    std::vector<AdItem> test2 = {
        {"B1", 2, 70}, {"B2", 2, 90}, {"B3", 2, 85}, {"B4", 1, 60}};

    std::vector<AdItem> expected2 = {
        {"B4", 1, 60}, {"B2", 2, 90}, {"B3", 2, 85}, {"B1", 2, 70}};

    std::vector<AdItem> test3 = {
        {"C1", 1, 80}, {"C2", 1, 80}, {"C3", 2, 70}, {"C4", 2, 70}};

    std::vector<AdItem> expected3 = {
        {"C1", 1, 80}, {"C2", 1, 80}, {"C3", 2, 70}, {"C4", 2, 70}};

    auto result1 = sortAdItems(test1);
    auto result2 = sortAdItems(test2);
    auto result3 = sortAdItems(test3);

    bool test1Result = (result1 == expected1);
    bool test2Result = (result2 == expected2);
    bool test3Result = (result3 == expected3);

    std::cout << "测试用例1 (基本排序): " << (test1Result ? "通过" : "失败")
              << std::endl;
    if (!test1Result) {
        std::cout << "预期结果:" << std::endl;
        printAdItems(expected1);
        std::cout << "实际结果:" << std::endl;
        printAdItems(result1);
    }

    std::cout << "测试用例2 (相同优先级按分数排序): "
              << (test2Result ? "通过" : "失败") << std::endl;
    if (!test2Result) {
        std::cout << "预期结果:" << std::endl;
        printAdItems(expected2);
        std::cout << "实际结果:" << std::endl;
        printAdItems(result2);
    }

    std::cout << "测试用例3 (稳定排序): " << (test3Result ? "通过" : "失败")
              << std::endl;
    if (!test3Result) {
        std::cout << "预期结果:" << std::endl;
        printAdItems(expected3);
        std::cout << "实际结果:" << std::endl;
        printAdItems(result3);
    }

    return test1Result && test2Result && test3Result;
}

int main() {
    if (testValid()) {
        std::cout << "所有测试通过!" << std::endl;
    } else {
        std::cout << "测试失败!" << std::endl;
    }
    return 0;
}
