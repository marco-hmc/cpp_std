#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::TRANSFORM 标准用法 ===" << std::endl;

        // 1. 基本单元操作 - 平方
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::vector<int> squares;

        std::transform(numbers.begin(), numbers.end(),
                       std::back_inserter(squares),
                       [](int x) { return x * x; });

        std::cout << "原数组: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << "\n平方: ";
        for (int s : squares) std::cout << s << " ";
        std::cout << std::endl;

        // 2. 字符串转换 - 转大写
        std::vector<std::string> words = {"hello", "world", "cpp"};
        std::vector<std::string> upper_words;

        std::transform(words.begin(), words.end(),
                       std::back_inserter(upper_words),
                       [](const std::string& str) {
                           std::string result = str;
                           std::transform(result.begin(), result.end(),
                                          result.begin(), ::toupper);
                           return result;
                       });

        std::cout << "转大写: ";
        for (const auto& word : upper_words) std::cout << word << " ";
        std::cout << std::endl;

        // 3. 双输入版本 - 向量相加
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        std::vector<int> vec2 = {10, 20, 30, 40, 50};
        std::vector<int> sum_result;

        std::transform(vec1.begin(), vec1.end(), vec2.begin(),
                       std::back_inserter(sum_result),
                       [](int a, int b) { return a + b; });

        std::cout << "向量相加: ";
        for (int s : sum_result) std::cout << s << " ";
        std::cout << std::endl;

        // 4. 就地转换 (in-place)
        std::vector<int> data = {1, -2, 3, -4, 5};
        std::cout << "原数据: ";
        for (int d : data) std::cout << d << " ";

        std::transform(data.begin(), data.end(), data.begin(),
                       [](int x) { return std::abs(x); });

        std::cout << "\n取绝对值: ";
        for (int d : data) std::cout << d << " ";
        std::cout << std::endl;
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 transform (单输入版本)
    template <typename InputIt, typename OutputIt, typename UnaryOperation>
    OutputIt my_transform(InputIt first, InputIt last, OutputIt d_first,
                          UnaryOperation unary_op) {
        while (first != last) {
            *d_first++ = unary_op(*first++);
        }
        return d_first;
    }

    // 自定义实现 transform (双输入版本)
    template <typename InputIt1, typename InputIt2, typename OutputIt,
              typename BinaryOperation>
    OutputIt my_transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                          OutputIt d_first, BinaryOperation binary_op) {
        while (first1 != last1) {
            *d_first++ = binary_op(*first1++, *first2++);
        }
        return d_first;
    }

    void task() {
        std::cout << "\n=== 自定义 TRANSFORM 实现测试 ===" << std::endl;

        // 测试自定义单输入版本
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::vector<int> cubes;

        my_transform(numbers.begin(), numbers.end(), std::back_inserter(cubes),
                     [](int x) { return x * x * x; });

        std::cout << "立方运算: ";
        for (int c : cubes) std::cout << c << " ";
        std::cout << std::endl;

        // 测试自定义双输入版本
        std::vector<int> vec1 = {2, 4, 6, 8};
        std::vector<int> vec2 = {1, 3, 5, 7};
        std::vector<int> mult_result;

        my_transform(vec1.begin(), vec1.end(), vec2.begin(),
                     std::back_inserter(mult_result),
                     [](int a, int b) { return a * b; });

        std::cout << "向量相乘: ";
        for (int m : mult_result) std::cout << m << " ";
        std::cout << std::endl;
    }
}  // namespace MY_IMPL

int square(int x) {
    return x * x;
}

namespace BackInsertVsBegin {
    void task() {
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::vector<int> result;
        std::vector<int> result2(5);  // 预分配大小

        // back_inserter vs begin() 的区别说明：

        // 使用 back_inserter: 动态扩展容器
        // std::back_inserter 是一个迭代器适配器，它允许在容器的末尾插入元素，
        // 而不需要提前分配容器的大小。result 容器会自动根据需要进行扩展。
        std::transform(numbers.begin(), numbers.end(),
                       std::back_inserter(result), square);

        // 使用 begin(): 需要预分配大小
        // 使用 result2.begin() 来指定转换后的元素的起始位置，
        // 要求 result2 容器的大小必须足够大，以容纳转换后的元素。
        // 如果 result2 容器的大小不足，可能会导致未定义的行为。
        std::transform(numbers.begin(), numbers.end(), result2.begin(), square);

        std::cout << "=== 基本示例 ===" << std::endl;
        std::cout << "使用 back_inserter 结果: ";
        for (int num : result) {
            std::cout << num << " ";
        }
        std::cout << "\n使用 begin() 结果: ";
        for (int num : result2) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

}  // namespace BackInsertVsBegin

int main() {
    STD_USAGE::task();
    MY_IMPL::task();
    BackInsertVsBegin::task();
    return 0;
}