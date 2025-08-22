// std::variant 全面实现和用法详解

#include <array>
#include <cassert>
#include <exception>
#include <iostream>
#include <new>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

// 包含 typelist 实现（简化版）
namespace Mc {
    template <typename... Types>
    struct typelist {};

    template <typename List>
    struct front;

    template <typename Head, typename... Tail>
    struct front<typelist<Head, Tail...>> {
        using type = Head;
    };

    template <typename List>
    using front_t = typename front<List>::type;

    template <typename List, typename T>
    struct find_index_of;

    template <typename T, typename Head, typename... Tail>
    struct find_index_of<typelist<Head, Tail...>, T> {
        static constexpr std::size_t value =
            std::is_same_v<T, Head>
                ? 0
                : 1 + find_index_of<typelist<Tail...>, T>::value;
    };

    template <typename T>
    struct find_index_of<typelist<>, T> {
        static constexpr std::size_t value = 0;
    };

    template <typename List, typename T>
    using find_index_of_t = find_index_of<List, T>;

    template <typename List>
    struct largest_type;

    template <typename T>
    struct largest_type<typelist<T>> {
        using type = T;
    };

    template <typename Head, typename... Tail>
    struct largest_type<typelist<Head, Tail...>> {
        using rest = typename largest_type<typelist<Tail...>>::type;
        using type =
            std::conditional_t<sizeof(Head) >= sizeof(rest), Head, rest>;
    };

    template <typename List>
    using largest_type_t = typename largest_type<List>::type;
}  // namespace Mc

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::VARIANT 标准用法 ===" << std::endl;

        // 1. 基本用法
        std::variant<int, double, std::string> var = 42;
        std::cout << "初始值 (int): " << std::get<int>(var) << std::endl;

        var = 3.14;
        std::cout << "赋值 (double): " << std::get<double>(var) << std::endl;

        var = std::string("Hello");
        std::cout << "赋值 (string): " << std::get<std::string>(var)
                  << std::endl;

        // 2. index() 和 holds_alternative
        std::cout << "当前类型索引: " << var.index() << std::endl;
        std::cout << "是否为string: "
                  << std::holds_alternative<std::string>(var) << std::endl;

        // 3. visit 模式
        std::visit(
            [](const auto& value) {
                std::cout << "访问者模式输出: " << value << std::endl;
            },
            var);

        // 4. get_if 安全访问
        if (auto* str_ptr = std::get_if<std::string>(&var)) {
            std::cout << "get_if 安全访问: " << *str_ptr << std::endl;
        }

        // 5. 异常处理
        try {
            std::get<int>(var);  // 当前是string，会抛异常
        } catch (const std::bad_variant_access& e) {
            std::cout << "捕获异常: " << e.what() << std::endl;
        }
    }
}  // namespace STD_USAGE

namespace SIMPLE_VARIANT_IMPL {
    // 简化的 variant 实现（基于第一个文件）
    class Variant {
      public:
        enum class Type { Int, Float, String, VectorInt, ArrayInt5 };

        template <typename T>
        Variant(T value) {
            using U = std::decay_t<T>;
            if constexpr (std::is_same_v<U, int>) {
                type = Type::Int;
                data.intValue = value;
            } else if constexpr (std::is_same_v<U, float>) {
                type = Type::Float;
                data.floatValue = value;
            } else if constexpr (std::is_same_v<U, std::string>) {
                type = Type::String;
                new (&data.stringValue) std::string(value);
            } else if constexpr (std::is_same_v<U, std::vector<int>>) {
                type = Type::VectorInt;
                new (&data.vectorIntValue) std::vector<int>(value);
            } else if constexpr (std::is_same_v<U, std::array<int, 5>>) {
                type = Type::ArrayInt5;
                new (&data.arrayInt5Value) std::array<int, 5>(value);
            } else {
                throw std::invalid_argument("Unsupported type");
            }
        }

        ~Variant() { destroy(); }

        Variant(const Variant& other) { copyFrom(other); }
        Variant(Variant&& other) noexcept { moveFrom(std::move(other)); }

        Variant& operator=(const Variant& other) {
            if (this != &other) {
                destroy();
                copyFrom(other);
            }
            return *this;
        }

        Variant& operator=(Variant&& other) noexcept {
            if (this != &other) {
                destroy();
                moveFrom(std::move(other));
            }
            return *this;
        }

        Type getType() const { return type; }

