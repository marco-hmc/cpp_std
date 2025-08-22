// std::unique 用法详解和自定义实现

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::UNIQUE 标准用法 ===" << std::endl;

        // 1. 基本去重 - 连续相同元素
        std::vector<int> numbers = {1, 2, 2, 3, 4, 4, 5, 6, 6};
        std::cout << "原数组: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << std::endl;

        auto it = std::unique(numbers.begin(), numbers.end());
        std::cout << "去重后: ";
        for (auto iter = numbers.begin(); iter != it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << "\n新的逻辑结尾位置: "
                  << std::distance(numbers.begin(), it) << std::endl;

        // 2. 使用 erase 真正删除重复元素
        std::vector<int> nums2 = {1, 1, 2, 2, 3, 3, 4, 4, 5};
        std::cout << "\n使用 erase-unique 惯用法:" << std::endl;
        std::cout << "原数组: ";
        for (int n : nums2) std::cout << n << " ";
        std::cout << std::endl;

        nums2.erase(std::unique(nums2.begin(), nums2.end()), nums2.end());
        std::cout << "真正删除后: ";
        for (int n : nums2) std::cout << n << " ";
        std::cout << std::endl;

        // 3. 自定义比较谓词 - 按绝对值去重
        std::vector<int> signed_nums = {-1, 1, -2, 2, -3, 3, 4};
        std::cout << "\n按绝对值去重:" << std::endl;
        std::cout << "原数组: ";
        for (int n : signed_nums) std::cout << n << " ";
        std::cout << std::endl;

        auto abs_it = std::unique(
            signed_nums.begin(), signed_nums.end(),
            [](int a, int b) { return std::abs(a) == std::abs(b); });

        std::cout << "按绝对值去重后: ";
        for (auto iter = signed_nums.begin(); iter != abs_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        // 4. 字符串按长度去重
        std::vector<std::string> words = {"a",   "bb",  "cc",
                                          "ddd", "eee", "ffff"};
        std::cout << "\n字符串按长度去重:" << std::endl;
        std::cout << "原字符串: ";
        for (const auto& word : words) std::cout << word << " ";
        std::cout << std::endl;

        auto word_it =
            std::unique(words.begin(), words.end(),
                        [](const std::string& a, const std::string& b) {
                            return a.length() == b.length();
                        });

        std::cout << "按长度去重后: ";
        for (auto iter = words.begin(); iter != word_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 unique (默认比较版本)
    template <class ForwardIt>
    ForwardIt my_unique(ForwardIt first, ForwardIt last) {
        if (first == last) return last;

        ForwardIt result = first;
        while (++first != last) {
            if (!(*result == *first)) {
                *(++result) = std::move(*first);
            }
        }
        return ++result;
    }

    // 自定义实现 unique (自定义谓词版本)
    template <class ForwardIt, class BinaryPredicate>
    ForwardIt my_unique(ForwardIt first, ForwardIt last, BinaryPredicate p) {
        if (first == last) return last;

        ForwardIt result = first;
        while (++first != last) {
            if (!p(*result, *first)) {
                *(++result) = std::move(*first);
            }
        }
        return ++result;
    }

    void task() {
        std::cout << "\n=== 自定义 UNIQUE 实现测试 ===" << std::endl;

        // 测试自定义默认版本
        std::vector<int> test1 = {1, 1, 2, 3, 3, 4, 5, 5, 5};
        std::cout << "自定义 unique 测试:" << std::endl;
        std::cout << "原数组: ";
        for (int n : test1) std::cout << n << " ";
        std::cout << std::endl;

        auto my_it = my_unique(test1.begin(), test1.end());
        std::cout << "去重后: ";
        for (auto iter = test1.begin(); iter != my_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        // 测试自定义谓词版本
        std::vector<double> test2 = {1.1, 1.2, 2.1, 2.9, 3.1, 3.8, 4.0};
        std::cout << "\n自定义谓词测试 (整数部分相同视为重复):" << std::endl;
        std::cout << "原数组: ";
        for (double d : test2) std::cout << d << " ";
        std::cout << std::endl;

        auto pred_it =
            my_unique(test2.begin(), test2.end(), [](double a, double b) {
                return static_cast<int>(a) == static_cast<int>(b);
            });

        std::cout << "去重后: ";
        for (auto iter = test2.begin(); iter != pred_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;
    }
}  // namespace MY_IMPL

namespace PREDICATE_EXPLANATION {
    void task() {
        std::cout << "\n=== 谓词函数详解 ===" << std::endl;
        std::cout << "BinaryPredicate p 的作用：" << std::endl;
        std::cout << "- 当 p(a, b) 返回 true 时，认为 a 和 b 是\"相等\"的"
                  << std::endl;
        std::cout << "- 当 p(a, b) 返回 false 时，认为 a 和 b 是\"不同\"的"
                  << std::endl;
        std::cout << "- unique 会保留每组\"相等\"元素中的第一个" << std::endl;

        // 示例：自定义"相等"的定义
        std::vector<int> numbers = {10, 12, 20, 25, 30, 33, 40};
        std::cout << "\n示例：按十位数去重（10-19算一组，20-29算一组...）"
                  << std::endl;
        std::cout << "原数组: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << std::endl;

        auto it = std::unique(numbers.begin(), numbers.end(), [](int a, int b) {
            return a / 10 == b / 10;  // 十位数相同就认为"相等"
        });

        std::cout << "按十位数去重后: ";
        for (auto iter = numbers.begin(); iter != it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        // 另一个示例：字符串首字母相同
        std::vector<std::string> words = {"apple", "banana", "cherry",
                                          "apricot", "blueberry"};
        std::cout << "\n示例：按首字母去重" << std::endl;
        std::cout << "原字符串: ";
        for (const auto& word : words) std::cout << word << " ";
        std::cout << std::endl;

        auto word_it =
            std::unique(words.begin(), words.end(),
                        [](const std::string& a, const std::string& b) {
                            return !a.empty() && !b.empty() && a[0] == b[0];
                        });

        std::cout << "按首字母去重后: ";
        for (auto iter = words.begin(); iter != word_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;
    }
}  // namespace PREDICATE_EXPLANATION

namespace ADVANCED_USAGE {
    void task() {
        std::cout << "\n=== UNIQUE 高级用法和注意事项 ===" << std::endl;

        // 1. unique 只处理连续的重复元素
        std::vector<int> unsorted = {1, 3, 1, 2, 3, 2, 1};
        std::cout << "重要：unique 只处理连续重复!" << std::endl;
        std::cout << "原数组（未排序）: ";
        for (int n : unsorted) std::cout << n << " ";
        std::cout << std::endl;

        auto unsorted_it = std::unique(unsorted.begin(), unsorted.end());
        std::cout << "直接 unique 结果: ";
        for (auto iter = unsorted.begin(); iter != unsorted_it; ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << " (注意：1,3,2都还存在!)" << std::endl;

        // 2. 正确的全局去重：先排序再unique
        std::vector<int> for_global_unique = {1, 3, 1, 2, 3, 2, 1};
        std::cout << "\n正确的全局去重方法：" << std::endl;
        std::cout << "原数组: ";
        for (int n : for_global_unique) std::cout << n << " ";
        std::cout << std::endl;

        std::sort(for_global_unique.begin(), for_global_unique.end());
        std::cout << "排序后: ";
        for (int n : for_global_unique) std::cout << n << " ";
        std::cout << std::endl;

        for_global_unique.erase(
            std::unique(for_global_unique.begin(), for_global_unique.end()),
            for_global_unique.end());
        std::cout << "全局去重后: ";
        for (int n : for_global_unique) std::cout << n << " ";
        std::cout << std::endl;

        // 3. unique 与 remove 的区别
        std::cout << "\nunique vs remove 的区别:" << std::endl;
        std::cout << "- unique: 移除连续的重复元素，保留每组的第一个"
                  << std::endl;
        std::cout << "- remove: 移除所有等于特定值的元素" << std::endl;

        std::vector<int> demo1 = {1, 1, 2, 2, 3, 3};
        std::vector<int> demo2 = {1, 1, 2, 2, 3, 3};

        auto unique_end = std::unique(demo1.begin(), demo1.end());
        auto remove_end = std::remove(demo2.begin(), demo2.end(), 2);

        std::cout << "unique 结果: ";
        for (auto it = demo1.begin(); it != unique_end; ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\nremove(2) 结果: ";
        for (auto it = demo2.begin(); it != remove_end; ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
}  // namespace ADVANCED_USAGE

int main() {
    std::vector<int> numbers = {1, 2, 2, 3, 4, 4, 5, 6, 6};

    std::cout << "=== 基本示例 ===" << std::endl;
    std::cout << "原数组: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    auto it = std::unique(numbers.begin(), numbers.end());

    std::cout << "去重后的数组: ";
    std::copy(numbers.begin(), it, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    // 调用详细示例
    STD_USAGE::task();
    MY_IMPL::task();
    PREDICATE_EXPLANATION::task();
    ADVANCED_USAGE::task();

    return 0;
}