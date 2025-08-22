#include <cassert>
#include <cmath>
#include <iostream>
#include <ratio>
#include <type_traits>

namespace ratio_basic_transform {

    void basic_usage() {
        std::cout << "=== std::ratio 基础用法 ===" << std::endl;

        // 1. 基本ratio定义
        using half = std::ratio<1, 2>;
        using third = std::ratio<1, 3>;
        using two_thirds = std::ratio<2, 3>;

        std::cout << "half: " << half::num << "/" << half::den << std::endl;
        std::cout << "third: " << third::num << "/" << third::den << std::endl;
        std::cout << "two_thirds: " << two_thirds::num << "/" << two_thirds::den
                  << std::endl;

        // 2. ratio运算
        using sum = std::ratio_add<half, third>;              // 1/2 + 1/3 = 5/6
        using diff = std::ratio_subtract<two_thirds, third>;  // 2/3 - 1/3 = 1/3
        using product = std::ratio_multiply<half, third>;     // 1/2 * 1/3 = 1/6
        using quotient = std::ratio_divide<half, third>;  // (1/2) / (1/3) = 3/2

        std::cout << "1/2 + 1/3 = " << sum::num << "/" << sum::den << std::endl;
        std::cout << "2/3 - 1/3 = " << diff::num << "/" << diff::den
                  << std::endl;
        std::cout << "1/2 * 1/3 = " << product::num << "/" << product::den
                  << std::endl;
        std::cout << "(1/2) / (1/3) = " << quotient::num << "/" << quotient::den
                  << std::endl;
    }

    void ratio_comparison() {
        std::cout << "\n=== std::ratio 比较操作 ===" << std::endl;

        using half = std::ratio<1, 2>;
        using third = std::ratio<1, 3>;
        using quarter = std::ratio<1, 4>;

        // 比较操作
        std::cout << "1/2 == 1/3: " << std::ratio_equal<half, third>::value
                  << std::endl;
        std::cout << "1/2 != 1/3: " << std::ratio_not_equal<half, third>::value
                  << std::endl;
        std::cout << "1/2 > 1/3: " << std::ratio_greater<half, third>::value
                  << std::endl;
        std::cout << "1/4 < 1/3: " << std::ratio_less<quarter, third>::value
                  << std::endl;
        std::cout << "1/2 >= 1/3: "
                  << std::ratio_greater_equal<half, third>::value << std::endl;
        std::cout << "1/4 <= 1/3: "
                  << std::ratio_less_equal<quarter, third>::value << std::endl;
    }

    void predefined_ratios() {
        std::cout << "\n=== std::ratio 预定义常量 ===" << std::endl;
        std::cout << "atto (10^-18): " << std::atto::num << "/"
                  << std::atto::den << std::endl;
        std::cout << "femto (10^-15): " << std::femto::num << "/"
                  << std::femto::den << std::endl;
        std::cout << "pico (10^-12): " << std::pico::num << "/"
                  << std::pico::den << std::endl;
        std::cout << "nano (10^-9): " << std::nano::num << "/" << std::nano::den
                  << std::endl;
        std::cout << "micro (10^-6): " << std::micro::num << "/"
                  << std::micro::den << std::endl;
        std::cout << "milli (10^-3): " << std::milli::num << "/"
                  << std::milli::den << std::endl;
        std::cout << "centi (10^-2): " << std::centi::num << "/"
                  << std::centi::den << std::endl;
        std::cout << "deci (10^-1): " << std::deci::num << "/" << std::deci::den
                  << std::endl;
        std::cout << "deca (10^1): " << std::deca::num << "/" << std::deca::den
                  << std::endl;
        std::cout << "hecto (10^2): " << std::hecto::num << "/"
                  << std::hecto::den << std::endl;
        std::cout << "kilo (10^3): " << std::kilo::num << "/" << std::kilo::den
                  << std::endl;
        std::cout << "mega (10^6): " << std::mega::num << "/" << std::mega::den
                  << std::endl;
        std::cout << "giga (10^9): " << std::giga::num << "/" << std::giga::den
                  << std::endl;
        std::cout << "tera (10^12): " << std::tera::num << "/" << std::tera::den
                  << std::endl;
    }

}  // namespace ratio_basic_transform

namespace ratio_advanced_usage {

    // 自定义duration类演示ratio的应用
    template <typename T, typename Period = std::ratio<1>>
    class duration {
      public:
        using rep = T;
        using period = Period;

        constexpr duration() = default;
        constexpr explicit duration(const rep& r) : count_(r) {}

        template <typename Rep2, typename Period2>
        constexpr duration(const duration<Rep2, Period2>& other) {
            using common_type = std::ratio_divide<Period2, Period>;
            count_ = other.count() * static_cast<rep>(common_type::num) /
                     static_cast<rep>(common_type::den);
        }

        constexpr rep count() const { return count_; }

      private:
        rep count_{};
    };