        template <typename T>
        T get() const {
            using U = std::decay_t<T>;
            if constexpr (std::is_same_v<U, int>) {
                if (type != Type::Int) throw std::bad_variant_access();
                return data.intValue;
            } else if constexpr (std::is_same_v<U, float>) {
                if (type != Type::Float) throw std::bad_variant_access();
                return data.floatValue;
            } else if constexpr (std::is_same_v<U, std::string>) {
                if (type != Type::String) throw std::bad_variant_access();
                return data.stringValue;
            } else if constexpr (std::is_same_v<U, std::vector<int>>) {
                if (type != Type::VectorInt) throw std::bad_variant_access();
                return data.vectorIntValue;
            } else if constexpr (std::is_same_v<U, std::array<int, 5>>) {
                if (type != Type::ArrayInt5) throw std::bad_variant_access();
                return data.arrayInt5Value;
            } else {
                throw std::invalid_argument("Unsupported type");
            }
        }

      private:
        Type type;

        union Data {
            int intValue;
            float floatValue;
            std::string stringValue;
            std::vector<int> vectorIntValue;
            std::array<int, 5> arrayInt5Value;

            Data() {}
            ~Data() {}
        } data;

        void destroy() {
            switch (type) {
                case Type::String:
                    data.stringValue.~basic_string();
                    break;
                case Type::VectorInt:
                    data.vectorIntValue.~vector();
                    break;
                case Type::ArrayInt5:
                    data.arrayInt5Value.~array();
                    break;
                default:
                    break;
            }
        }

        void copyFrom(const Variant& other) {
            type = other.type;
            switch (type) {
                case Type::Int:
                    data.intValue = other.data.intValue;
                    break;
                case Type::Float:
                    data.floatValue = other.data.floatValue;
                    break;
                case Type::String:
                    new (&data.stringValue) std::string(other.data.stringValue);
                    break;
                case Type::VectorInt:
                    new (&data.vectorIntValue)
                        std::vector<int>(other.data.vectorIntValue);
                    break;
                case Type::ArrayInt5:
                    new (&data.arrayInt5Value)
                        std::array<int, 5>(other.data.arrayInt5Value);
                    break;
            }
        }

        void moveFrom(Variant&& other) {
            type = other.type;
            switch (type) {
                case Type::Int:
                    data.intValue = other.data.intValue;
                    break;
                case Type::Float:
                    data.floatValue = other.data.floatValue;
                    break;
                case Type::String:
                    new (&data.stringValue)
                        std::string(std::move(other.data.stringValue));
                    break;
                case Type::VectorInt:
                    new (&data.vectorIntValue)
                        std::vector<int>(std::move(other.data.vectorIntValue));
                    break;
                case Type::ArrayInt5:
                    new (&data.arrayInt5Value) std::array<int, 5>(
                        std::move(other.data.arrayInt5Value));
                    break;
            }
            other.type = Type::Int;
        }
    };

    void printVariant(const Variant& var) {
        switch (var.getType()) {
            case Variant::Type::Int:
                std::cout << "int: " << var.get<int>() << std::endl;
                break;
            case Variant::Type::Float:
                std::cout << "float: " << var.get<float>() << std::endl;
                break;
            case Variant::Type::String:
                std::cout << "string: " << var.get<std::string>() << std::endl;
                break;
            case Variant::Type::VectorInt:
                std::cout << "vector<int>: ";
                for (const auto& elem : var.get<std::vector<int>>()) {
                    std::cout << elem << ' ';
                }
                std::cout << std::endl;
                break;
            case Variant::Type::ArrayInt5:
                std::cout << "array<int, 5>: ";
                for (const auto& elem : var.get<std::array<int, 5>>()) {
                    std::cout << elem << ' ';
                }
                std::cout << std::endl;
                break;
        }
    }

    void task() {
        std::cout << "\n=== 简化 VARIANT 实现测试 ===" << std::endl;

        Variant var = 42;
        printVariant(var);

        var = 3.14f;
        printVariant(var);

        var = std::string("Hello, world!");
        printVariant(var);

        var = std::vector<int>{1, 2, 3, 4, 5};
        printVariant(var);

        var = std::array<int, 5>{6, 7, 8, 9, 10};
        printVariant(var);
    }
}  // namespace SIMPLE_VARIANT_IMPL

namespace ADVANCED_VARIANT_IMPL {
    // 高级 variant 实现（基于第三个文件的设计）
    using namespace Mc;

