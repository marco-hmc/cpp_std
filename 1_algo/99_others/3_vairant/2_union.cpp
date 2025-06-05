#include <cstring>
#include <iostream>

/*
    union 的所有成员共享同一个内存位置，因此 union 的第一个成员的地址就是整个 union 的地址。
    访问 union 的不同成员实际上都是从同一个内存地址开始读取，只是读取时指定了不同的类型。
*/

union Data {
    int intValue;
    float floatValue;
    char strValue[20];
};

int main() {
    Data data{};

    data.floatValue = 3.14f;
    std::cout << "data.intValue: " << data.floatValue << std::endl;

    float floatValue = *reinterpret_cast<float*>(&data.intValue);
    std::cout << "data.floatValue (reinterpret_cast): " << floatValue
              << std::endl;

    return 0;
}