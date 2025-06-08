#include <algorithm>
#include <iostream>
#include <vector>

int square(int x) {
    return x * x;
}

int main() {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };
    std::vector<int> result;
    std::vector<int> result2;

    // 第一行代码使用了 std::back_inserter 来插入转换后的元素到 result容器中.
    // std::back_inserter是一个迭代器适配器,它允许在容器的末尾插入元素,而不需要提前分配容器的大小.
    // 这意味着    result 容器会自动根据需要进行扩展,以容纳转换后的元素.

    // 第二行代码使用了result2.begin()
    // 来指定转换后的元素的起始位置,并将转换后的元素直接存储到 result2
    // 容器中.这要求 result2容器的大小必须足够大,以容纳转换后的元素.如果
    // result2容器的大小不足,可能会导致未定义的行为或内存越界错误.

    // 第一个代码使用了动态扩展的容器result,
    // 而第二个代码使用了预先分配大小的容器result2
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(result), square);
    std::transform(numbers.begin(), numbers.end(), result2.begin(), square);

    // 输出结果
    for (int num : result) {
        std::cout << num << " ";
    }

    return 0;
}