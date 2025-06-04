// 字符串解码
//
// 题目描述：
// 给定一个经过编码的字符串，返回它解码后的字符串。
// 编码规则为: k[encoded_string]，表示其中方括号内部的encoded_string正好重复k次。
// 注意k保证为正整数。你可以认为输入字符串总是有效的；
// 输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。
// 此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数k，
// 例如不会出现像3a或2[4]的输入。

#include <iostream>
#include <stack>
#include <string>

std::string decodeString(std::string s) {
    int n = s.size();
    std::string ret, next;
    std::stack<std::pair<int, std::string> > stk;
    int num = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] <= '9' && s[i] >= '0') {
            num = num * 10 + s[i] - '0';
        } else if (s[i] == '[') {
            stk.emplace(num, ret);
            num = 0;  // 置零置空
            ret = "";
        } else if (s[i] == ']') {
            auto p = stk.top();
            stk.pop();
            std::string temp;
            while (p.first--) {
                temp += ret;
            }
            ret = p.second + temp;
        } else {
            ret += s[i];
        }
    }
    return ret;
}

// 测试decodeString函数的正确性
bool testValid() {
    // 测试用例
    std::string test1 = "3[a]2[bc]";
    std::string expected1 = "aaabcbc";

    std::string test2 = "3[a2[c]]";
    std::string expected2 = "accaccacc";

    std::string test3 = "2[abc]3[cd]ef";
    std::string expected3 = "abcabccdcdcdef";

    std::string test4 = "abc3[cd]xyz";
    std::string expected4 = "abccdcdcdxyz";

    // 检查解码结果是否符合预期
    bool test1Result = decodeString(test1) == expected1;
    bool test2Result = decodeString(test2) == expected2;
    bool test3Result = decodeString(test3) == expected3;
    bool test4Result = decodeString(test4) == expected4;

    // 输出测试结果
    std::cout << "测试用例1: " << (test1Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例2: " << (test2Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例3: " << (test3Result ? "通过" : "失败") << std::endl;
    std::cout << "测试用例4: " << (test4Result ? "通过" : "失败") << std::endl;

    return test1Result && test2Result && test3Result && test4Result;
}

int main() {
    if (testValid()) {
        std::cout << "所有测试通过!" << std::endl;
    } else {
        std::cout << "测试失败!" << std::endl;
    }
    return 0;
}
