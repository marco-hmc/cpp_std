// 表示数值的字符串
//
// 题目描述：
// 请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。例如，字符串
// "+100", "5e2", "-123", "3.1416"和"-1E-16"都表示数值。
// 但是"12e", "1a3.14", "1.2.3", "+-5"和"12e+4.3"都不是。

#include <iostream>
#include <string>

bool isNumeric(char* string) {
    char* s = string;
    while (*s == ' ') s++;
    if (*s == '+' || *s == '-') s++;
    int pointNum = 0;
    int num = 0;
    while (*s >= '0' && *s <= '9' || *s == '.') {
        *s == '.' ? pointNum++ : num++;
        s++;
    }
    // 如果小数点的数目大于1  或者数字为0,则不是数字（包含全空格）
    if (num < 1 || pointNum > 1) {
        return false;
    }
    //科学计数法
    if (*s == 'e' || *s == 'E') {
        s++;
        if (*s == '+' || *s == '-') {
            s++;
        }
        // 统计e后面数字，e后面只能包含数字
        int eNum = 0;
        while (*s >= '0' && *s <= '9') {
            eNum++;
            s++;
        }
        if (eNum == 0) {
            return false;
        }
    }
    // 去除后面的空格
    while (*s == ' ') {
        s++;
    }
    // 没到字符串结尾，必然不是合法数字表示
    return *s == '\0';
}

// 测试isNumeric函数的正确性
bool testValid() {
    // 测试合法的数值
    char valid1[] = "+100";
    char valid2[] = "5e2";
    char valid3[] = "-123";
    char valid4[] = "3.1416";
    char valid5[] = "-1E-16";

    // 测试非法的数值
    char invalid1[] = "12e";
    char invalid2[] = "1a3.14";
    char invalid3[] = "1.2.3";
    char invalid4[] = "+-5";
    char invalid5[] = "12e+4.3";

    // 检查合法的数值是否都返回true
    bool allValidCorrect = isNumeric(valid1) && isNumeric(valid2) &&
                           isNumeric(valid3) && isNumeric(valid4) &&
                           isNumeric(valid5);

    // 检查非法的数值是否都返回false
    bool allInvalidCorrect = !isNumeric(invalid1) && !isNumeric(invalid2) &&
                             !isNumeric(invalid3) && !isNumeric(invalid4) &&
                             !isNumeric(invalid5);

    // 输出测试结果
    std::cout << "合法数值测试: " << (allValidCorrect ? "通过" : "失败")
              << std::endl;
    std::cout << "非法数值测试: " << (allInvalidCorrect ? "通过" : "失败")
              << std::endl;

    return allValidCorrect && allInvalidCorrect;
}

int main() {
    if (testValid()) {
        std::cout << "所有测试通过!" << std::endl;
    } else {
        std::cout << "测试失败!" << std::endl;
    }
    return 0;
}