    // 预定义时间单位
    using nanoseconds = duration<long long, std::nano>;
    using microseconds = duration<long long, std::micro>;
    using milliseconds = duration<long long, std::milli>;
    using seconds = duration<long long>;
    using minutes = duration<long long, std::ratio<60>>;
    using hours = duration<long long, std::ratio<3600>>;

    void duration_examples() {
        std::cout << "\n=== ratio在时间duration中的应用 ===" << std::endl;

        // 创建不同单位的时间
        auto ns = nanoseconds(1000000000);  // 1秒 = 10^9纳秒
        auto ms = milliseconds(1000);       // 1秒 = 1000毫秒
        auto s = seconds(1);                // 1秒
        auto m = minutes(1);                // 1分钟

        std::cout << "1000000000 ns = " << ns.count() << " ns" << std::endl;
        std::cout << "1000 ms = " << ms.count() << " ms" << std::endl;
        std::cout << "1 s = " << s.count() << " s" << std::endl;
        std::cout << "1 m = " << m.count() << " m" << std::endl;

        // 单位转换
        seconds from_ns(ns);
        seconds from_ms(ms);
        seconds from_m(m);

        std::cout << "转换为秒:" << std::endl;
        std::cout << "1000000000 ns -> " << from_ns.count() << " s"
                  << std::endl;
        std::cout << "1000 ms -> " << from_ms.count() << " s" << std::endl;
        std::cout << "1 m -> " << from_m.count() << " s" << std::endl;
    }

    // 物理量单位系统
    template <typename T, typename LengthRatio = std::ratio<1>,
              typename MassRatio = std::ratio<1>,
              typename TimeRatio = std::ratio<1>>
    struct physical_quantity {
        using value_type = T;
        using length_ratio = LengthRatio;
        using mass_ratio = MassRatio;
        using time_ratio = TimeRatio;

        constexpr physical_quantity(T val = T{}) : value(val) {}

        T value;
    };

    // 长度单位
    using meter = physical_quantity<double, std::ratio<1>>;
    using kilometer = physical_quantity<double, std::kilo>;
    using centimeter = physical_quantity<double, std::centi>;
    using millimeter = physical_quantity<double, std::milli>;

    // 质量单位
    using kilogram = physical_quantity<double, std::ratio<1>, std::ratio<1>>;
    using gram = physical_quantity<double, std::ratio<1>, std::milli>;
    using ton = physical_quantity<double, std::ratio<1>, std::kilo>;

    void physical_units_example() {
        std::cout << "\n=== ratio在物理单位系统中的应用 ===" << std::endl;

        meter m1(1.0);
        kilometer km1(1.0);
        centimeter cm1(100.0);

        std::cout << "1 m = " << m1.value << " m" << std::endl;
        std::cout << "1 km = " << km1.value << " km" << std::endl;
        std::cout << "100 cm = " << cm1.value << " cm" << std::endl;

        // 单位转换计算
        double m_to_km =
            m1.value * meter::length_ratio::num / meter::length_ratio::den *
            kilometer::length_ratio::den / kilometer::length_ratio::num;
        double cm_to_m = cm1.value * centimeter::length_ratio::num /
                         centimeter::length_ratio::den *
                         meter::length_ratio::den / meter::length_ratio::num;

        std::cout << "1 m = " << m_to_km << " km" << std::endl;
        std::cout << "100 cm = " << cm_to_m << " m" << std::endl;
    }

}  // namespace ratio_advanced_usage

namespace ratio_implementation_details {

    // 简化的ratio实现说明
    template <intmax_t N, intmax_t D = 1>
    struct simple_ratio {
      private:
        // 最大公约数计算
        static constexpr intmax_t gcd(intmax_t a, intmax_t b) {
            return b == 0 ? a : gcd(b, a % b);
        }

        static constexpr intmax_t abs_val = (N < 0) ? -N : N;
        static constexpr intmax_t abs_den = (D < 0) ? -D : D;
        static constexpr intmax_t g = gcd(abs_val, abs_den);

      public:
        static constexpr intmax_t num =
            (N < 0) != (D < 0) ? -(abs_val / g) : (abs_val / g);
        static constexpr intmax_t den = abs_den / g;

        using type = simple_ratio<num, den>;
    };

    // ratio加法实现
    template <typename R1, typename R2>
    struct simple_ratio_add {
      private:
        static constexpr intmax_t gcd(intmax_t a, intmax_t b) {
            return b == 0 ? a : gcd(b, a % b);
        }

        static constexpr intmax_t num = R1::num * R2::den + R2::num * R1::den;
        static constexpr intmax_t den = R1::den * R2::den;
        static constexpr intmax_t g = gcd(num < 0 ? -num : num, den);

      public:
        using type = simple_ratio<num / g, den / g>;
    };

