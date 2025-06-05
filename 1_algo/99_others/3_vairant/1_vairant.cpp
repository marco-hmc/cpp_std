#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

class Variant {
  public:
    // 构造函数模板，用于存储不同类型的数据
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

    // 析构函数
    ~Variant() { destroy(); }

    // 拷贝构造函数
    Variant(const Variant& other) { copyFrom(other); }

    // 移动构造函数
    Variant(Variant&& other) noexcept { moveFrom(std::move(other)); }

    // 拷贝赋值运算符
    Variant& operator=(const Variant& other) {
        if (this != &other) {
            destroy();
            copyFrom(other);
        }
        return *this;
    }

    // 移动赋值运算符
    Variant& operator=(Variant&& other) noexcept {
        if (this != &other) {
            destroy();
            moveFrom(std::move(other));
        }
        return *this;
    }

    // 获取存储的数据类型
    enum class Type { Int, Float, String, VectorInt, ArrayInt5 };

    Type getType() const { return type; }

    // 获取存储的数据
    template <typename T>
    T get() const {
        using U = std::decay_t<T>;
        if constexpr (std::is_same_v<U, int>) {
            if (type != Type::Int) {
                throw std::bad_variant_access();
            }
            return data.intValue;
        } else if constexpr (std::is_same_v<U, float>) {
            if (type != Type::Float) {
                throw std::bad_variant_access();
            }
            return data.floatValue;
        } else if constexpr (std::is_same_v<U, std::string>) {
            if (type != Type::String) {
                throw std::bad_variant_access();
            }
            return data.stringValue;
        } else if constexpr (std::is_same_v<U, std::vector<int>>) {
            if (type != Type::VectorInt) {
                throw std::bad_variant_access();
            }
            return data.vectorIntValue;
        } else if constexpr (std::is_same_v<U, std::array<int, 5>>) {
            if (type != Type::ArrayInt5) {
                throw std::bad_variant_access();
            }
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
                new (&data.arrayInt5Value)
                    std::array<int, 5>(std::move(other.data.arrayInt5Value));
                break;
        }
        other.type = Type::Int;  // Reset other to a valid state
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

int main() {
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

    return 0;
}