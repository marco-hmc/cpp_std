#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

namespace reference_wrapper_basic_transform {

    void basic_usage() {
        std::cout << "=== reference_wrapper 基础用法 ===" << std::endl;

        // 1. 基本包装和访问
        int x = 42;
        std::reference_wrapper<int> ref_x = std::ref(x);

        std::cout << "原始值 x: " << x << std::endl;
        std::cout << "通过ref_x.get(): " << ref_x.get() << std::endl;
        std::cout << "通过隐式转换: " << static_cast<int&>(ref_x) << std::endl;

        // 2. 修改引用的值
        ref_x.get() = 100;
        std::cout << "修改后 x: " << x << std::endl;
    }

    void container_usage() {
        std::cout << "\n=== reference_wrapper 容器使用 ===" << std::endl;

        int a = 1, b = 2, c = 3;

        // 在容器中存储引用
        std::vector<std::reference_wrapper<int>> ref_vec = {
            std::ref(a), std::ref(b), std::ref(c)};

        std::cout << "原始值: a=" << a << ", b=" << b << ", c=" << c
                  << std::endl;

        // 通过容器修改原始值
        for (auto& ref : ref_vec) {
            ref.get() *= 10;
        }

        std::cout << "修改后: a=" << a << ", b=" << b << ", c=" << c
                  << std::endl;

        // 使用算法
        std::for_each(ref_vec.begin(), ref_vec.end(),
                      [](std::reference_wrapper<int> ref) { ref.get() += 5; });

        std::cout << "算法处理后: a=" << a << ", b=" << b << ", c=" << c
                  << std::endl;
    }

    void function_parameter_usage() {
        std::cout << "\n=== reference_wrapper 函数参数使用 ===" << std::endl;

        auto process_values =
            [](std::vector<std::reference_wrapper<int>> refs) {
                for (auto& ref : refs) {
                    ref.get() = ref.get() * 2 + 1;
                }
            };

        int x = 5, y = 10, z = 15;
        std::cout << "处理前: x=" << x << ", y=" << y << ", z=" << z
                  << std::endl;

        process_values({std::ref(x), std::ref(y), std::ref(z)});
        std::cout << "处理后: x=" << x << ", y=" << y << ", z=" << z
                  << std::endl;
    }

}  // namespace reference_wrapper_basic_transform

namespace reference_wrapper_advanced_usage {

    // 自定义类示例
    class Counter {
      public:
        Counter(int initial = 0) : count_(initial) {}

        void increment() { ++count_; }
        void decrement() { --count_; }
        int get() const { return count_; }

        friend std::ostream& operator<<(std::ostream& os, const Counter& c) {
            return os << "Counter(" << c.count_ << ")";
        }

      private:
        int count_;
    };

    void object_reference_usage() {
        std::cout << "\n=== reference_wrapper 对象引用使用 ===" << std::endl;

        Counter c1(10), c2(20), c3(30);

        // 存储对象引用
        std::vector<std::reference_wrapper<Counter>> counters = {
            std::ref(c1), std::ref(c2), std::ref(c3)};

        std::cout << "初始状态:" << std::endl;
        for (const auto& counter_ref : counters) {
            std::cout << "  " << counter_ref.get() << std::endl;
        }

        // 批量操作
        std::for_each(counters.begin(), counters.end(),
                      [](std::reference_wrapper<Counter> ref) {
                          ref.get().increment();
                          ref.get().increment();
                      });

        std::cout << "增加2后:" << std::endl;
        for (const auto& counter_ref : counters) {
            std::cout << "  " << counter_ref.get() << std::endl;
        }
    }

    void const_reference_usage() {
        std::cout << "\n=== reference_wrapper const引用使用 ===" << std::endl;

        const int x = 42;
        const std::string str = "hello";

        // 包装const引用
        std::reference_wrapper<const int> const_ref_x = std::cref(x);
        std::reference_wrapper<const std::string> const_ref_str =
            std::cref(str);

        std::cout << "const int引用: " << const_ref_x.get() << std::endl;
        std::cout << "const string引用: " << const_ref_str.get() << std::endl;

        // 在容器中存储const引用
        std::vector<std::reference_wrapper<const int>> const_refs;
        const int a = 1, b = 2, c = 3;
        const_refs.push_back(std::cref(a));
        const_refs.push_back(std::cref(b));
        const_refs.push_back(std::cref(c));

        std::cout << "const引用容器: ";
        for (const auto& ref : const_refs) {
            std::cout << ref.get() << " ";
        }
        std::cout << std::endl;
    }

