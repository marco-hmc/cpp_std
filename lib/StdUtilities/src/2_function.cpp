// std::function 实现详解和用法

#include <algorithm>
#include <any>
#include <cassert>
#include <exception>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::FUNCTION 标准用法 ===" << std::endl;

        // 1. 存储普通函数
        std::function<int(int, int)> add_func = [](int a, int b) {
            return a + b;
        };
        std::cout << "Lambda 函数: 3 + 4 = " << add_func(3, 4) << std::endl;

        // 2. 存储函数指针
        auto multiply = [](int a, int b) { return a * b; };
        std::function<int(int, int)> mult_func = multiply;
        std::cout << "函数对象: 3 * 4 = " << mult_func(3, 4) << std::endl;

        // 3. 存储成员函数
        struct Calculator {
            int subtract(int a, int b) { return a - b; }
        };

        Calculator calc;
        std::function<int(Calculator*, int, int)> sub_func =
            &Calculator::subtract;
        std::cout << "成员函数: 10 - 3 = " << sub_func(&calc, 10, 3)
                  << std::endl;

        // 4. 函数容器
        std::vector<std::function<int(int)>> operations;
        operations.emplace_back([](int x) { return x * 2; });
        operations.emplace_back([](int x) { return x + 10; });
        operations.emplace_back([](int x) { return x * x; });

        std::cout << "函数容器应用于值 5:" << std::endl;
        for (size_t i = 0; i < operations.size(); ++i) {
            std::cout << "操作 " << i << ": " << operations[i](5) << std::endl;
        }
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    template <typename T>
    class is_equality_comparable {
      private:
        static void* conv(bool);

        template <typename U>
        static std::true_type test(decltype(conv(std::declval<const U&>() ==
                                                 std::declval<const U&>())),
                                   decltype(conv(!(std::declval<const U&>() ==
                                                   std::declval<const U&>()))));

        template <typename U>
        static std::false_type test(...);

      public:
        static constexpr bool value =
            decltype(test<T>(nullptr, nullptr))::value;
    };

    template <typename T, bool = is_equality_comparable<T>::value>
    struct try_equals {
        static bool equals(const T& lhs, const T& rhs) { return lhs == rhs; }
    };

    struct not_equality_comparable : std::exception {};

    template <typename T>
    struct try_equals<T, false> {
        static bool equals(const T& lhs, const T& rhs) {
            throw not_equality_comparable();
        }
    };

    template <typename R, typename... Args>
    class functor_bridge {
      public:
        virtual ~functor_bridge() {}
        virtual functor_bridge* clone() const = 0;
        virtual R invoke(Args... args) const = 0;
        virtual bool equals(const functor_bridge*) const = 0;
    };

    template <typename F, typename R, typename... Args>
    class functor_bridge_impl : public functor_bridge<R, Args...> {
      public:
        template <typename T>
        functor_bridge_impl(T&& f) : f_(std::forward<T>(f)) {}

        virtual functor_bridge_impl* clone() const override {
            return new functor_bridge_impl(f_);
        }

        virtual R invoke(Args... args) const override {
            return f_(std::forward<Args>(args)...);
        }

        virtual bool equals(
            const functor_bridge<R, Args...>* rhs) const override {
            if (auto p = dynamic_cast<const functor_bridge_impl*>(rhs)) {
                return try_equals<F>::equals(f_, p->f_);
            }
            return false;
        }

      private:
        F f_;
    };

    template <typename>
    class function;

    template <typename R, typename... Args>
    class function<R(Args...)> {
        friend bool operator==(const function& lhs, const function& rhs) {
            if (!lhs || !rhs) {
                return !lhs && !rhs;
            }
            return lhs.bridge_->equals(rhs.bridge_);
        }

        friend bool operator!=(const function& lhs, const function& rhs) {
            return !(lhs == rhs);
        }

        friend void swap(function& lhs, function& rhs) noexcept {
            std::swap(lhs.bridge_, rhs.bridge_);
        }

      public:
        function() = default;

        function(const function& rhs) {
            if (rhs.bridge_) {
                bridge_ = rhs.bridge_->clone();
            }
        }

        function(function& rhs) : function(static_cast<const function&>(rhs)) {}

        function(function&& rhs) noexcept : bridge_(rhs.bridge_) {
            rhs.bridge_ = nullptr;
        }

        template <typename F>
        function(F&& f) {
            using Bridge = functor_bridge_impl<std::decay_t<F>, R, Args...>;
            bridge_ = new Bridge(std::forward<F>(f));  // type erasure
        }

        ~function() { delete bridge_; }

        function& operator=(const function& rhs) {
            function tmp(rhs);
            swap(*this, tmp);
            return *this;
        }

        function& operator=(function&& rhs) noexcept {
            delete bridge_;
            bridge_ = rhs.bridge_;
            rhs.bridge_ = nullptr;
            return *this;
        }

        template <typename F>
        function& operator=(F&& rhs) {
            function tmp(std::forward<F>(rhs));
            swap(*this, tmp);
            return *this;
        }

        explicit operator bool() const { return bridge_ != nullptr; }

        R operator()(Args... args) const {
            return bridge_->invoke(std::forward<Args>(args)...);
        }

      private:
        functor_bridge<R, Args...>* bridge_ = nullptr;
    };

    void task() {
        std::cout << "\n=== 自定义 FUNCTION 实现测试 ===" << std::endl;

        // 测试基本功能
        function<int(int, int)> add_func = [](int a, int b) { return a + b; };
        std::cout << "自定义 function 测试: 5 + 3 = " << add_func(5, 3)
                  << std::endl;

        // 测试拷贝构造
        function<int(int, int)> copied_func = add_func;
        std::cout << "拷贝构造测试: 7 + 2 = " << copied_func(7, 2) << std::endl;

        // 测试移动构造
        function<int(int, int)> moved_func = std::move(copied_func);
        std::cout << "移动构造测试: 6 + 4 = " << moved_func(6, 4) << std::endl;

        // 测试空函数对象
        function<void()> empty_func;
        std::cout << "空函数对象检查: " << (empty_func ? "非空" : "空")
                  << std::endl;
    }
}  // namespace MY_IMPL

