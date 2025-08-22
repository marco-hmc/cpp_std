#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace mc {

    template <typename F, std::size_t... N>
    constexpr auto make_array_impl(F f, std::index_sequence<N...>)
        -> std::array<std::invoke_result_t<F, std::size_t>, sizeof...(N)> {
        return {std::invoke(f, std::integral_constant<decltype(N), N>{})...};
    }

    template <std::size_t N, typename F>
    constexpr auto make_array(F f)
        -> std::array<std::invoke_result_t<F, std::size_t>, N> {
        return make_array_impl(f, std::make_index_sequence<N>{});
    }

    template <typename T, typename Dst, typename... List>
    bool get_value_impl(const std::variant<List...>& v, Dst& dst) {
        if (std::holds_alternative<T>(v)) {
            if constexpr (std::is_convertible_v<T, Dst>) {
                dst = static_cast<Dst>(std::get<T>(v));
                return true;
            }
        }
        return false;
    }

    template <typename Dst, typename... List>
    bool get_value(const std::variant<List...>& v, Dst& dst) {
        using Variant = std::variant<List...>;
        using F = std::function<bool(const Variant&, Dst&)>;
        static auto _list = make_array<sizeof...(List)>([](auto i) -> F {
            return &get_value_impl<std::variant_alternative_t<i, Variant>, Dst,
                                   List...>;
        });
        return std::invoke(_list[v.index()], v, dst);
    }

}  // namespace mc

namespace variant_basic_transform {

    void basic_usage() {
        std::cout << "=== variant get_value 基础用法 ===" << std::endl;

        // 1. 基本类型转换
        std::variant<int, std::string, double> v1 = 42;
        std::variant<int, std::string, double> v2 = std::string("hello");
        std::variant<int, std::string, double> v3 = 3.14;

        // 获取值
        int int_val;
        std::string str_val;
        double double_val;

        if (mc::get_value(v1, int_val)) {
            std::cout << "v1 包含 int: " << int_val << std::endl;
        }

        if (mc::get_value(v2, str_val)) {
            std::cout << "v2 包含 string: " << str_val << std::endl;
        }

        if (mc::get_value(v3, double_val)) {
            std::cout << "v3 包含 double: " << double_val << std::endl;
        }
    }

    void type_conversion_usage() {
        std::cout << "\n=== variant get_value 类型转换 ===" << std::endl;

        std::variant<int, float, std::string> v;

        // int 到 double 的转换
        v = 42;
        double d;
        if (mc::get_value(v, d)) {
            std::cout << "int(42) -> double: " << d << std::endl;
        }

        // float 到 double 的转换
        v = 3.14f;
        if (mc::get_value(v, d)) {
            std::cout << "float(3.14) -> double: " << d << std::endl;
        }

        // 失败的转换示例
        v = std::string("not a number");
        if (!mc::get_value(v, d)) {
            std::cout << "string 无法转换为 double" << std::endl;
        }
    }

