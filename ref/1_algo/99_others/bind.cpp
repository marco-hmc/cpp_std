#include <functional>
#include <iostream>

// 一个简单的函数，接受两个参数
void printSum(int a, int b) { std::cout << "Sum: " << a + b << std::endl; }

int main() {
    // 使用std::bind将printSum的第一个参数绑定为10
    auto boundFunction = std::bind(printSum, 10, std::placeholders::_1);

    // 调用绑定后的函数，只需要传递一个参数
    boundFunction(20);  // 输出: Sum: 30

    return 0;
}