namespace IMPLEMENTATION_DETAILS {
    void task() {
        std::cout << "\n=== FUNCTION 实现细节说明 ===" << std::endl;

        std::cout << "1. 类型擦除 (Type Erasure):" << std::endl;
        std::cout << "   - 使用虚函数基类 functor_bridge 抽象接口" << std::endl;
        std::cout << "   - 派生类 functor_bridge_impl 存储具体的可调用对象"
                  << std::endl;
        std::cout << "   - 通过模板构造函数接受任意可调用类型" << std::endl;

        std::cout << "\n2. 内存管理:" << std::endl;
        std::cout << "   - 使用原始指针和手动内存管理" << std::endl;
        std::cout << "   - 拷贝时通过 clone() 虚函数深拷贝" << std::endl;
        std::cout << "   - 移动时转移所有权，原对象置空" << std::endl;

        std::cout << "\n3. 相等性比较:" << std::endl;
        std::cout << "   - 使用 SFINAE 检测类型是否支持 == 运算符" << std::endl;
        std::cout << "   - 不支持比较的类型会抛出异常" << std::endl;

        std::cout << "\n4. 调用接口:" << std::endl;
        std::cout << "   - operator() 通过 bridge->invoke() 转发调用"
                  << std::endl;
        std::cout << "   - 使用完美转发保持参数类型" << std::endl;
    }
}  // namespace IMPLEMENTATION_DETAILS

namespace ADVANCED_FEATURES {
    void task() {
        std::cout << "\n=== FUNCTION 高级特性 ===" << std::endl;

        // 1. 函数组合
        auto compose = [](auto f, auto g) {
            return [f, g](auto x) { return f(g(x)); };
        };

        std::function<int(int)> double_fn = [](int x) { return x * 2; };
        std::function<int(int)> add_one = [](int x) { return x + 1; };

        auto composed = compose(double_fn, add_one);
        std::cout << "函数组合: double(add_one(5)) = " << composed(5)
                  << std::endl;

        // 2. 回调机制
        class EventHandler {
          public:
            using Callback = std::function<void(const std::string&)>;

            void setOnEvent(Callback cb) { callback_ = cb; }

            void triggerEvent(const std::string& event) {
                if (callback_) {
                    callback_(event);
                }
            }

          private:
            Callback callback_;
        };

        EventHandler handler;
        handler.setOnEvent([](const std::string& event) {
            std::cout << "事件触发: " << event << std::endl;
        });
        handler.triggerEvent("用户点击");

        // 3. 策略模式
        class Sorter {
          public:
            using Strategy = std::function<bool(int, int)>;

            void setStrategy(Strategy strategy) { strategy_ = strategy; }

            void sort(std::vector<int>& data) {
                if (strategy_) {
                    std::sort(data.begin(), data.end(), strategy_);
                }
            }

          private:
            Strategy strategy_;
        };

        std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};
        Sorter sorter;

        std::cout << "策略模式排序:" << std::endl;
        std::cout << "原数据: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 升序排序
        sorter.setStrategy([](int a, int b) { return a < b; });
        sorter.sort(data);
        std::cout << "升序: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 降序排序
        sorter.setStrategy([](int a, int b) { return a > b; });
        sorter.sort(data);
        std::cout << "降序: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace ADVANCED_FEATURES

namespace PERFORMANCE_CONSIDERATIONS {
    void task() {
        std::cout << "\n=== FUNCTION 性能考虑 ===" << std::endl;

        std::cout << "1. 开销分析:" << std::endl;
        std::cout << "   - 动态分配: 每个 function 对象需要堆分配" << std::endl;
        std::cout << "   - 虚函数调用: invoke() 是虚函数，有间接调用开销"
                  << std::endl;
        std::cout << "   - 类型擦除: 编译时优化受限" << std::endl;

        std::cout << "\n2. 优化建议:" << std::endl;
        std::cout << "   - 对于性能关键路径，考虑使用模板而非 std::function"
                  << std::endl;
        std::cout << "   - 重复使用 function 对象，避免频繁构造/析构"
                  << std::endl;
        std::cout << "   - 小对象优化: 现代实现通常避免小函数对象的堆分配"
                  << std::endl;

        std::cout << "\n3. 替代方案:" << std::endl;
        std::cout << "   - 函数指针: 零开销，但功能有限" << std::endl;
        std::cout << "   - 模板参数: 编译时确定，性能最佳" << std::endl;
        std::cout << "   - std::function: 灵活性最高，但有运行时开销"
                  << std::endl;
    }
}  // namespace PERFORMANCE_CONSIDERATIONS

int main() {
    STD_USAGE::task();

    // 修正原始测试代码中的类型不匹配问题
    std::cout << "\n=== 修正的原始示例 ===" << std::endl;

    // 原代码有类型不匹配问题，这里修正
    MY_IMPL::function<int(int)> f = [](int x) -> int {
        return x > 0 ? 1 : 0;  // 简单的正数检测
    };

    std::cout << "f(3) = " << f(3) << std::endl;
    std::cout << "f(-1) = " << f(-1) << std::endl;

    MY_IMPL::task();
    IMPLEMENTATION_DETAILS::task();
    ADVANCED_FEATURES::task();
    PERFORMANCE_CONSIDERATIONS::task();

    return 0;
}
