// 括号的分数
//
// 题目描述：
// 给定一个平衡括号字符串 s，按下述规则计算该字符串的分数：
// 1. () 的分数为 1
// 2. AB 的分数为 A + B，其中 A 和 B 是平衡括号字符串
// 3. (A) 的分数为 2 * A，其中 A 是平衡括号字符串
//
// 例如：
// "()" 的分数是 1
// "(())" 的分数是 2
// "()()" 的分数是 1 + 1 = 2
// "(()(()))" 的分数是 2 * (1 + 2 * 1) = 2 * 3 = 6

#include <algorithm>
#include <iostream>
#include <stack>
#include <string>

int scoreOfParentheses(std::string s) {
    std::stack<int> stk;
    stk.push(0);
    for (auto ch : s) {
        if (ch == '(') {
            stk.push(0);
        } else {
            int cur = stk.top();
            stk.pop();
            int last = stk.top();
            stk.pop();
            stk.push(std::max(cur * 2, 1) + last);
        }
    }
    return stk.top();
}

// 测试scoreOfParentheses函数的正确性
bool testValid() {
    // 测试用例
    std::string test1 = "()";
    int expected1 = 1;

    std::string test2 = "(())";
    int expected2 = 2;

    std::string test3 = "()()";
    int expected3 = 2;

    std::string test4 = "(()(()))";
    int expected4 = 6;

    std::string test5 = "(()())";
    int expected5 = 4;

    // 检查计算结果是否符合预期
    bool test1Result = scoreOfParentheses(test1) == expected1;
    bool test2Result = scoreOfParentheses(test2) == expected2;
    bool test3Result = scoreOfParentheses(test3) == expected3;
    bool test4Result = scoreOfParentheses(test4) == expected4;
    bool test5Result = scoreOfParentheses(test5) == expected5;

    // 输出测试结果
    std::cout << "测试用例1 (\"" << test1
              << "\"): " << (test1Result ? "通过" : "失败")
              << " 结果: " << scoreOfParentheses(test1) << std::endl;
    std::cout << "测试用例2 (\"" << test2
              << "\"): " << (test2Result ? "通过" : "失败")
              << " 结果: " << scoreOfParentheses(test2) << std::endl;
    std::cout << "测试用例3 (\"" << test3
              << "\"): " << (test3Result ? "通过" : "失败")
              << " 结果: " << scoreOfParentheses(test3) << std::endl;
    std::cout << "测试用例4 (\"" << test4
              << "\"): " << (test4Result ? "通过" : "失败")
              << " 结果: " << scoreOfParentheses(test4) << std::endl;
    std::cout << "测试用例5 (\"" << test5
              << "\"): " << (test5Result ? "通过" : "失败")
              << " 结果: " << scoreOfParentheses(test5) << std::endl;

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
