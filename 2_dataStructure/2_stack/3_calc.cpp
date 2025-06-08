// https://leetcode-cn.com/problems/basic-calculator-ii/
// 基本计算器 II
//
// 题目描述：
// 给你一个字符串表达式 s ，请你实现一个基本计算器来计算并返回它的值。
// 整数除法仅保留整数部分。
// 你可以假设给定的表达式总是有效的。所有中间结果将在 [-2^31, 2^31 - 1] 的范围内。
//
// 注意：不允许使用任何将字符串作为数学表达式计算的内置函数，比如 eval() 。
//
// 示例：
// 输入：s = "3+2*2"
// 输出：7
//
// 输入：s = " 3/2 "
// 输出：1
//
// 输入：s = " 3+5 / 2 "
// 输出：5

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int calculate(std::string s) {
    int n = s.size();
    int num = 0;
    char sign = '+';
    std::vector<int> stk;
    for (int i = 0; i < n; ++i) {
        char ch = s[i];
        if (isdigit(ch)) {
            num = num * 10 + (ch - '0');
        }
        if ((!isdigit(ch) && ch != ' ') || i == n - 1) {
            switch (sign) {
                case '+':
                    stk.push_back(num);
                    break;
                case '-':
                    stk.push_back(-num);
                    break;
                case '*':
                    stk.back() *= num;
                    break;
                case '/':
                    stk.back() /= num;
                    break;
            }
            sign = ch;
            num = 0;
        }
    }
    return std::accumulate(stk.begin(), stk.end(), 0);
}

// 测试calculate函数的正确性
bool testValid() {
    // 测试用例
    std::string test1 = "3+2*2";
    int expected1 = 7;

    std::string test2 = " 3/2 ";
    int expected2 = 1;

    std::string test3 = " 3+5 / 2 ";
    int expected3 = 5;

    std::string test4 = "14-3/2";
    int expected4 = 13;

    std::string test5 = "2*3+4";
    int expected5 = 10;

    // 检查计算结果是否符合预期
    bool test1Result = calculate(test1) == expected1;
    bool test2Result = calculate(test2) == expected2;
    bool test3Result = calculate(test3) == expected3;
    bool test4Result = calculate(test4) == expected4;
    bool test5Result = calculate(test5) == expected5;

    // 输出测试结果
    std::cout << "测试用例1 (\"" << test1
              << "\"): " << (test1Result ? "通过" : "失败")
              << " 结果: " << calculate(test1) << std::endl;
    std::cout << "测试用例2 (\"" << test2
              << "\"): " << (test2Result ? "通过" : "失败")
              << " 结果: " << calculate(test2) << std::endl;
    std::cout << "测试用例3 (\"" << test3
              << "\"): " << (test3Result ? "通过" : "失败")
              << " 结果: " << calculate(test3) << std::endl;
    std::cout << "测试用例4 (\"" << test4
              << "\"): " << (test4Result ? "通过" : "失败")
              << " 结果: " << calculate(test4) << std::endl;
    std::cout << "测试用例5 (\"" << test5
              << "\"): " << (test5Result ? "通过" : "失败")
              << " 结果: " << calculate(test5) << std::endl;

    return test1Result && test2Result && test3Result && test4Result &&
           test5Result;
}

int main() {
    if (testValid()) {
        std::cout << "所有测试通过!" << std::endl;
    } else {
        std::cout << "测试失败!" << std::endl;
    }
    return 0;
}
