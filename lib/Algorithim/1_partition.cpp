// std::partition 用法详解和自定义实现

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::PARTITION 标准用法 ===" << std::endl;

        // 1. 基本用法 - 分离奇偶数
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "原数组: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << std::endl;

        auto it = std::partition(numbers.begin(), numbers.end(),
                                 [](int n) { return n % 2 != 0; });

        std::cout << "分离奇偶后: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << "\n分界点位置: " << std::distance(numbers.begin(), it)
                  << std::endl;

        // 2. stable_partition - 保持相对顺序
        std::vector<int> stable_nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "\n使用 stable_partition (保持相对顺序):" << std::endl;
        std::cout << "原数组: ";
        for (int n : stable_nums) std::cout << n << " ";
        std::cout << std::endl;

        auto stable_it =
            std::stable_partition(stable_nums.begin(), stable_nums.end(),
                                  [](int n) { return n % 2 != 0; });

        std::cout << "分离后: ";
        for (int n : stable_nums) std::cout << n << " ";
        std::cout << std::endl;

        // 3. 字符串分类 - 按长度分类
        std::vector<std::string> words = {
            "cat", "elephant", "dog", "butterfly", "ox", "hippopotamus"};
        std::cout << "\n字符串按长度分类 (长度<=3):" << std::endl;
        std::cout << "原字符串: ";
        for (const auto& word : words) std::cout << word << " ";
        std::cout << std::endl;

        auto word_it = std::partition(
            words.begin(), words.end(),
            [](const std::string& s) { return s.length() <= 3; });

        std::cout << "分类后: ";
        for (const auto& word : words) std::cout << word << " ";
        std::cout << std::endl;

        // 4. partition_point - 找到分界点
        std::vector<int> sorted_data = {1, 3, 5, 7, 2, 4, 6, 8};
        std::partition(sorted_data.begin(), sorted_data.end(),
                       [](int n) { return n % 2 != 0; });

        auto partition_pt =
            std::partition_point(sorted_data.begin(), sorted_data.end(),
                                 [](int n) { return n % 2 != 0; });

        std::cout << "\npartition_point 找到的分界点位置: "
                  << std::distance(sorted_data.begin(), partition_pt)
                  << std::endl;
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 partition
    template <class ForwardIt, class UnaryPred>
    ForwardIt my_partition(ForwardIt first, ForwardIt last, UnaryPred p) {
        // 找到第一个不满足条件的元素
        first = std::find_if_not(first, last, p);
        if (first == last) return first;

        // 从剩下的再找满足条件的，与不满足条件的元素交换
        for (auto i = std::next(first); i != last; ++i) {
            if (p(*i)) {
                std::iter_swap(i, first);
                ++first;
            }
        }
        return first;
    }

    // 自定义实现 stable_partition (简化版)
    template <class ForwardIt, class UnaryPred>
    ForwardIt my_stable_partition(ForwardIt first, ForwardIt last,
                                  UnaryPred p) {
        auto dist = std::distance(first, last);
        if (dist <= 1) return first;

        // 使用额外空间保持稳定性
        std::vector<typename std::iterator_traits<ForwardIt>::value_type> temp;
        std::vector<typename std::iterator_traits<ForwardIt>::value_type>
            false_part;

        for (auto it = first; it != last; ++it) {
            if (p(*it)) {
                temp.push_back(*it);
            } else {
                false_part.push_back(*it);
            }
        }

        // 复制回原容器
        auto result = first;
        for (const auto& val : temp) {
            *result++ = val;
        }
        auto partition_point = result;
        for (const auto& val : false_part) {
            *result++ = val;
        }

        return partition_point;
    }

    void task() {
        std::cout << "\n=== 自定义 PARTITION 实现测试 ===" << std::endl;

        // 测试自定义 partition
        std::vector<int> test1 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
        std::cout << "自定义 partition 测试:" << std::endl;
        std::cout << "原数组: ";
        for (int n : test1) std::cout << n << " ";
        std::cout << std::endl;

        auto my_it = my_partition(test1.begin(), test1.end(),
                                  [](int n) { return n > 5; });

        std::cout << "分离后 (>5): ";
        for (int n : test1) std::cout << n << " ";
        std::cout << "\n分界点位置: " << std::distance(test1.begin(), my_it)
                  << std::endl;

        // 测试自定义 stable_partition
        std::vector<int> test2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "\n自定义 stable_partition 测试:" << std::endl;
        std::cout << "原数组: ";
        for (int n : test2) std::cout << n << " ";
        std::cout << std::endl;

        auto stable_my_it = my_stable_partition(
            test2.begin(), test2.end(), [](int n) { return n % 2 == 0; });

        std::cout << "分离后 (偶数): ";
        for (int n : test2) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace MY_IMPL

namespace ADVANCED_USAGE {
    void task() {
        std::cout << "\n=== PARTITION 高级用法 ===" << std::endl;

        // 1. 三路分割 (Dutch National Flag Problem)
        std::vector<int> colors = {2, 0, 1, 2, 1, 0, 2, 1, 0, 1, 2, 0};
        std::cout << "三路分割示例 (0, 1, 2 分别代表红白蓝):" << std::endl;
        std::cout << "原数组: ";
        for (int c : colors) std::cout << c << " ";
        std::cout << std::endl;

        // 先分离 0 和非0
        auto mid1 = std::partition(colors.begin(), colors.end(),
                                   [](int x) { return x == 0; });
        // 再在非0部分分离 1 和 2
        auto mid2 =
            std::partition(mid1, colors.end(), [](int x) { return x == 1; });

        std::cout << "分割后: ";
        for (int c : colors) std::cout << c << " ";
        std::cout << std::endl;

        // 2. 复杂条件分割 - 按多个条件分类
        struct Person {
            std::string name;
            int age;
            bool is_student;

            Person(const std::string& n, int a, bool s)
                : name(n), age(a), is_student(s) {}
        };

        std::vector<Person> people = {
            {"Alice", 20, true},  {"Bob", 25, false}, {"Charlie", 19, true},
            {"David", 30, false}, {"Eve", 22, true},  {"Frank", 35, false}};

        std::cout << "\n复杂对象分割 (学生 vs 非学生):" << std::endl;
        std::cout << "原顺序: ";
        for (const auto& p : people) {
            std::cout << p.name << "(" << (p.is_student ? "学生" : "非学生")
                      << ") ";
        }
        std::cout << std::endl;

        auto student_end =
            std::stable_partition(people.begin(), people.end(),
                                  [](const Person& p) { return p.is_student; });

        std::cout << "分割后: ";
        for (const auto& p : people) {
            std::cout << p.name << "(" << (p.is_student ? "学生" : "非学生")
                      << ") ";
        }
        std::cout << std::endl;

        // 3. 性能对比说明
        std::cout << "\n性能特性说明:" << std::endl;
        std::cout << "- partition: O(n) 时间, O(1) 空间, 不保持相对顺序"
                  << std::endl;
        std::cout
            << "- stable_partition: O(n log n) 时间或 O(n) 空间, 保持相对顺序"
            << std::endl;
        std::cout << "- partition_point: O(log n) 时间, 需要已分割的序列"
                  << std::endl;
    }
}  // namespace ADVANCED_USAGE

bool isOdd(int n) { return n % 2 != 0; }

namespace BASIC_EXAMPLE {
    void task() {
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
        std::cout << "=== 基本示例 ===" << std::endl;
        std::cout << "原数组: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << std::endl;

        auto it = std::partition(numbers.begin(), numbers.end(), isOdd);

        std::cout << "重新排列后的数组: ";
        for (int n : numbers) {
            std::cout << n << " ";
        }
        std::cout << std::endl;

        std::cout << "第一个不满足条件的元素的位置: "
                  << std::distance(numbers.begin(), it) << std::endl;
    }
}  // namespace BASIC_EXAMPLE

int main() {
    BASIC_EXAMPLE::task();
    STD_USAGE::task();
    MY_IMPL::task();
    ADVANCED_USAGE::task();
    return 0;
}