    class computed_result_type;

    template <typename Visitor, typename T>
    using visit_element_result =
        decltype(std::declval<Visitor>()(std::declval<T>()));

    template <typename R, typename Visitor, typename... Types>
    struct visit_result {
        using type = R;
    };

    template <typename Visitor, typename... Types>
    struct visit_result<computed_result_type, Visitor, Types...> {
        using type =
            std::common_type_t<visit_element_result<Visitor, Types>...>;
    };

    template <typename R, typename Visitor, typename... Types>
    using visit_result_t = typename visit_result<R, Visitor, Types...>::type;

    struct empty_variant : std::exception {};

    template <typename R, typename V, typename Visitor, typename Head,
              typename... Tail>
    R variant_visit_impl(V&& variant, Visitor&& vis, typelist<Head, Tail...>) {
        if (variant.template is<Head>()) {
            return static_cast<R>(std::forward<Visitor>(vis)(
                std::forward<V>(variant).template get<Head>()));
        } else if constexpr (sizeof...(Tail) > 0) {
            return variant_visit_impl<R>(std::forward<V>(variant),
                                         std::forward<Visitor>(vis),
                                         typelist<Tail...>{});
        } else {
            throw empty_variant();
        }
    }

    template <typename... Types>
    class variant_storage {
      public:
        unsigned char get_discriminator() const { return discriminator_; }
        void set_discriminator(unsigned char d) { discriminator_ = d; }
        void* get_raw_buffer() { return buffer_; }
        const void* get_raw_buffer() const { return buffer_; }

        template <typename T>
        T* get_buffer_as() {
            return std::launder(reinterpret_cast<T*>(buffer_));
        }

        template <typename T>
        const T* get_buffer_as() const {
            return std::launder(reinterpret_cast<const T*>(buffer_));
        }

      private:
        using largest_t = largest_type_t<typelist<Types...>>;
        alignas(Types...) unsigned char buffer_[sizeof(largest_t)];
        unsigned char discriminator_ = 0;
    };

    template <typename... Types>
    class variant;

    template <typename T, typename... Types>
    class variant_choice {
        using Derived = variant<Types...>;

        Derived& get_derived() { return *static_cast<Derived*>(this); }
        const Derived& get_derived() const {
            return *static_cast<const Derived*>(this);
        }

      protected:
        static constexpr unsigned Discriminator =
            find_index_of_t<typelist<Types...>, T>::value + 1;

      public:
        variant_choice() = default;

        variant_choice(const T& value) {
            new (get_derived().get_raw_buffer()) T(value);
            get_derived().set_discriminator(Discriminator);
        }

        variant_choice(T&& value) {
            new (get_derived().get_raw_buffer()) T(std::move(value));
            get_derived().set_discriminator(Discriminator);
        }

        bool destroy() {
            if (get_derived().get_discriminator() == Discriminator) {
                get_derived().template get_buffer_as<T>()->~T();
                return true;
            }
            return false;
        }

        Derived& operator=(const T& value) {
            if (get_derived().get_discriminator() == Discriminator) {
                *get_derived().template get_buffer_as<T>() = value;
            } else {
                get_derived().destroy();
                new (get_derived().get_raw_buffer()) T(value);
                get_derived().set_discriminator(Discriminator);
            }
            return get_derived();
        }

        Derived& operator=(T&& value) {
            if (get_derived().get_discriminator() == Discriminator) {
                *get_derived().template get_buffer_as<T>() = std::move(value);
            } else {
                get_derived().destroy();
                new (get_derived().get_raw_buffer()) T(std::move(value));
                get_derived().set_discriminator(Discriminator);
            }
            return get_derived();
        }
    };

