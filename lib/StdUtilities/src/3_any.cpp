#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

namespace any_basic_transform {

    void basic_usage() {
        std::cout << "=== std::any 基础用法 ===" << std::endl;

        // 1. 构造和赋值
        std::any a;                         // 空any
        std::any b = 42;                    // int
        std::any c = 3.14;                  // double
        std::any d = std::string("hello");  // string

        // 2. 检查状态
        std::cout << "a.has_value(): " << a.has_value() << std::endl;
        std::cout << "b.has_value(): " << b.has_value() << std::endl;

        // 3. 类型信息
        std::cout << "b type: " << b.type().name() << std::endl;
        std::cout << "c type: " << c.type().name() << std::endl;

        // 4. 安全转换
        try {
            int val_b = std::any_cast<int>(b);
            double val_c = std::any_cast<double>(c);
            std::string val_d = std::any_cast<std::string>(d);

            std::cout << "b = " << val_b << std::endl;
            std::cout << "c = " << val_c << std::endl;
            std::cout << "d = " << val_d << std::endl;
        } catch (const std::bad_any_cast& e) {
            std::cout << "转换失败: " << e.what() << std::endl;
        }
    }

    void container_usage() {
        std::cout << "\n=== std::any 容器使用 ===" << std::endl;

        // 异构容器
        std::vector<std::any> mixed_data;
        mixed_data.push_back(100);
        mixed_data.push_back(2.718);
        mixed_data.push_back(std::string("world"));
        mixed_data.push_back(true);

        // 遍历和类型检查
        for (size_t i = 0; i < mixed_data.size(); ++i) {
            const auto& item = mixed_data[i];
            std::cout << "Item " << i << ": ";

            if (item.type() == typeid(int)) {
                std::cout << "int(" << std::any_cast<int>(item) << ")";
            } else if (item.type() == typeid(double)) {
                std::cout << "double(" << std::any_cast<double>(item) << ")";
            } else if (item.type() == typeid(std::string)) {
                std::cout << "string(" << std::any_cast<std::string>(item)
                          << ")";
            } else if (item.type() == typeid(bool)) {
                std::cout << "bool(" << std::any_cast<bool>(item) << ")";
            }
            std::cout << std::endl;
        }
    }

    void safe_cast_usage() {
        std::cout << "\n=== std::any 安全转换 ===" << std::endl;

        std::any data = std::string("test");

        // 方法1: 使用指针版本 any_cast (推荐)
        if (const std::string* str_ptr = std::any_cast<std::string>(&data)) {
            std::cout << "安全获取: " << *str_ptr << std::endl;
        } else {
            std::cout << "类型不匹配" << std::endl;
        }

        // 方法2: 先检查类型
        if (data.type() == typeid(std::string)) {
            std::string str = std::any_cast<std::string>(data);
            std::cout << "类型检查后获取: " << str << std::endl;
        }

        // 方法3: try-catch
        try {
            std::string str = std::any_cast<std::string>(data);
            std::cout << "异常处理获取: " << str << std::endl;
        } catch (const std::bad_any_cast& e) {
            std::cout << "转换失败: " << e.what() << std::endl;
        }
    }

}  // namespace any_basic_transform

namespace any_advanced_usage {

    struct Person {
        std::string name;
        int age;
        Person(const std::string& n, int a) : name(n), age(a) {}
    };

    void custom_type_usage() {
        std::cout << "\n=== std::any 自定义类型 ===" << std::endl;

        // 存储自定义类型
        std::any person_any = Person("Alice", 30);

        // 获取自定义类型
        if (Person* p = std::any_cast<Person>(&person_any)) {
            std::cout << "Person: " << p->name << ", age: " << p->age
                      << std::endl;
        }

        // 存储智能指针
        std::any ptr_any = std::make_shared<Person>("Bob", 25);
        if (auto ptr = std::any_cast<std::shared_ptr<Person>>(ptr_any)) {
            std::cout << "Shared Person: " << ptr->name << ", age: " << ptr->age
                      << std::endl;
        }
    }

    void function_storage() {
        std::cout << "\n=== std::any 函数存储 ===" << std::endl;

        // 存储lambda函数
        std::any func_any =
            std::function<int(int, int)>([](int a, int b) { return a + b; });

        // 调用存储的函数
        if (auto func =
                std::any_cast<std::function<int(int, int)>>(&func_any)) {
            int result = (*func)(10, 20);
            std::cout << "函数调用结果: " << result << std::endl;
        }

        // 存储普通函数指针
        std::any func_ptr_any =
            static_cast<int (*)(int, int)>([](int a, int b) { return a * b; });

        if (auto func_ptr = std::any_cast<int (*)(int, int)>(func_ptr_any)) {
            int result = func_ptr(5, 6);
            std::cout << "函数指针调用结果: " << result << std::endl;
        }
    }

    // 配置管理系统示例
    class ConfigManager {
      private:
        std::map<std::string, std::any> configs;

      public:
        template <typename T>
        void set(const std::string& key, const T& value) {
            configs[key] = value;
        }

        template <typename T>
        T get(const std::string& key) const {
            auto it = configs.find(key);
            if (it != configs.end()) {
                try {
                    return std::any_cast<T>(it->second);
                } catch (const std::bad_any_cast&) {
                    throw std::runtime_error("Type mismatch for key: " + key);
                }
            }
            throw std::runtime_error("Key not found: " + key);
        }

        template <typename T>
        T get_or_default(const std::string& key, const T& default_value) const {
            try {
                return get<T>(key);
            } catch (...) {
                return default_value;
            }
        }

        bool has(const std::string& key) const {
            return configs.find(key) != configs.end();
        }

