// std::merge 用法详解和自定义实现

#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

using std::deque;
using dict_entry = std::pair<std::string, std::string>;

namespace std {
    std::ostream& operator<<(std::ostream& os, const dict_entry p) {
        return os << p.first << " " << p.second;
    }
    std::istream& operator>>(std::istream& is, dict_entry& p) {
        return is >> p.first >> p.second;
    }
}  // namespace std

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::MERGE 标准用法 ===" << std::endl;

        // 1. 基本合并 - 两个有序数组
        std::vector<int> vec1 = {1, 3, 5, 7, 9};
        std::vector<int> vec2 = {2, 4, 6, 8, 10};
        std::vector<int> merged;

        std::cout << "基本合并两个有序数组:" << std::endl;
        std::cout << "数组1: ";
        for (int n : vec1) std::cout << n << " ";
        std::cout << "\n数组2: ";
        for (int n : vec2) std::cout << n << " ";
        std::cout << std::endl;

        std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(),
                   std::back_inserter(merged));

        std::cout << "合并结果: ";
        for (int n : merged) std::cout << n << " ";
        std::cout << std::endl;

        // 2. 不同长度的数组合并
        std::vector<int> short_vec = {1, 5, 9};
        std::vector<int> long_vec = {2, 3, 4, 6, 7, 8, 10, 11};
        std::vector<int> unequal_merged;

        std::cout << "\n不同长度数组合并:" << std::endl;
        std::cout << "短数组: ";
        for (int n : short_vec) std::cout << n << " ";
        std::cout << "\n长数组: ";
        for (int n : long_vec) std::cout << n << " ";
        std::cout << std::endl;

        std::merge(short_vec.begin(), short_vec.end(), long_vec.begin(),
                   long_vec.end(), std::back_inserter(unequal_merged));

        std::cout << "合并结果: ";
        for (int n : unequal_merged) std::cout << n << " ";
        std::cout << std::endl;

        // 3. 字符串合并
        std::vector<std::string> words1 = {"apple", "cherry", "grape"};
        std::vector<std::string> words2 = {"banana", "date", "fig"};
        std::vector<std::string> word_merged;

        std::cout << "\n字符串合并:" << std::endl;
        std::merge(words1.begin(), words1.end(), words2.begin(), words2.end(),
                   std::back_inserter(word_merged));

        std::cout << "合并结果: ";
        for (const auto& word : word_merged) std::cout << word << " ";
        std::cout << std::endl;

        // 4. 自定义比较器 - 降序合并
        std::vector<int> desc1 = {9, 7, 5, 3, 1};
        std::vector<int> desc2 = {8, 6, 4, 2};
        std::vector<int> desc_merged;

        std::cout << "\n使用自定义比较器(降序):" << std::endl;
        std::merge(desc1.begin(), desc1.end(), desc2.begin(), desc2.end(),
                   std::back_inserter(desc_merged), std::greater<int>());

        std::cout << "降序合并结果: ";
        for (int n : desc_merged) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 merge
    template <class InputIt1, class InputIt2, class OutputIt>
    OutputIt my_merge(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                      InputIt2 last2, OutputIt d_first) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 <= *first2) {
                *d_first++ = *first1++;
            } else {
                *d_first++ = *first2++;
            }
        }

        // 复制剩余元素
        while (first1 != last1) {
            *d_first++ = *first1++;
        }
        while (first2 != last2) {
            *d_first++ = *first2++;
        }

        return d_first;
    }

    // 自定义实现 merge (带比较器)
    template <class InputIt1, class InputIt2, class OutputIt, class Compare>
    OutputIt my_merge(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                      InputIt2 last2, OutputIt d_first, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                *d_first++ = *first1++;
            } else {
                *d_first++ = *first2++;
            }
        }

        // 复制剩余元素
        while (first1 != last1) {
            *d_first++ = *first1++;
        }
        while (first2 != last2) {
            *d_first++ = *first2++;
        }

        return d_first;
    }

    void task() {
        std::cout << "\n=== 自定义 MERGE 实现测试 ===" << std::endl;

        // 测试自定义基本版本
        std::vector<int> test1 = {1, 4, 7, 10};
        std::vector<int> test2 = {2, 3, 5, 8, 9};
        std::vector<int> my_result;

        std::cout << "自定义 merge 测试:" << std::endl;
        std::cout << "数组1: ";
        for (int n : test1) std::cout << n << " ";
        std::cout << "\n数组2: ";
        for (int n : test2) std::cout << n << " ";
        std::cout << std::endl;

        my_merge(test1.begin(), test1.end(), test2.begin(), test2.end(),
                 std::back_inserter(my_result));

        std::cout << "自定义实现结果: ";
        for (int n : my_result) std::cout << n << " ";
        std::cout << std::endl;

        // 对比标准实现
        std::vector<int> std_result;
        std::merge(test1.begin(), test1.end(), test2.begin(), test2.end(),
                   std::back_inserter(std_result));

        std::cout << "标准实现结果: ";
        for (int n : std_result) std::cout << n << " ";
        std::cout << std::endl;

        bool same =
            std::equal(my_result.begin(), my_result.end(), std_result.begin());
        std::cout << "结果一致: " << (same ? "是" : "否") << std::endl;
    }
}  // namespace MY_IMPL