    void implementation_explanation() {
        std::cout << "\n=== std::ratio 实现原理 ===" << std::endl;

        /*
    std::ratio 的核心实现原理：
    
    1. 编译时有理数表示：
       - 使用两个整数 num (分子) 和 den (分母) 表示有理数
       - 在编译时进行最简化处理 (约分)
    
    2. 最大公约数 (GCD) 算法：
       - 使用欧几里得算法在编译时计算GCD
       - 确保分数始终处于最简形式
    
    3. 符号处理：
       - 负号统一存储在分子中
       - 分母始终为正数
    
    4. 编译时运算：
       - 所有运算都在编译时完成
       - 使用模板元编程技术实现
    */

        using r1 = simple_ratio<6, 9>;               // 自动简化为 2/3
        using r2 = simple_ratio<4, 6>;               // 自动简化为 2/3
        using sum = simple_ratio_add<r1, r2>::type;  // 4/3

        std::cout << "simple_ratio<6, 9>: " << r1::num << "/" << r1::den
                  << std::endl;
        std::cout << "simple_ratio<4, 6>: " << r2::num << "/" << r2::den
                  << std::endl;
        std::cout << "sum: " << sum::num << "/" << sum::den << std::endl;
    }

    void compile_time_features() {
        std::cout << "\n=== std::ratio 编译时特性 ===" << std::endl;

        /*
    编译时计算的优势：
    1. 零运行时开销
    2. 类型安全
    3. 编译期错误检查
    4. 模板特化支持
    */

        // 编译时断言
        using half = std::ratio<1, 2>;
        using quarter = std::ratio<1, 4>;
        using three_quarters = std::ratio_add<half, quarter>;

        static_assert(three_quarters::num == 3);
        static_assert(three_quarters::den == 4);
        static_assert(
            std::ratio_equal<three_quarters, std::ratio<3, 4>>::value);

        std::cout << "所有编译时断言通过" << std::endl;
        std::cout << "1/2 + 1/4 = " << three_quarters::num << "/"
                  << three_quarters::den << std::endl;
    }

    void overflow_handling() {
        std::cout << "\n=== std::ratio 溢出处理 ===" << std::endl;

        /*
    std::ratio 的溢出处理：
    1. 使用 intmax_t 类型存储分子分母
    2. 编译时检查溢出
    3. 提供 safe_multiply, safe_add 等安全运算
    */

        std::cout << "intmax_t 范围: " << std::numeric_limits<intmax_t>::min()
                  << " 到 " << std::numeric_limits<intmax_t>::max()
                  << std::endl;

        // 大数值示例
        using large_ratio = std::ratio<1000000000LL, 1000000001LL>;
        std::cout << "大分数示例: " << large_ratio::num << "/"
                  << large_ratio::den << std::endl;
    }

}  // namespace ratio_implementation_details

namespace {

    template <int N, int D = 1>
    struct ratio {
        static constexpr int num = N;
        static constexpr int den = D;
        using type = ratio<num, den>;
    };

    template <typename R1, typename R2>
    struct ratio_add_impl {
      private:
        static constexpr int den = R1::den * R2::den;
        static constexpr int num = R1::num * R2::den + R2::num * R1::den;

      public:
        using type = ratio<num, den>;
    };

    template <typename R1, typename R2>
    using ratio_add = typename ratio_add_impl<R1, R2>::type;

    template <typename T, typename U = ratio<1>>
    class duration {
      public:
        using rep = T;
        using period = typename U::type;

      public:
        constexpr duration(rep r = 0) : r_(r) {}
        constexpr rep count() const { return r_; }

      private:
        rep r_;
    };

    template <typename T1, typename U1, typename T2, typename U2>
    constexpr auto operator+(const duration<T1, U1>& lhs,
                             const duration<T2, U2>& rhs) {
        using CommonType = ratio<1, ratio_add<U1, U2>::den>;
        auto res = (lhs.count() * U1::num / U1::den +
                    rhs.count() * U2::num / U2::den) *
                   CommonType::den;
        return duration<decltype(res), CommonType>{res};
    }

}  // namespace

void demonstrate_all_ratio_features() {
    ratio_basic_transform::basic_usage();
    ratio_basic_transform::ratio_comparison();
    ratio_basic_transform::predefined_ratios();

    ratio_advanced_usage::duration_examples();
    ratio_advanced_usage::physical_units_example();

    ratio_implementation_details::implementation_explanation();
    ratio_implementation_details::compile_time_features();
    ratio_implementation_details::overflow_handling();
}

int main() {
    // 运行演示
    demonstrate_all_ratio_features();

    std::cout << "\n=== 原始测试代码 ===" << std::endl;

    constexpr auto a = duration<double, ratio<1, 1000>>(10);  // 10 ms
    constexpr auto b = duration<double, ratio<1, 3>>(7.5);    // 2.5 s
    constexpr auto c = a + b;  // 10 * 3 + 7.5 * 1000 = 7530 * 1/3000 s
    assert(std::abs(c.count() - 7530) < 1e-6);
    static_assert(std::is_same_v<std::decay_t<decltype(c)>,
                                 duration<double, ratio<1, 3000>>>);
    static_assert(decltype(c)::period::num == 1);
    static_assert(decltype(c)::period::den == 3000);

    std::cout << "原始测试通过" << std::endl;

    return 0;
}