    template <typename... Types>
    class variant : private variant_storage<Types...>,
                    private variant_choice<Types, Types...>... {
        template <typename T, typename... OtherTypes>
        friend class variant_choice;

      public:
        using variant_choice<Types, Types...>::variant_choice...;
        using variant_choice<Types, Types...>::operator=...;

        variant() { *this = front_t<typelist<Types...>>(); }

        variant(const variant& rhs) {
            if (!rhs.empty()) {
                rhs.visit([&](const auto& value) { *this = value; });
            }
        }

        variant(variant&& rhs) {
            if (!rhs.empty()) {
                std::move(rhs).visit(
                    [&](auto&& value) { *this = std::move(value); });
            }
        }

        template <typename... SourceTypes>
        variant(const variant<SourceTypes...>& rhs) {
            if (!rhs.empty()) {
                rhs.visit([&](const auto& value) { *this = value; });
            }
        }

        template <typename... SourceTypes>
        variant(variant<SourceTypes...>&& rhs) {
            if (!rhs.empty()) {
                std::move(rhs).visit(
                    [&](auto&& value) { *this = std::move(value); });
            }
        }

        variant& operator=(const variant& rhs) {
            if (!rhs.empty()) {
                rhs.visit([&](const auto& value) { *this = value; });
            } else {
                destroy();
            }
            return *this;
        }

        variant& operator=(variant&& rhs) {
            if (!rhs.empty()) {
                std::move(rhs).visit(
                    [&](auto&& value) { *this = std::move(value); });
            } else {
                destroy();
            }
            return *this;
        }

        bool empty() const { return this->get_discriminator() == 0; }
        ~variant() { destroy(); }

        void destroy() {
            (variant_choice<Types, Types...>::destroy(), ...);
            this->set_discriminator(0);
        }

        template <typename T>
        bool is() const {
            return this->get_discriminator() ==
                   variant_choice<T, Types...>::Discriminator;
        }

        template <typename T>
        T& get() & {
            if (empty()) throw empty_variant();
            assert(is<T>());
            return *this->template get_buffer_as<T>();
        }

        template <typename T>
        const T& get() const& {
            if (empty()) throw empty_variant();
            assert(is<T>());
            return *this->template get_buffer_as<T>();
        }

        template <typename T>
        T&& get() && {
            if (empty()) throw empty_variant();
            assert(is<T>());
            return std::move(*this->template get_buffer_as<T>());
        }

        template <typename R = computed_result_type, typename Visitor>
        visit_result_t<R, Visitor, Types&...> visit(Visitor&& vis) & {
            using Result = visit_result_t<R, Visitor, Types&...>;
            return variant_visit_impl<Result>(*this, std::forward<Visitor>(vis),
                                              typelist<Types...>{});
        }

        template <typename R = computed_result_type, typename Visitor>
        visit_result_t<R, Visitor, const Types&...> visit(
            Visitor&& vis) const& {
            using Result = visit_result_t<R, Visitor, const Types&...>;
            return variant_visit_impl<Result>(*this, std::forward<Visitor>(vis),
                                              typelist<Types...>{});
        }

        template <typename R = computed_result_type, typename Visitor>
        visit_result_t<R, Visitor, Types&&...> visit(Visitor&& vis) && {
            using Result = visit_result_t<R, Visitor, Types&&...>;
            return variant_visit_impl<Result>(std::move(*this),
                                              std::forward<Visitor>(vis),
                                              typelist<Types...>{});
        }
    };

    // 测试代码
    struct copied_noncopyable : std::exception {};

    struct noncopyable {
        noncopyable() = default;
        noncopyable(const noncopyable&) { throw copied_noncopyable(); }
        noncopyable(noncopyable&&) = default;
        noncopyable& operator=(const noncopyable&) {
            throw copied_noncopyable();
        }
        noncopyable& operator=(noncopyable&&) = default;
    };

    void task() {
        std::cout << "\n=== 高级 VARIANT 实现测试 ===" << std::endl;

        variant<int, double, std::string> v{42};
        assert(!v.empty());
        assert(v.is<int>());
        assert(v.get<int>() == 42);

        v = 3.14;
        assert(v.is<double>());
        assert(v.get<double>() == 3.14);

        v = "hello";
        assert(v.is<std::string>());
        assert(v.get<std::string>() == "hello");

        std::stringstream os;
        v.visit([&os](const auto& value) { os << value; });
        assert(os.str() == "hello");
        std::cout << "访问者模式输出: " << os.str() << std::endl;

        // 测试移动语义
        variant<int, double, std::string> v2;
        assert(!v2.empty());
        assert(v2.is<int>());

        v2 = std::move(v);
        assert(v.is<std::string>());
        assert(v.get<std::string>().empty());
        assert(v2.is<std::string>());
        assert(v2.get<std::string>() == "hello");

        v2.destroy();
        assert(v2.empty());

        std::cout << "高级variant实现测试通过!" << std::endl;
    }
}  // namespace ADVANCED_VARIANT_IMPL