namespace INPLACE_MERGE {
    void task() {
        std::cout << "\n=== INPLACE_MERGE 就地合并 ===" << std::endl;

        // inplace_merge 合并同一容器中的两个有序部分
        std::vector<int> data = {1, 3, 5, 7, 2, 4, 6, 8};

        std::cout << "就地合并说明:" << std::endl;
        std::cout << "原数据: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 分别对两部分排序
        auto middle = data.begin() + 4;
        std::sort(data.begin(), middle);
        std::sort(middle, data.end());

        std::cout << "分别排序后: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 就地合并
        std::inplace_merge(data.begin(), middle, data.end());

        std::cout << "就地合并后: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        std::cout << "\ninplace_merge vs merge 区别:" << std::endl;
        std::cout
            << "- inplace_merge: 合并同一容器中的两个有序部分，不需要额外空间"
            << std::endl;
        std::cout << "- merge: 合并两个独立的有序容器，需要输出容器"
                  << std::endl;
    }
}  // namespace INPLACE_MERGE

namespace MERGE_SORT_DEMO {
    void task() {
        std::cout << "\n=== 归并排序演示 ===" << std::endl;

        // 使用 merge 实现简单的归并排序
        auto merge_sort = [](std::vector<int>& arr, int left,
                             int right) -> void {
            std::function<void(std::vector<int>&, int, int)> merge_sort_impl;
            merge_sort_impl = [&](std::vector<int>& arr, int left, int right) {
                if (left >= right) return;

                int mid = left + (right - left) / 2;
                merge_sort_impl(arr, left, mid);
                merge_sort_impl(arr, mid + 1, right);

                // 使用 inplace_merge 合并
                std::inplace_merge(arr.begin() + left, arr.begin() + mid + 1,
                                   arr.begin() + right + 1);
            };
            merge_sort_impl(arr, left, right);
        };

        std::vector<int> unsorted = {64, 34, 25, 12, 22, 11,
                                     90, 88, 76, 50, 42};
        std::cout << "归并排序演示:" << std::endl;
        std::cout << "原数组: ";
        for (int n : unsorted) std::cout << n << " ";
        std::cout << std::endl;

        merge_sort(unsorted, 0, unsorted.size() - 1);

        std::cout << "排序后: ";
        for (int n : unsorted) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace MERGE_SORT_DEMO

namespace ADVANCED_USAGE {
    void task() {
        std::cout << "\n=== MERGE 高级用法 ===" << std::endl;

        // 1. 合并不同容器类型
        std::vector<int> vec = {1, 3, 5};
        std::list<int> lst = {2, 4, 6};
        std::deque<int> result;

        std::cout << "合并不同容器类型:" << std::endl;
        std::merge(vec.begin(), vec.end(), lst.begin(), lst.end(),
                   std::back_inserter(result));

        std::cout << "vector + list -> deque: ";
        for (int n : result) std::cout << n << " ";
        std::cout << std::endl;

        // 2. 复杂对象合并
        struct Person {
            std::string name;
            int age;

            Person(const std::string& n, int a) : name(n), age(a) {}
        };

        std::vector<Person> group1 = {{"Alice", 20}, {"Charlie", 25}};
        std::vector<Person> group2 = {{"Bob", 22}, {"David", 30}};
        std::vector<Person> all_people;

        std::cout << "\n复杂对象合并 (按年龄排序):" << std::endl;
        std::merge(
            group1.begin(), group1.end(), group2.begin(), group2.end(),
            std::back_inserter(all_people),
            [](const Person& a, const Person& b) { return a.age < b.age; });

        std::cout << "合并后: ";
        for (const auto& p : all_people) {
            std::cout << p.name << "(" << p.age << ") ";
        }
        std::cout << std::endl;

        // 3. 性能提示
        std::cout << "\n性能特性:" << std::endl;
        std::cout << "- merge 时间复杂度: O(m + n)" << std::endl;
        std::cout << "- inplace_merge 时间复杂度: O((m+n)log(m+n)) 或 O(m+n) "
                     "(如果有额外内存)"
                  << std::endl;
        std::cout << "- 前提条件: 输入序列必须已排序" << std::endl;
        std::cout << "- merge 是稳定的，相等元素的相对顺序保持不变"
                  << std::endl;
    }
}  // namespace ADVANCED_USAGE

template <typename IS>
deque<dict_entry> from_instream(IS&& is) {
    std::deque<dict_entry> d{std::istream_iterator<dict_entry>{is}, {}};
    std::sort(std::begin(d), std::end(d));
    return d;
}

int main() {
    std::cout << "=== 原始示例（模拟文件输入）===" << std::endl;
    // 模拟文件内容而不是实际读取文件
    std::vector<dict_entry> dict1_data = {
        {"apple", "苹果"}, {"cat", "猫"}, {"dog", "狗"}};
    std::vector<dict_entry> dict2_data = {{"bird", "鸟"}, {"fish", "鱼"}};

    std::cout << "字典1: ";
    for (const auto& entry : dict1_data) std::cout << entry << " ";
    std::cout << "\n字典2: ";
    for (const auto& entry : dict2_data) std::cout << entry << " ";
    std::cout << "\n合并结果:" << std::endl;

    std::merge(dict1_data.begin(), dict1_data.end(), dict2_data.begin(),
               dict2_data.end(),
               std::ostream_iterator<dict_entry>{std::cout, "\n"});

    // 调用详细示例
    STD_USAGE::task();
    MY_IMPL::task();
    INPLACE_MERGE::task();
    MERGE_SORT_DEMO::task();
    ADVANCED_USAGE::task();

    return 0;
}