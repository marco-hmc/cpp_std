// std::next_permutation 用法详解和自定义实现

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::NEXT_PERMUTATION 标准用法 ===" << std::endl;

        // 1. 基本用法 - 数字排列
        std::vector<int> numbers = {1, 2, 3};
        std::cout << "数字 {1, 2, 3} 的所有排列:" << std::endl;

        do {
            for (int n : numbers) std::cout << n << " ";
            std::cout << std::endl;
        } while (std::next_permutation(numbers.begin(), numbers.end()));

        // 2. 字符串排列
        std::string str = "abc";
        std::cout << "\n字符串 \"abc\" 的所有排列:" << std::endl;

        do {
            std::cout << str << std::endl;
        } while (std::next_permutation(str.begin(), str.end()));

        // 3. prev_permutation - 前一个排列
        std::vector<int> nums = {3, 2, 1};
        std::cout << "\n使用 prev_permutation (从 {3, 2, 1} 开始):"
                  << std::endl;

        do {
            for (int n : nums) std::cout << n << " ";
            std::cout << std::endl;
        } while (std::prev_permutation(nums.begin(), nums.end()));

        // 4. 自定义比较器
        std::vector<int> custom = {1, 2, 3};
        std::cout << "\n使用自定义比较器 (降序):" << std::endl;

        std::sort(custom.begin(), custom.end(), std::greater<int>());
        do {
            for (int n : custom) std::cout << n << " ";
            std::cout << std::endl;
        } while (std::next_permutation(custom.begin(), custom.end(),
                                       std::greater<int>()));
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 next_permutation
    template <class BidirIt>
    bool my_next_permutation(BidirIt first, BidirIt last) {
        if (first == last) return false;

        BidirIt i = last;
        if (first == --i) return false;

        while (true) {
            BidirIt i1, i2;

            i1 = i;
            if (*--i < *i1) {
                i2 = last;
                while (!(*i < *--i2));
                std::iter_swap(i, i2);
                std::reverse(i1, last);
                return true;
            }
            if (i == first) {
                std::reverse(first, last);
                return false;
            }
        }
    }

    // 自定义实现 prev_permutation
    template <class BidirIt>
    bool my_prev_permutation(BidirIt first, BidirIt last) {
        if (first == last) return false;

        BidirIt i = last;
        if (first == --i) return false;

        while (true) {
            BidirIt i1, i2;

            i1 = i;
            if (*i1 < *--i) {
                i2 = last;
                while (!(*--i2 < *i));
                std::iter_swap(i, i2);
                std::reverse(i1, last);
                return true;
            }
            if (i == first) {
                std::reverse(first, last);
                return false;
            }
        }
    }

    void task() {
        std::cout << "\n=== 自定义 NEXT_PERMUTATION 实现测试 ===" << std::endl;

        // 测试自定义 next_permutation
        std::vector<int> test = {1, 2, 3};
        std::cout << "自定义 next_permutation 测试:" << std::endl;

        do {
            for (int n : test) std::cout << n << " ";
            std::cout << std::endl;
        } while (my_next_permutation(test.begin(), test.end()));

        // 测试自定义 prev_permutation
        std::vector<int> test2 = {3, 2, 1};
        std::cout << "\n自定义 prev_permutation 测试:" << std::endl;

        do {
            for (int n : test2) std::cout << n << " ";
            std::cout << std::endl;
        } while (my_prev_permutation(test2.begin(), test2.end()));
    }
}  // namespace MY_IMPL

namespace ALGORITHM_EXPLANATION {
    void task() {
        std::cout << "\n=== NEXT_PERMUTATION 算法原理详解 ===" << std::endl;

        std::cout << "算法步骤:" << std::endl;
        std::cout << "1. 从右向左找到第一个 arr[i] < arr[i+1] 的位置 i"
                  << std::endl;
        std::cout << "2. 如果找不到，说明已是最大排列，返回 false" << std::endl;
        std::cout << "3. 从右向左找到第一个 arr[j] > arr[i] 的位置 j"
                  << std::endl;
        std::cout << "4. 交换 arr[i] 和 arr[j]" << std::endl;
        std::cout << "5. 反转 arr[i+1] 到末尾的部分" << std::endl;

        // 演示算法步骤
        std::vector<int> demo = {1, 2, 3, 4};
        std::cout << "\n演示过程 (从 {1, 2, 3, 4} 到下一个排列):" << std::endl;

        std::cout << "当前排列: ";
        for (int n : demo) std::cout << n << " ";
        std::cout << std::endl;

        // 手动执行算法步骤
        auto it = demo.end() - 1;
        while (it != demo.begin() && *(it - 1) >= *it) --it;

        if (it != demo.begin()) {
            auto pivot = it - 1;
            std::cout << "步骤1: 找到 pivot 位置 "
                      << std::distance(demo.begin(), pivot) << ", 值为 "
                      << *pivot << std::endl;

            auto successor = demo.end() - 1;
            while (*successor <= *pivot) --successor;
            std::cout << "步骤2: 找到 successor 位置 "
                      << std::distance(demo.begin(), successor) << ", 值为 "
                      << *successor << std::endl;

            std::swap(*pivot, *successor);
            std::cout << "步骤3: 交换后: ";
            for (int n : demo) std::cout << n << " ";
            std::cout << std::endl;

            std::reverse(it, demo.end());
            std::cout << "步骤4: 反转后: ";
            for (int n : demo) std::cout << n << " ";
            std::cout << std::endl;
        }
    }
}  // namespace ALGORITHM_EXPLANATION