    void bind_usage() {
        std::cout << "\n=== reference_wrapper 与 std::bind 配合使用 ==="
                  << std::endl;

        auto add_to = [](int& target, int value) {
            target += value;
            std::cout << "添加 " << value << " 到目标，结果: " << target
                      << std::endl;
        };

        int result = 10;
        std::cout << "初始值: " << result << std::endl;

        // 使用 std::ref 与 bind
        auto bound_func =
            std::bind(add_to, std::ref(result), std::placeholders::_1);

        bound_func(5);   // result += 5
        bound_func(15);  // result += 15

        std::cout << "最终值: " << result << std::endl;
    }

}  // namespace reference_wrapper_advanced_usage

namespace reference_wrapper_implementation_details {

    // 简化的 reference_wrapper 实现
    template <typename T>
    class simple_reference_wrapper {
      private:
        T* ptr_;

      public:
        // 类型别名
        using type = T;

        // 构造函数 - 只接受左值引用
        explicit simple_reference_wrapper(T& ref) noexcept
            : ptr_(std::addressof(ref)) {}

        // 禁止从右值构造
        simple_reference_wrapper(T&&) = delete;

        // 拷贝构造和赋值
        simple_reference_wrapper(const simple_reference_wrapper&) noexcept =
            default;
        simple_reference_wrapper& operator=(
            const simple_reference_wrapper&) noexcept = default;

        // 隐式转换为引用
        operator T&() const noexcept { return *ptr_; }

        // 显式获取引用
        T& get() const noexcept { return *ptr_; }

        // 函数调用操作符（如果T是可调用的）
        template <typename... Args>
        auto operator()(Args&&... args) const
            noexcept(noexcept(std::invoke(*ptr_, std::forward<Args>(args)...)))
                -> decltype(std::invoke(*ptr_, std::forward<Args>(args)...)) {
            return std::invoke(*ptr_, std::forward<Args>(args)...);
        }
    };

    // 辅助函数
    template <typename T>
    simple_reference_wrapper<T> simple_ref(T& ref) noexcept {
        return simple_reference_wrapper<T>(ref);
    }

    template <typename T>
    simple_reference_wrapper<const T> simple_cref(const T& ref) noexcept {
        return simple_reference_wrapper<const T>(ref);
    }

    void implementation_explanation() {
        std::cout << "\n=== reference_wrapper 实现原理 ===" << std::endl;

        /*
    reference_wrapper 实现要点：
    
    1. 存储指针而不是引用：
       - 引用不能重新绑定，但指针可以
       - 允许拷贝和赋值操作
    
    2. 禁止从右值构造：
       - 防止悬垂引用
       - 只接受左值引用
    
    3. 隐式转换：
       - 提供到T&的隐式转换
       - 使其可以像普通引用一样使用
    
    4. 函数调用操作符：
       - 如果包装的是可调用对象，支持直接调用
    */

        int x = 100;
        auto ref_x = simple_ref(x);

        std::cout << "使用自定义实现:" << std::endl;
        std::cout << "原始值: " << x << std::endl;
        std::cout << "通过wrapper: " << ref_x.get() << std::endl;

        ref_x.get() = 200;
        std::cout << "修改后: " << x << std::endl;
    }

    void callable_wrapper() {
        std::cout << "\n=== reference_wrapper 包装可调用对象 ===" << std::endl;

        auto lambda = [](int a, int b) {
            std::cout << "lambda调用: " << a << " + " << b << " = " << (a + b)
                      << std::endl;
            return a + b;
        };

        // 包装lambda
        auto ref_lambda = std::ref(lambda);

        // 直接调用
        int result = ref_lambda(10, 20);
        std::cout << "调用结果: " << result << std::endl;

        // 函数对象示例
        struct Multiplier {
            int factor;
            Multiplier(int f) : factor(f) {}
            int operator()(int x) const {
                std::cout << "Multiplier: " << x << " * " << factor << " = "
                          << (x * factor) << std::endl;
                return x * factor;
            }
        };

        Multiplier mul(5);
        auto ref_mul = std::ref(mul);
        int result2 = ref_mul(8);
        std::cout << "函数对象调用结果: " << result2 << std::endl;
    }

