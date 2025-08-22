// std::remove_if 用法详解和自定义实现

#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::REMOVE_IF 标准用法 ===" << std::endl;

        // 1. 基本用法 - 移除偶数
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "原数组: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << std::endl;

        auto it = std::remove_if(numbers.begin(), numbers.end(),
                                 [](int x) { return x % 2 == 0; });

        std::cout << "remove_if后(未erase): ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << "\n新的逻辑结尾位置: "
                  << std::distance(numbers.begin(), it) << std::endl;

        // 2. 使用 erase-remove 惯用法
        std::vector<int> nums2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "\n使用 erase-remove 惯用法:" << std::endl;
        std::cout << "原数组: ";
        for (int n : nums2) std::cout << n << " ";
        std::cout << std::endl;

        nums2.erase(std::remove_if(nums2.begin(), nums2.end(),
                                   [](int x) { return x % 2 == 0; }),
                    nums2.end());

        std::cout << "真正删除偶数后: ";
        for (int n : nums2) std::cout << n << " ";
        std::cout << std::endl;

        // 3. 字符串过滤 - 移除长度小于4的单词
        std::vector<std::string> words = {"cat", "dog",       "elephant",
                                          "ox",  "butterfly", "fish"};
        std::cout << "\n字符串过滤:" << std::endl;
        std::cout << "原字符串: ";
        for (const auto& word : words) std::cout << word << " ";
        std::cout << std::endl;

        words.erase(
            std::remove_if(words.begin(), words.end(),
                           [](const std::string& s) { return s.length() < 4; }),
            words.end());

        std::cout << "移除长度<4的单词后: ";
        for (const auto& word : words) std::cout << word << " ";
        std::cout << std::endl;

        // 4. 复杂条件过滤
        std::vector<int> mixed = {-5, -2, 0, 3, -1, 8, -9, 4};
        std::cout << "\n复杂条件过滤 (移除负数和零):" << std::endl;
        std::cout << "原数组: ";
        for (int n : mixed) std::cout << n << " ";
        std::cout << std::endl;

        mixed.erase(std::remove_if(mixed.begin(), mixed.end(),
                                   [](int x) { return x <= 0; }),
                    mixed.end());

        std::cout << "只保留正数: ";
        for (int n : mixed) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 remove_if
    template <class ForwardIt, class UnaryPredicate>
    ForwardIt my_remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p) {
        ForwardIt result = first;
        while (first != last) {
            if (!p(*first)) {
                if (result != first) {
                    *result = std::move(*first);
                }
                ++result;
            }
            ++first;
        }
        return result;
    }

    void task() {
        std::cout << "\n=== 自定义 REMOVE_IF 实现测试 ===" << std::endl;

        // 测试自定义实现
        std::vector<int> test_nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "自定义实现测试:" << std::endl;
        std::cout << "原数组: ";
        for (int n : test_nums) std::cout << n << " ";
        std::cout << std::endl;

        auto my_it = my_remove_if(test_nums.begin(), test_nums.end(),
                                  [](int x) { return x > 5; });

        std::cout << "移除>5的元素后: ";
        for (auto iter = test_nums.begin(); iter != my_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        // 对比标准实现
        std::vector<int> std_test = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto std_it = std::remove_if(std_test.begin(), std_test.end(),
                                     [](int x) { return x > 5; });

        std::cout << "标准实现结果: ";
        for (auto iter = std_test.begin(); iter != std_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;
    }
}  // namespace MY_IMPL

namespace IMPORTANT_NOTES {
    void task() {
        std::cout << "\n=== REMOVE_IF 重要说明 ===" << std::endl;

        std::cout << "1. remove_if 不会改变容器大小!" << std::endl;
        std::vector<int> demo = {1, 2, 3, 4, 5};
        std::cout << "原容器大小: " << demo.size() << std::endl;

        auto it = std::remove_if(demo.begin(), demo.end(),
                                 [](int x) { return x % 2 == 0; });

        std::cout << "remove_if后容器大小: " << demo.size() << std::endl;
        std::cout << "有效元素数量: " << std::distance(demo.begin(), it)
                  << std::endl;

        std::cout << "\n2. remove_if 是\"移动\"而非\"删除\"!" << std::endl;
        std::cout << "remove_if后的完整容器内容: ";
        for (int n : demo) std::cout << n << " ";
        std::cout << std::endl;
        std::cout << "(注意：被\"移除\"的元素仍在容器末尾)" << std::endl;

        std::cout << "\n3. 必须配合 erase 才能真正删除!" << std::endl;
        demo.erase(it, demo.end());
        std::cout << "erase后容器大小: " << demo.size() << std::endl;
        std::cout << "最终内容: ";
        for (int n : demo) std::cout << n << " ";
        std::cout << std::endl;

        std::cout << "\n4. remove vs remove_if 的区别:" << std::endl;
        std::vector<int> demo2 = {1, 2, 3, 2, 4, 2, 5};
        std::cout << "remove(value): 移除等于特定值的元素" << std::endl;
        std::cout << "remove_if(predicate): 移除满足条件的元素" << std::endl;

        auto remove_it = std::remove(demo2.begin(), demo2.end(), 2);
        std::cout << "remove(2)结果: ";
        for (auto iter = demo2.begin(); iter != remove_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;
    }
}  // namespace IMPORTANT_NOTES

namespace ADVANCED_USAGE {
    void task() {
        std::cout << "\n=== REMOVE_IF 高级用法 ===" << std::endl;

        // 1. 复杂对象过滤
        struct Person {
            std::string name;
            int age;
            double salary;

            Person(const std::string& n, int a, double s)
                : name(n), age(a), salary(s) {}
        };

        std::vector<Person> employees = {{"Alice", 25, 50000},
                                         {"Bob", 30, 60000},
                                         {"Charlie", 22, 40000},
                                         {"David", 35, 80000}};

        std::cout << "员工信息过滤 (移除年龄<25或薪水<50000的员工):"
                  << std::endl;
        std::cout << "原员工列表:" << std::endl;
        for (const auto& p : employees) {
            std::cout << p.name << "(年龄:" << p.age << ", 薪水:" << p.salary
                      << ") ";
        }
        std::cout << std::endl;

        employees.erase(
            std::remove_if(
                employees.begin(), employees.end(),
                [](const Person& p) { return p.age < 25 || p.salary < 50000; }),
            employees.end());

        std::cout << "过滤后:" << std::endl;
        for (const auto& p : employees) {
            std::cout << p.name << "(年龄:" << p.age << ", 薪水:" << p.salary
                      << ") ";
        }
        std::cout << std::endl;

        // 2. 与其他算法结合
        std::cout << "\n与其他算法结合使用:" << std::endl;
        std::vector<int> data = {5, 1, 9, 3, 7, 2, 8, 4, 6};

        // 先排序，再移除小于5的元素
        std::sort(data.begin(), data.end());
        std::cout << "排序后: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        data.erase(std::remove_if(data.begin(), data.end(),
                                  [](int x) { return x < 5; }),
                   data.end());

        std::cout << "移除<5的元素: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 3. 性能提示
        std::cout << "\n性能提示:" << std::endl;
        std::cout << "- remove_if 的时间复杂度是 O(n)" << std::endl;
        std::cout << "- 对于 list，使用 list::remove_if 更高效" << std::endl;
        std::cout << "- 对于关联容器，使用专门的 erase 方法" << std::endl;

        // list 的特殊用法
        std::list<int> lst = {1, 2, 3, 4, 5, 6};
        std::cout << "\nlist 专用方法示例:" << std::endl;
        std::cout << "原list: ";
        for (int n : lst) std::cout << n << " ";
        std::cout << std::endl;

        lst.remove_if([](int x) { return x % 2 == 0; });
        std::cout << "list.remove_if后: ";
        for (int n : lst) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace ADVANCED_USAGE

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "=== 基本示例 ===" << std::endl;
    std::cout << "原数组: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    auto it = std::remove_if(numbers.begin(), numbers.end(),
                             [](int x) { return x % 2 == 0; });

    numbers.erase(it, numbers.end());

    std::cout << "移除偶数后: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // 调用详细示例
    STD_USAGE::task();
    MY_IMPL::task();
    IMPORTANT_NOTES::task();
    ADVANCED_USAGE::task();

    return 0;
}