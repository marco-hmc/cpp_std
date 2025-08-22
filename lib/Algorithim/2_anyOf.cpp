// std::any_of 用法详解和自定义实现

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::ANY_OF 标准用法 ===" << std::endl;

        // 1. 基本用法 - 检查是否有偶数
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::cout << "原数组: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << std::endl;

        bool anyEven = std::any_of(numbers.begin(), numbers.end(),
                                   [](int i) { return i % 2 == 0; });
        std::cout << "是否有偶数: " << (anyEven ? "是" : "否") << std::endl;

        // 2. 检查字符串长度
        std::vector<std::string> words = {"cat", "dog", "elephant", "ox"};
        std::cout << "\n字符串数组: ";
        for (const auto& word : words) std::cout << word << " ";
        std::cout << std::endl;

        bool anyLong =
            std::any_of(words.begin(), words.end(),
                        [](const std::string& s) { return s.length() > 5; });
        std::cout << "是否有长度>5的字符串: " << (anyLong ? "是" : "否")
                  << std::endl;

        // 3. 检查负数
        std::vector<int> mixed = {-1, 2, -3, 4, 5};
        std::cout << "\n混合数组: ";
        for (int n : mixed) std::cout << n << " ";
        std::cout << std::endl;

        bool anyNegative = std::any_of(mixed.begin(), mixed.end(),
                                       [](int x) { return x < 0; });
        std::cout << "是否有负数: " << (anyNegative ? "是" : "否") << std::endl;

        // 4. 空容器测试
        std::vector<int> empty;
        bool anyInEmpty = std::any_of(empty.begin(), empty.end(),
                                      [](int x) { return x > 0; });
        std::cout << "\n空容器中是否有正数: " << (anyInEmpty ? "是" : "否")
                  << std::endl;
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 any_of
    template <class InputIt, class UnaryPredicate>
    bool my_any_of(InputIt first, InputIt last, UnaryPredicate p) {
        while (first != last) {
            if (p(*first)) {
                return true;
            }
            ++first;
        }
        return false;
    }

    void task() {
        std::cout << "\n=== 自定义 ANY_OF 实现测试 ===" << std::endl;

        // 测试自定义实现
        std::vector<int> test_nums = {1, 3, 5, 7, 9};
        std::cout << "测试数组: ";
        for (int n : test_nums) std::cout << n << " ";
        std::cout << std::endl;

        bool my_result = my_any_of(test_nums.begin(), test_nums.end(),
                                   [](int x) { return x > 5; });
        std::cout << "自定义实现 - 是否有>5的数: " << (my_result ? "是" : "否")
                  << std::endl;

        // 对比标准实现
        bool std_result = std::any_of(test_nums.begin(), test_nums.end(),
                                      [](int x) { return x > 5; });
        std::cout << "标准实现 - 是否有>5的数: " << (std_result ? "是" : "否")
                  << std::endl;
        std::cout << "结果一致: " << (my_result == std_result ? "是" : "否")
                  << std::endl;
    }
}  // namespace MY_IMPL

namespace FAMILY_ALGORITHMS {
    void task() {
        std::cout << "\n=== ANY_OF 算法家族 ===" << std::endl;

        std::vector<int> test_data = {2, 4, 6, 8, 10};
        std::cout << "测试数据: ";
        for (int n : test_data) std::cout << n << " ";
        std::cout << std::endl;

        // any_of: 是否有任意一个满足条件
        bool any_result = std::any_of(test_data.begin(), test_data.end(),
                                      [](int x) { return x % 2 == 0; });
        std::cout << "any_of (是否有偶数): " << (any_result ? "是" : "否")
                  << std::endl;

        // all_of: 是否所有都满足条件
        bool all_result = std::all_of(test_data.begin(), test_data.end(),
                                      [](int x) { return x % 2 == 0; });
        std::cout << "all_of (是否都是偶数): " << (all_result ? "是" : "否")
                  << std::endl;

        // none_of: 是否没有满足条件的
        bool none_result = std::none_of(test_data.begin(), test_data.end(),
                                        [](int x) { return x % 2 != 0; });
        std::cout << "none_of (是否没有奇数): " << (none_result ? "是" : "否")
                  << std::endl;

        std::cout << "\n逻辑关系验证:" << std::endl;
        std::cout << "any_of 和 none_of 应该互补: " << std::endl;
        bool complement_check =
            any_result == !std::none_of(test_data.begin(), test_data.end(),
                                        [](int x) { return x % 2 == 0; });
        std::cout << "验证结果: " << (complement_check ? "正确" : "错误")
                  << std::endl;
    }
}  // namespace FAMILY_ALGORITHMS

namespace ADVANCED_USAGE {
    void task() {
        std::cout << "\n=== ANY_OF 高级用法 ===" << std::endl;

        // 1. 复杂对象检查
        struct Student {
            std::string name;
            int age;
            double gpa;

            Student(const std::string& n, int a, double g)
                : name(n), age(a), gpa(g) {}
        };

        std::vector<Student> students = {{"Alice", 20, 3.8},
                                         {"Bob", 19, 3.2},
                                         {"Charlie", 21, 3.9},
                                         {"David", 20, 2.8}};

        std::cout << "学生信息:" << std::endl;
        for (const auto& s : students) {
            std::cout << s.name << "(年龄:" << s.age << ", GPA:" << s.gpa
                      << ") ";
        }
        std::cout << std::endl;

        // 检查是否有GPA > 3.5的学生
        bool hasHighGPA =
            std::any_of(students.begin(), students.end(),
                        [](const Student& s) { return s.gpa > 3.5; });
        std::cout << "是否有GPA>3.5的学生: " << (hasHighGPA ? "是" : "否")
                  << std::endl;

        // 2. 多条件检查
        bool hasYoungHighAchiever = std::any_of(
            students.begin(), students.end(),
            [](const Student& s) { return s.age < 21 && s.gpa > 3.5; });
        std::cout << "是否有年龄<21且GPA>3.5的学生: "
                  << (hasYoungHighAchiever ? "是" : "否") << std::endl;

        // 3. 性能提示
        std::cout << "\n性能特性:" << std::endl;
        std::cout
            << "- any_of 在找到第一个满足条件的元素时立即返回 true（短路求值）"
            << std::endl;
        std::cout << "- 最佳情况: O(1)，最坏情况: O(n)" << std::endl;
        std::cout << "- 适合在大数据集中快速检查是否存在满足条件的元素"
                  << std::endl;
    }
}  // namespace ADVANCED_USAGE

int main() {
    // 原始基本示例
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    bool anyEven = std::any_of(numbers.begin(), numbers.end(),
                               [](int i) { return i % 2 == 0; });
    assert(anyEven == true);

    std::cout << "=== 基本示例 ===" << std::endl;
    std::cout << "数组 {1, 2, 3, 4, 5} 中是否有偶数: "
              << (anyEven ? "是" : "否") << std::endl;

    // 调用详细示例
    STD_USAGE::task();
    MY_IMPL::task();
    FAMILY_ALGORITHMS::task();
    ADVANCED_USAGE::task();

    return 0;
}