    void container_usage() {
        std::cout << "\n=== variant get_value 容器使用 ===" << std::endl;

        using VariantType = std::variant<int, std::string, double>;
        std::vector<VariantType> variants = {42, std::string("hello"), 3.14,
                                             std::string("world"), 100};

        // 提取所有字符串
        std::vector<std::string> strings;
        for (const auto& v : variants) {
            std::string str;
            if (mc::get_value(v, str)) {
                strings.push_back(str);
            }
        }

        std::cout << "提取的字符串: ";
        for (const auto& s : strings) {
            std::cout << "\"" << s << "\" ";
        }
        std::cout << std::endl;

        // 提取所有数值（转换为double）
        std::vector<double> numbers;
        for (const auto& v : variants) {
            double num;
            if (mc::get_value(v, num)) {
                numbers.push_back(num);
            }
        }

        std::cout << "提取的数值: ";
        for (double num : numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

}  // namespace variant_basic_transform

namespace variant_advanced_usage {

    // 自定义类型示例
    struct Point {
        double x, y;
        Point(double x = 0, double y = 0) : x(x), y(y) {}

        // 允许从double转换（用于演示）
        Point(double val) : x(val), y(val) {}

        friend std::ostream& operator<<(std::ostream& os, const Point& p) {
            return os << "Point(" << p.x << ", " << p.y << ")";
        }
    };

    struct Circle {
        Point center;
        double radius;
        Circle(const Point& c, double r) : center(c), radius(r) {}

        friend std::ostream& operator<<(std::ostream& os, const Circle& c) {
            return os << "Circle(center=" << c.center << ", radius=" << c.radius
                      << ")";
        }
    };

    void custom_type_usage() {
        std::cout << "\n=== variant get_value 自定义类型 ===" << std::endl;

        std::variant<int, Point, Circle, std::string> shape_variant;

        // 存储不同类型
        shape_variant = Point(1.0, 2.0);
        Point p;
        if (mc::get_value(shape_variant, p)) {
            std::cout << "获取到 Point: " << p << std::endl;
        }

        shape_variant = Circle(Point(0, 0), 5.0);
        Circle c(Point(0, 0), 0);
        if (mc::get_value(shape_variant, c)) {
            std::cout << "获取到 Circle: " << c << std::endl;
        }

        // 类型转换示例：int -> Point
        shape_variant = 42;
        Point p2;
        if (mc::get_value(shape_variant, p2)) {
            std::cout << "int(42) -> Point: " << p2 << std::endl;
        }
    }

    // 通用访问器模板
    template <typename Dst>
    class VariantExtractor {
      public:
        template <typename... Types>
        std::optional<Dst> extract(const std::variant<Types...>& v) {
            Dst result;
            if (mc::get_value(v, result)) {
                return result;
            }
            return std::nullopt;
        }

        template <typename... Types>
        std::vector<Dst> extract_all(
            const std::vector<std::variant<Types...>>& variants) {
            std::vector<Dst> results;
            for (const auto& v : variants) {
                if (auto result = extract(v)) {
                    results.push_back(*result);
                }
            }
            return results;
        }
    };

    void generic_extractor_usage() {
        std::cout << "\n=== variant get_value 通用提取器 ===" << std::endl;

        using MixedVariant = std::variant<int, double, std::string, Point>;
        std::vector<MixedVariant> mixed_data = {
            42, 3.14, std::string("text"), Point(1, 2), 100, 2.71};

        // 提取所有可转换为double的值
        VariantExtractor<double> double_extractor;
        auto doubles = double_extractor.extract_all(mixed_data);

        std::cout << "提取的double值: ";
        for (double d : doubles) {
            std::cout << d << " ";
        }
        std::cout << std::endl;

        // 提取所有字符串
        VariantExtractor<std::string> string_extractor;
        auto strings = string_extractor.extract_all(mixed_data);

        std::cout << "提取的字符串: ";
        for (const auto& s : strings) {
            std::cout << "\"" << s << "\" ";
        }
        std::cout << std::endl;
    }

    void error_handling() {
        std::cout << "\n=== variant get_value 错误处理 ===" << std::endl;

        std::variant<int, std::string> v = std::string("hello");

        // 尝试获取错误类型
        int wrong_type;
        if (!mc::get_value(v, wrong_type)) {
            std::cout << "无法将字符串转换为int" << std::endl;
        }

        // 尝试不兼容的转换
        struct NonConvertible {
            int value;
        };

        NonConvertible nc;
        if (!mc::get_value(v, nc)) {
            std::cout << "无法将字符串转换为NonConvertible" << std::endl;
        }
    }

}  // namespace variant_advanced_usage

namespace variant_implementation_details {

    void implementation_explanation() {
        std::cout << "\n=== get_value 实现原理解析 ===" << std::endl;

        /*
    get_value 实现的关键技术：
    
    1. make_array 函数模板：
       - 在编译时生成函数指针数组
       - 每个数组元素对应variant的一个可能类型
    
    2. get_value_impl 函数模板：
       - 检查variant是否持有特定类型T
       - 如果类型匹配且可转换，执行转换
    
    3. 运行时类型分发：
       - 使用variant.index()获取当前类型索引
       - 通过函数指针数组实现O(1)的类型分发
    
    4. 编译时类型检查：
       - 使用std::is_convertible_v检查类型兼容性
       - 只有兼容类型才会进行转换
    */

        std::cout << "实现特点:" << std::endl;
        std::cout << "1. 编译时生成类型分发表" << std::endl;
        std::cout << "2. 运行时O(1)类型查找" << std::endl;
        std::cout << "3. 类型安全的转换检查" << std::endl;
        std::cout << "4. 支持隐式类型转换" << std::endl;
    }

    // 简化版实现示例
    template <typename Dst, typename... Types>
    bool simple_get_value(const std::variant<Types...>& v, Dst& dst) {
        return std::visit(
            [&dst](const auto& value) -> bool {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_convertible_v<T, Dst>) {
                    dst = static_cast<Dst>(value);
                    return true;
                }
                return false;
            },
            v);
    }

    void alternative_implementation() {
        std::cout << "\n=== get_value 替代实现方案 ===" << std::endl;

        std::variant<int, double, std::string> v = 42;

        // 使用简化的std::visit实现
        double result;
        if (simple_get_value(v, result)) {
            std::cout << "简化实现获取值: " << result << std::endl;
        }

        std::cout << "std::visit方案的特点:" << std::endl;
        std::cout << "1. 代码更简洁" << std::endl;
        std::cout << "2. 不需要预生成函数表" << std::endl;
        std::cout << "3. 每次调用都会进行类型检查" << std::endl;
        std::cout << "4. 可能有轻微的性能开销" << std::endl;
    }

    void performance_analysis() {
        std::cout << "\n=== get_value 性能分析 ===" << std::endl;

        /*
    性能特点分析：
    
    原始实现（make_array + 函数指针）：
    优点：
    - 编译时生成分发表，运行时O(1)查找
    - 避免重复的类型检查
    - 对于频繁调用的场景性能优秀
    
    缺点：
    - 编译时间较长
    - 代码复杂度高
    - 静态存储开销
    
    std::visit实现：
    优点：
    - 代码简洁易懂
    - 编译速度快
    - 无额外存储开销
    
    缺点：
    - 每次调用都需要类型分发
    - 对于大型variant可能有性能影响
    */

        std::cout << "选择建议:" << std::endl;
        std::cout << "- 频繁调用场景：使用原始实现" << std::endl;
        std::cout << "- 偶尔调用场景：使用std::visit实现" << std::endl;
        std::cout << "- 代码可读性优先：使用std::visit实现" << std::endl;
    }

}  // namespace variant_implementation_details

namespace variant_best_practices {