namespace PRACTICAL_APPLICATIONS {
    void task() {
        std::cout << "\n=== VARIANT 实际应用场景 ===" << std::endl;

        // 1. 错误处理替代方案
        using Result = std::variant<int, std::string>;  // value or error

        auto divide = [](int a, int b) -> Result {
            if (b == 0) {
                return std::string("Division by zero");
            }
            return a / b;
        };

        auto result1 = divide(10, 2);
        auto result2 = divide(10, 0);

        std::visit(
            [](const auto& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, int>) {
                    std::cout << "计算结果: " << value << std::endl;
                } else {
                    std::cout << "错误: " << value << std::endl;
                }
            },
            result1);

        std::visit(
            [](const auto& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, int>) {
                    std::cout << "计算结果: " << value << std::endl;
                } else {
                    std::cout << "错误: " << value << std::endl;
                }
            },
            result2);

        // 2. 状态机实现
        struct Idle {};
        struct Running {
            int progress;
        };
        struct Completed {
            std::string result;
        };

        using State = std::variant<Idle, Running, Completed>;

        auto processState = [](const State& state) {
            std::visit(
                [](const auto& s) {
                    using T = std::decay_t<decltype(s)>;
                    if constexpr (std::is_same_v<T, Idle>) {
                        std::cout << "状态: 空闲" << std::endl;
                    } else if constexpr (std::is_same_v<T, Running>) {
                        std::cout << "状态: 运行中 (" << s.progress << "%)"
                                  << std::endl;
                    } else if constexpr (std::is_same_v<T, Completed>) {
                        std::cout << "状态: 完成 - " << s.result << std::endl;
                    }
                },
                state);
        };

        State machine = Idle{};
        processState(machine);

        machine = Running{50};
        processState(machine);

        machine = Completed{"Success"};
        processState(machine);

        // 3. 多态容器
        std::vector<std::variant<int, double, std::string>> mixed_data;
        mixed_data.push_back(42);
        mixed_data.push_back(3.14);
        mixed_data.push_back(std::string("hello"));

        std::cout << "\n多态容器内容:" << std::endl;
        for (const auto& item : mixed_data) {
            std::visit(
                [](const auto& value) {
                    std::cout << "值: " << value
                              << " (类型: " << typeid(value).name() << ")"
                              << std::endl;
                },
                item);
        }
    }
}  // namespace PRACTICAL_APPLICATIONS

namespace PERFORMANCE_CONSIDERATIONS {
    void task() {
        std::cout << "\n=== VARIANT 性能考虑 ===" << std::endl;

        std::cout << "1. 内存布局:" << std::endl;
        std::cout
            << "   - variant 大小 = max(所有类型大小) + 判别器大小 + 对齐填充"
            << std::endl;
        std::cout << "   - 判别器通常是 1 字节" << std::endl;

        std::variant<int, double, std::string> v;
        std::cout << "   - variant<int, double, string> 大小: " << sizeof(v)
                  << " 字节" << std::endl;
        std::cout << "   - int 大小: " << sizeof(int) << " 字节" << std::endl;
        std::cout << "   - double 大小: " << sizeof(double) << " 字节"
                  << std::endl;
        std::cout << "   - string 大小: " << sizeof(std::string) << " 字节"
                  << std::endl;

        std::cout << "\n2. 性能特性:" << std::endl;
        std::cout << "   - 类型检查: O(1) - 只需检查判别器" << std::endl;
        std::cout << "   - 赋值: 可能需要析构+构造，取决于类型变化"
                  << std::endl;
        std::cout << "   - 访问: O(1) - 直接内存访问，无虚函数开销"
                  << std::endl;
        std::cout << "   - visit: 编译期生成 switch，性能接近手写 switch"
                  << std::endl;

        std::cout << "\n3. 最佳实践:" << std::endl;
        std::cout << "   - 将最常用的类型放在前面（更好的分支预测）"
                  << std::endl;
        std::cout << "   - 避免频繁的类型切换" << std::endl;
        std::cout << "   - 对于复杂类型，考虑存储指针而不是值" << std::endl;
        std::cout << "   - 使用 visit 而不是手动类型检查" << std::endl;
    }
}  // namespace PERFORMANCE_CONSIDERATIONS

int main() {
    STD_USAGE::task();
    SIMPLE_VARIANT_IMPL::task();
    ADVANCED_VARIANT_IMPL::task();
    PRACTICAL_APPLICATIONS::task();
    PERFORMANCE_CONSIDERATIONS::task();
    return 0;
}
