#include <math.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void basicUsage() {
    std::cout << "\n===== 示例1：基本用法 =====\n";

    std::string data = "123 3.14 Hello";
    std::istringstream iss(data);

    int i = 0;
    double d = NAN;
    std::string s;
    iss >> i >> d >> s;

    std::cout << "解析整数: " << i << std::endl;
    std::cout << "解析浮点数: " << d << std::endl;
    std::cout << "解析字符串: " << s << std::endl;
}

void stringSplit() {
    std::cout << "\n===== 示例2：字符串分割 =====\n";

    std::string sentence = "Hello world this is a test";
    std::istringstream iss(sentence);

    std::string word;
    std::vector<std::string> words;

    while (iss >> word) {
        words.push_back(word);
    }

    std::cout << "分割结果:" << std::endl;
    for (size_t i = 0; i < words.size(); ++i) {
        std::cout << i << ": " << words[i] << std::endl;
    }
}

void parseCSV() {
    std::cout << "\n===== 示例3：处理CSV格式 =====\n";
    std::string csvLine = "Alice,30,New York";
    std::istringstream iss(csvLine);

    std::string name, age, city;
    std::getline(iss, name, ',');
    std::getline(iss, age, ',');
    std::getline(iss, city, ',');

    std::cout << "名字: " << name << std::endl;
    std::cout << "年龄: " << age << std::endl;
    std::cout << "城市: " << city << std::endl;
}

void reuseStream() {
    std::cout << "\n===== 示例4：重用istringstream =====\n";
    std::istringstream iss;
    iss.str("42 73");

    int a, b;
    iss >> a >> b;
    std::cout << "第一次读取: " << a << ", " << b << std::endl;

    iss.clear();
    iss.str("100 200");
    iss >> a >> b;
    std::cout << "第二次读取: " << a << ", " << b << std::endl;
}

void errorHandling() {
    std::cout << "\n===== 示例5：错误处理和状态检查 =====\n";
    std::string data = "123 abc 456";
    std::istringstream iss(data);

    int a, b;
    iss >> a;
    iss >> b;
    if (iss.fail()) {
        std::cout << "读取失败: 无法将'abc'转换为整数" << std::endl;
        iss.clear();
        std::string dummy;
        iss >> dummy;
        std::cout << "跳过的内容: " << dummy << std::endl;
    }

    if (iss >> b) {
        std::cout << "继续读取成功: " << b << std::endl;
    }
}

void parseFormattedData() {
    std::cout << "\n===== 示例6：解析具有固定格式的数据 =====\n";
    std::string data = "Point(10,20) Circle(15,30,5)";
    std::istringstream iss(data);

    std::string type;
    int x, y, radius;
    char c;

    iss >> type >> c >> x >> c >> y >> c;
    std::cout << "类型: " << type << ", x = " << x << ", y = " << y
              << std::endl;

    iss >> type >> c >> x >> c >> y >> c >> radius >> c;
    std::cout << "类型: " << type << ", x = " << x << ", y = " << y
              << ", 半径 = " << radius << std::endl;
}

int main() {
    std::cout << "=== istringstream 用法示例 ===\n";

    // basicUsage();
    // stringSplit();
    // parseCSV();
    // reuseStream();
    // errorHandling();
    parseFormattedData();

    return 0;
}