    void type_safety_guidelines() {
        std::cout << "\n=== variant get_value 类型安全指南 ===" << std::endl;

        std::cout << "最佳实践:" << std::endl;
        std::cout << "1. 总是检查get_value的返回值" << std::endl;
        std::cout << "2. 优先使用明确的类型而非隐式转换" << std::endl;
        std::cout << "3. 为自定义类型提供明确的转换语义" << std::endl;
        std::cout << "4. 避免有损转换（如double到int）" << std::endl;

        // 示例：安全的类型检查
        std::variant<int, double, std::string> v = 3.14159;

        // 推荐：明确检查
        double precise_val;
        if (mc::get_value(v, precise_val)) {
            std::cout << "精确值: " << precise_val << std::endl;
        }

        // 注意：有损转换
        int lossy_val;
        if (mc::get_value(v, lossy_val)) {
            std::cout << "有损转换: " << lossy_val << std::endl;
        }
    }

    void error_handling_strategies() {
        std::cout << "\n=== variant get_value 错误处理策略 ===" << std::endl;

        using DataVariant = std::variant<int, double, std::string>;
        DataVariant data = std::string("not_a_number");

        // 策略1：返回optional
        auto safe_get = [](const DataVariant& v) -> std::optional<double> {
            double result;
            if (mc::get_value(v, result)) {
                return result;
            }
            return std::nullopt;
        };

        if (auto result = safe_get(data)) {
            std::cout << "成功获取: " << *result << std::endl;
        } else {
            std::cout << "获取失败，返回nullopt" << std::endl;
        }

        // 策略2：提供默认值
        auto get_or_default = [](const DataVariant& v,
                                 double default_val) -> double {
            double result;
            if (mc::get_value(v, result)) {
                return result;
            }
            return default_val;
        };

        double value = get_or_default(data, 0.0);
        std::cout << "获取值或默认值: " << value << std::endl;
    }

    void performance_tips() {
        std::cout << "\n=== variant get_value 性能优化建议 ===" << std::endl;

        std::cout << "性能优化建议:" << std::endl;
        std::cout << "1. 缓存频繁访问的结果" << std::endl;
        std::cout << "2. 使用std::holds_alternative预检查" << std::endl;
        std::cout << "3. 考虑批量处理相同类型的variant" << std::endl;
        std::cout << "4. 避免在热点路径中进行复杂转换" << std::endl;

        // 性能优化示例
        std::vector<std::variant<int, double, std::string>> data_batch;
        // ... 填充数据 ...

        // 优化：按类型分组处理
        std::vector<double> numbers;
        std::vector<std::string> texts;

        for (const auto& item : data_batch) {
            // 预检查避免不必要的转换尝试
            if (std::holds_alternative<int>(item) ||
                std::holds_alternative<double>(item)) {
                double num;
                if (mc::get_value(item, num)) {
                    numbers.push_back(num);
                }
            } else if (std::holds_alternative<std::string>(item)) {
                std::string text;
                if (mc::get_value(item, text)) {
                    texts.push_back(text);
                }
            }
        }
    }

}  // namespace variant_best_practices

// 演示所有功能
void demonstrate_all_get_value_features() {
    variant_basic_transform::basic_usage();
    variant_basic_transform::type_conversion_usage();
    variant_basic_transform::container_usage();

    variant_advanced_usage::custom_type_usage();
    variant_advanced_usage::generic_extractor_usage();
    variant_advanced_usage::error_handling();

    variant_implementation_details::implementation_explanation();
    variant_implementation_details::alternative_implementation();
    variant_implementation_details::performance_analysis();

    variant_best_practices::type_safety_guidelines();
    variant_best_practices::error_handling_strategies();
    variant_best_practices::performance_tips();
}

int main() {
    // 演示所有功能
    demonstrate_all_get_value_features();

    std::cout << "\n=== 原始测试用例 ===" << std::endl;

    std::variant<int, std::string> v = std::string{"test"};
    std::string s;
    assert(mc::get_value(v, s));
    assert(s == "test");
    v = 42;
    int i;
    assert(mc::get_value(v, i));
    assert(i == 42);

    std::cout << "所有测试通过!" << std::endl;

    return 0;
}