    void type_traits_support() {
        std::cout << "\n=== reference_wrapper 类型特性支持 ===" << std::endl;

        int x = 42;
        auto ref_x = std::ref(x);

        // 类型检查
        std::cout << "is_reference_wrapper: "
                  << std::is_same_v<std::reference_wrapper<int>,
                                    std::decay_t<decltype(ref_x)>> << std::endl;

        // unwrap_reference (C++20)
        using unwrapped_type = typename std::reference_wrapper<int>::type;
        std::cout << "unwrapped type same as int: "
                  << std::is_same_v<unwrapped_type, int> << std::endl;

        // 检查是否可调用
        auto func = []() { return 42; };
        auto ref_func = std::ref(func);
        std::cout << "包装的lambda是否可调用: "
                  << std::is_invocable_v<decltype(ref_func)> << std::endl;
    }

}  // namespace reference_wrapper_implementation_details

namespace reference_wrapper_best_practices {

    void avoid_dangling_references() {
        std::cout << "\n=== 避免悬垂引用的最佳实践 ===" << std::endl;

        std::vector<std::reference_wrapper<int>> refs;

        // 错误示例（已注释）：
        // {
        //     int local_var = 42;
        //     refs.push_back(std::ref(local_var)); // 危险！local_var会被销毁
        // }
        // // 此时refs[0]指向已销毁的对象

        // 正确示例：
        int global_var = 42;
        refs.push_back(std::ref(global_var));  // 安全

        std::cout << "安全引用的值: " << refs[0].get() << std::endl;
    }

    void performance_considerations() {
        std::cout << "\n=== reference_wrapper 性能考虑 ===" << std::endl;

        /*
    性能特点：
    1. 几乎零开销 - 只是存储一个指针
    2. 隐式转换是内联的
    3. 比std::shared_ptr轻量得多
    4. 不需要动态内存分配
    */

        std::cout << "sizeof(int): " << sizeof(int) << std::endl;
        std::cout << "sizeof(int&): " << sizeof(int&) << std::endl;
        std::cout << "sizeof(std::reference_wrapper<int>): "
                  << sizeof(std::reference_wrapper<int>) << std::endl;
        std::cout << "sizeof(int*): " << sizeof(int*) << std::endl;
    }

    void when_to_use() {
        std::cout << "\n=== 何时使用 reference_wrapper ===" << std::endl;

        std::cout << "适用场景:" << std::endl;
        std::cout << "1. 在容器中存储引用" << std::endl;
        std::cout << "2. 将引用传递给需要值语义的函数" << std::endl;
        std::cout << "3. 与std::bind配合使用" << std::endl;
        std::cout << "4. 包装可调用对象用于延迟调用" << std::endl;
        std::cout << "5. 避免拷贝大对象" << std::endl;

        std::cout << "\n不适用场景:" << std::endl;
        std::cout << "1. 需要可选引用（使用指针或optional）" << std::endl;
        std::cout << "2. 需要重新绑定引用（使用指针）" << std::endl;
        std::cout << "3. 简单的函数参数传递（直接使用引用）" << std::endl;
    }

}  // namespace reference_wrapper_best_practices

// 演示所有功能
void demonstrate_all_reference_wrapper_features() {
    reference_wrapper_basic_transform::basic_usage();
    reference_wrapper_basic_transform::container_usage();
    reference_wrapper_basic_transform::function_parameter_usage();

    reference_wrapper_advanced_usage::object_reference_usage();
    reference_wrapper_advanced_usage::const_reference_usage();
    reference_wrapper_advanced_usage::bind_usage();

    reference_wrapper_implementation_details::implementation_explanation();
    reference_wrapper_implementation_details::callable_wrapper();
    reference_wrapper_implementation_details::type_traits_support();

    reference_wrapper_best_practices::avoid_dangling_references();
    reference_wrapper_best_practices::performance_considerations();
    reference_wrapper_best_practices::when_to_use();
}

int main() {
    demonstrate_all_reference_wrapper_features();
    return 0;
}