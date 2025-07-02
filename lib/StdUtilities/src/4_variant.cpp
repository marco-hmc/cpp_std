#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct VariantVisitor {
    void operator()(int value) const {
        std::cout << "处理整数: " << value << std::endl;
    }

    void operator()(double value) const {
        std::cout << "处理浮点数: " << value << std::endl;
    }

    void operator()(const std::string& value) const {
        std::cout << "处理字符串: " << value << std::endl;
    }
};

int main() {
    using MyVariant = std::variant<int, double, std::string>;
    std::vector<MyVariant> variants = {42, 3.14, "Hello, variant!"};

    for (const auto& var : variants) {
        std::visit(VariantVisitor{}, var);
        std::visit(
            [](const auto& value) {
                std::cout << "值类型: " << typeid(value).name() << std::endl;
            },
            var);

        std::cout << "-------------------" << std::endl;
    }

    MyVariant var = 123;
    try {
        int intValue = std::get<int>(var);
        std::cout << "获取整数值: " << intValue << std::endl;
    } catch (const std::bad_variant_access& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }

    var = 3.14159;
    if (var.index() == 1) {
        std::cout << "通过索引获取 double 值: " << std::get<1>(var)
                  << std::endl;
    }

    return 0;
}