        void remove(const std::string& key) { configs.erase(key); }

        void clear() { configs.clear(); }
    };

    void config_system_example() {
        std::cout << "\n=== std::any 配置系统示例 ===" << std::endl;

        ConfigManager config;

        // 设置不同类型的配置
        config.set("server_port", 8080);
        config.set("server_host", std::string("localhost"));
        config.set("debug_mode", true);
        config.set("timeout", 30.5);

        // 获取配置
        std::cout << "Server Port: " << config.get<int>("server_port")
                  << std::endl;
        std::cout << "Server Host: " << config.get<std::string>("server_host")
                  << std::endl;
        std::cout << "Debug Mode: " << config.get<bool>("debug_mode")
                  << std::endl;
        std::cout << "Timeout: " << config.get<double>("timeout") << std::endl;

        // 使用默认值
        int max_connections =
            config.get_or_default<int>("max_connections", 100);
        std::cout << "Max Connections: " << max_connections << std::endl;
    }

}  // namespace any_advanced_usage

namespace any_implementation_notes {

    void memory_layout_explanation() {
        std::cout << "\n=== std::any 内存布局说明 ===" << std::endl;

        /*
    std::any 的典型实现包含：
    1. 类型信息指针 (const std::type_info*)
    2. 存储区域 (小对象优化缓冲区或堆指针)
    3. 管理函数指针 (拷贝、移动、销毁)
    
    小对象优化 (SOO - Small Object Optimization):
    - 小对象直接存储在 any 内部的缓冲区中
    - 大对象存储在堆上，any 存储指针
    */

        std::cout << "sizeof(std::any): " << sizeof(std::any) << " bytes"
                  << std::endl;
        std::cout << "sizeof(int): " << sizeof(int) << " bytes" << std::endl;
        std::cout << "sizeof(double): " << sizeof(double) << " bytes"
                  << std::endl;
        std::cout << "sizeof(std::string): " << sizeof(std::string) << " bytes"
                  << std::endl;

        // 演示小对象优化
        std::any small_int = 42;                            // 可能使用SOO
        std::any small_double = 3.14;                       // 可能使用SOO
        std::any large_vector = std::vector<int>(1000, 1);  // 可能使用堆分配

        std::cout << "已创建不同大小的对象进行存储测试" << std::endl;
    }

    void performance_considerations() {
        std::cout << "\n=== std::any 性能考虑 ===" << std::endl;

        std::cout << "性能特点:" << std::endl;
        std::cout << "1. 类型检查开销: any_cast 需要运行时类型检查"
                  << std::endl;
        std::cout << "2. 内存开销: 存储类型信息和管理函数" << std::endl;
        std::cout << "3. 拷贝开销: 拷贝 any 会深拷贝存储的对象" << std::endl;
        std::cout << "4. 小对象优化: 减少堆分配" << std::endl;

        std::cout << "\n优化建议:" << std::endl;
        std::cout << "1. 使用指针版本的 any_cast 避免异常" << std::endl;
        std::cout << "2. 缓存类型检查结果" << std::endl;
        std::cout << "3. 考虑使用 std::variant 替代（类型集合已知时）"
                  << std::endl;
        std::cout << "4. 避免频繁的类型转换" << std::endl;
    }

    void comparison_with_alternatives() {
        std::cout << "\n=== std::any 与替代方案比较 ===" << std::endl;

        std::cout << "std::any vs std::variant:" << std::endl;
        std::cout << "  any: 可存储任意类型，运行时开销较大" << std::endl;
        std::cout << "  variant: 预定义类型集合，编译时优化，性能更好"
                  << std::endl;

        std::cout << "\nstd::any vs void*:" << std::endl;
        std::cout << "  any: 类型安全，自动内存管理，运行时类型信息"
                  << std::endl;
        std::cout << "  void*: 无类型安全，手动内存管理，性能最佳" << std::endl;

        std::cout << "\nstd::any vs 虚基类:" << std::endl;
        std::cout << "  any: 无需继承关系，可存储值类型" << std::endl;
        std::cout << "  虚基类: 需要继承关系，支持多态，接口清晰" << std::endl;
    }

    void best_practices() {
        std::cout << "\n=== std::any 最佳实践 ===" << std::endl;

        std::cout << "1. 优先使用指针版本的 any_cast:" << std::endl;
        std::any data = 42;
        if (const int* ptr = std::any_cast<int>(&data)) {
            std::cout << "   安全获取: " << *ptr << std::endl;
        }

        std::cout << "2. 检查类型后再转换:" << std::endl;
        if (data.type() == typeid(int)) {
            int value = std::any_cast<int>(data);
            std::cout << "   类型安全: " << value << std::endl;
        }

        std::cout << "4. 避免存储引用类型:" << std::endl;
        int x = 10;
        // std::any ref_any = std::ref(x);  // 推荐使用指针或智能指针替代
        std::any ptr_any = &x;

        std::cout << "5. 考虑异常安全:" << std::endl;
        try {
            std::string str = std::any_cast<std::string>(data);
        } catch (const std::bad_any_cast& e) {
            std::cout << "   处理转换异常: " << e.what() << std::endl;
        }
    }

}  // namespace any_implementation_notes

// 演示所有功能
void demonstrate_all_any_features() {
    any_basic_transform::basic_usage();
    any_basic_transform::container_usage();
    any_basic_transform::safe_cast_usage();

    any_advanced_usage::custom_type_usage();
    any_advanced_usage::function_storage();
    any_advanced_usage::config_system_example();

    any_implementation_notes::memory_layout_explanation();
    any_implementation_notes::performance_considerations();
    any_implementation_notes::comparison_with_alternatives();
    any_implementation_notes::best_practices();
}