namespace PRACTICAL_APPLICATIONS {
    void task() {
        std::cout << "\n=== PERMUTATION 实际应用 ===" << std::endl;

        // 1. 计算排列总数
        std::vector<int> items = {1, 2, 3, 4};
        int count = 0;

        std::cout << "计算排列总数:" << std::endl;
        do {
            ++count;
        } while (std::next_permutation(items.begin(), items.end()));

        std::cout << "4个元素的排列总数: " << count << std::endl;

        // 2. 查找特定排列
        std::vector<int> target = {1, 2, 3, 4};
        std::vector<int> search = {3, 1, 4, 2};
        int position = 0;

        std::cout << "\n查找排列 {3, 1, 4, 2} 的位置:" << std::endl;
        do {
            ++position;
            if (target == search) {
                std::cout << "找到排列，位置: " << position << std::endl;
                break;
            }
        } while (std::next_permutation(target.begin(), target.end()));

        // 3. 生成组合问题的解
        std::string chars = "abcd";
        std::cout << "\n生成长度为2的组合:" << std::endl;

        // 使用 next_permutation 生成组合
        std::vector<bool> selector = {true, true, false, false};
        do {
            for (int i = 0; i < chars.size(); ++i) {
                if (selector[i]) std::cout << chars[i];
            }
            std::cout << " ";
        } while (std::next_permutation(selector.begin(), selector.end()));
        std::cout << std::endl;

        // 4. 性能提示
        std::cout << "\n性能提示:" << std::endl;
        std::cout << "- next_permutation 时间复杂度平均 O(1)，最坏 O(n)"
                  << std::endl;
        std::cout << "- 生成所有排列的时间复杂度为 O(n! * n)" << std::endl;
        std::cout << "- 对于大数据集，考虑使用迭代器或生成器模式" << std::endl;
        std::cout << "- 排列数量随n急剧增长：5!=120, 10!=3628800" << std::endl;
    }
}  // namespace PRACTICAL_APPLICATIONS

namespace EDGE_CASES {
    void task() {
        std::cout << "\n=== 边界情况和注意事项 ===" << std::endl;

        // 1. 空容器
        std::vector<int> empty;
        bool result1 = std::next_permutation(empty.begin(), empty.end());
        std::cout << "空容器的 next_permutation 结果: "
                  << (result1 ? "true" : "false") << std::endl;

        // 2. 单元素
        std::vector<int> single = {1};
        bool result2 = std::next_permutation(single.begin(), single.end());
        std::cout << "单元素的 next_permutation 结果: "
                  << (result2 ? "true" : "false") << std::endl;

        // 3. 重复元素
        std::vector<int> duplicates = {1, 1, 2};
        std::cout << "\n重复元素 {1, 1, 2} 的排列:" << std::endl;

        do {
            for (int n : duplicates) std::cout << n << " ";
            std::cout << std::endl;
        } while (std::next_permutation(duplicates.begin(), duplicates.end()));

        // 4. 已经是最大排列
        std::vector<int> maximum = {3, 2, 1};
        std::cout << "\n已经是最大排列 {3, 2, 1}:" << std::endl;
        bool result3 = std::next_permutation(maximum.begin(), maximum.end());
        std::cout << "next_permutation 结果: " << (result3 ? "true" : "false")
                  << std::endl;
        std::cout << "排列变为: ";
        for (int n : maximum) std::cout << n << " ";
        std::cout << " (自动变为最小排列)" << std::endl;

        std::cout << "\n重要提醒:" << std::endl;
        std::cout << "- 必须先排序才能生成所有排列" << std::endl;
        std::cout << "- next_permutation 按字典序生成" << std::endl;
        std::cout << "- 当到达最大排列时，会重置为最小排列并返回 false"
                  << std::endl;
    }
}  // namespace EDGE_CASES

int main() {
    // 原始示例（注释掉交互式输入，改为预设数据）
    std::cout << "=== 原始示例（模拟输入 a b c）===" << std::endl;
    std::vector<std::string> v = {"a", "b", "c"};
    std::sort(std::begin(v), std::end(v));

    do {
        std::copy(std::begin(v), std::end(v),
                  std::ostream_iterator<std::string>{std::cout, ", "});
        std::cout << '\n';
    } while (std::next_permutation(std::begin(v), std::end(v)));

    // 调用详细示例
    STD_USAGE::task();
    MY_IMPL::task();
    ALGORITHM_EXPLANATION::task();
    PRACTICAL_APPLICATIONS::task();
    EDGE_CASES::task();

    return 0;
}