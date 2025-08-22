// std::mem_fn 用法详解和自定义实现

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace STD_USAGE {
    class Person {
      public:
        std::string name;
        int age;

        Person(const std::string& n, int a) : name(n), age(a) {}

        void introduce() const {
            std::cout << "Hi, I'm " << name << ", " << age << " years old."
                      << std::endl;
        }

        void setAge(int newAge) { age = newAge; }

        int getAge() const { return age; }

        bool isAdult() const { return age >= 18; }
    };

    void task() {
        std::cout << "=== STD::MEM_FN 标准用法 ===" << std::endl;

        // 1. 基本成员函数调用
        Person person("Alice", 25);

        std::cout << "基本成员函数调用:" << std::endl;
        auto introduce_fn = std::mem_fn(&Person::introduce);
        introduce_fn(person);

        // 2. 成员变量访问
        std::cout << "\n成员变量访问:" << std::endl;
        auto name_fn = std::mem_fn(&Person::name);
        auto age_fn = std::mem_fn(&Person::age);

        std::cout << "Name: " << name_fn(person) << std::endl;
        std::cout << "Age: " << age_fn(person) << std::endl;

        // 3. 带参数的成员函数
        std::cout << "\n带参数的成员函数:" << std::endl;
        auto setAge_fn = std::mem_fn(&Person::setAge);
        setAge_fn(person, 30);
        std::cout << "New age: " << person.age << std::endl;

        // 4. 在算法中使用 - 容器元素调用
        std::vector<Person> people = {
            {"Bob", 20}, {"Charlie", 17}, {"David", 25}, {"Eve", 16}};

        std::cout << "\n在算法中使用 - 介绍所有人:" << std::endl;
        std::for_each(people.begin(), people.end(),
                      std::mem_fn(&Person::introduce));

        // 5. 条件过滤 - 找出成年人
        std::cout << "\n条件过滤 - 成年人数量:" << std::endl;
        int adults = std::count_if(people.begin(), people.end(),
                                   std::mem_fn(&Person::isAdult));
        std::cout << "Adult count: " << adults << std::endl;

        // 6. 提取成员变量
        std::cout << "\n提取所有人的年龄:" << std::endl;
        std::vector<int> ages;
        std::transform(people.begin(), people.end(), std::back_inserter(ages),
                       std::mem_fn(&Person::age));

        std::cout << "Ages: ";
        for (int age : ages) std::cout << age << " ";
        std::cout << std::endl;

        // 7. 与智能指针配合使用
        std::cout << "\n与智能指针配合使用:" << std::endl;
        std::vector<std::unique_ptr<Person>> ptr_people;
        ptr_people.push_back(std::make_unique<Person>("Frank", 22));
        ptr_people.push_back(std::make_unique<Person>("Grace", 19));

        std::for_each(
            ptr_people.begin(), ptr_people.end(),
            [](const auto& ptr) { std::mem_fn (&Person::introduce)(*ptr); });
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 mem_fn
    template <typename R, typename C>
    class mem_fn_t {
      private:
        R C::*ptr;

      public:
        explicit mem_fn_t(R C::*p) : ptr(p) {}

        // 对于成员变量的访问
        template <typename T>
        auto operator()(T&& obj) const -> decltype((std::forward<T>(obj).*
                                                    ptr)) {
            return std::forward<T>(obj).*ptr;
        }

        // 对于指针的解引用访问
        template <typename T>
        auto operator()(T* obj) const -> decltype((obj->*ptr)) {
            return obj->*ptr;
        }
    };

    // 成员函数的特化
    template <typename R, typename C, typename... Args>
    class mem_fn_func_t {
      private:
        R (C::*ptr)(Args...);

      public:
        explicit mem_fn_func_t(R (C::*p)(Args...)) : ptr(p) {}

        // 对象调用
        template <typename T>
        auto operator()(T&& obj, Args... args) const
            -> decltype((std::forward<T>(obj).*ptr)(args...)) {
            return (std::forward<T>(obj).*ptr)(args...);
        }

        // 指针调用
        auto operator()(C* obj, Args... args) const -> R {
            return (obj->*ptr)(args...);
        }
    };

    // const 成员函数的特化
    template <typename R, typename C, typename... Args>
    class mem_fn_const_func_t {
      private:
        R (C::*ptr)(Args...) const;

      public:
        explicit mem_fn_const_func_t(R (C::*p)(Args...) const) : ptr(p) {}

        template <typename T>
        auto operator()(T&& obj, Args... args) const
            -> decltype((std::forward<T>(obj).*ptr)(args...)) {
            return (std::forward<T>(obj).*ptr)(args...);
        }

        auto operator()(const C* obj, Args... args) const -> R {
            return (obj->*ptr)(args...);
        }
    };

    // 工厂函数
    template <typename R, typename C>
    mem_fn_t<R, C> my_mem_fn(R C::*ptr) {
        return mem_fn_t<R, C>(ptr);
    }

    template <typename R, typename C, typename... Args>
    mem_fn_func_t<R, C, Args...> my_mem_fn(R (C::*ptr)(Args...)) {
        return mem_fn_func_t<R, C, Args...>(ptr);
    }

    template <typename R, typename C, typename... Args>
    mem_fn_const_func_t<R, C, Args...> my_mem_fn(R (C::*ptr)(Args...) const) {
        return mem_fn_const_func_t<R, C, Args...>(ptr);
    }

    // 测试类
    class TestClass {
      public:
        int value;
        std::string name;

        TestClass(int v, const std::string& n) : value(v), name(n) {}

        void print() const {
            std::cout << "TestClass: " << name << " = " << value << std::endl;
        }

        void setValue(int v) { value = v; }

        int getValue() const { return value; }
    };

    void task() {
        std::cout << "\n=== 自定义 MEM_FN 实现测试 ===" << std::endl;

        TestClass obj(42, "test");

        // 测试成员变量访问
        std::cout << "自定义 mem_fn 测试 - 成员变量访问:" << std::endl;
        auto value_fn = my_mem_fn(&TestClass::value);
        auto name_fn = my_mem_fn(&TestClass::name);

        std::cout << "Value: " << value_fn(obj) << std::endl;
        std::cout << "Name: " << name_fn(obj) << std::endl;

        // 测试成员函数调用
        std::cout << "\n自定义 mem_fn 测试 - 成员函数调用:" << std::endl;
        auto print_fn = my_mem_fn(&TestClass::print);
        auto setValue_fn = my_mem_fn(&TestClass::setValue);
        auto getValue_fn = my_mem_fn(&TestClass::getValue);

        print_fn(obj);
        setValue_fn(obj, 100);
        std::cout << "New value: " << getValue_fn(obj) << std::endl;

        // 测试指针调用
        std::cout << "\n自定义 mem_fn 测试 - 指针调用:" << std::endl;
        TestClass* ptr = &obj;
        print_fn(ptr);
        std::cout << "Pointer value: " << value_fn(ptr) << std::endl;
    }
}  // namespace MY_IMPL

namespace PRACTICAL_APPLICATIONS {
    class Employee {
      public:
        std::string name;
        std::string department;
        double salary;

        Employee(const std::string& n, const std::string& d, double s)
            : name(n), department(d), salary(s) {}

        void displayInfo() const {
            std::cout << name << " (" << department << ") - $" << salary
                      << std::endl;
        }

        void raiseSalary(double percentage) {
            salary *= (1.0 + percentage / 100.0);
        }

        bool isHighEarner() const { return salary > 50000; }
    };

    void task() {
        std::cout << "\n=== MEM_FN 实际应用场景 ===" << std::endl;

        std::vector<Employee> employees = {{"Alice", "Engineering", 60000},
                                           {"Bob", "Marketing", 45000},
                                           {"Charlie", "Engineering", 55000},
                                           {"David", "Sales", 40000},
                                           {"Eve", "Management", 70000}};

        // 1. 显示所有员工信息
        std::cout << "所有员工信息:" << std::endl;
        std::for_each(employees.begin(), employees.end(),
                      std::mem_fn(&Employee::displayInfo));

        // 2. 统计高收入员工
        std::cout << "\n高收入员工数量: ";
        int highEarners = std::count_if(employees.begin(), employees.end(),
                                        std::mem_fn(&Employee::isHighEarner));
        std::cout << highEarners << std::endl;

        // 3. 提取所有薪资
        std::vector<double> salaries;
        std::transform(employees.begin(), employees.end(),
                       std::back_inserter(salaries),
                       std::mem_fn(&Employee::salary));

        std::cout << "\n所有薪资: ";
        for (double s : salaries) std::cout << "$" << s << " ";
        std::cout << std::endl;

        // 4. 给所有人加薪10%
        std::cout << "\n给所有人加薪10%后:" << std::endl;
        std::for_each(employees.begin(), employees.end(), [](Employee& emp) {
            std::mem_fn (&Employee::raiseSalary)(emp, 10.0);
        });

        std::for_each(employees.begin(), employees.end(),
                      std::mem_fn(&Employee::displayInfo));

        // 5. 按部门分组（使用 mem_fn 提取部门名）
        std::cout << "\n按部门统计:" << std::endl;
        std::vector<std::string> departments;
        std::transform(employees.begin(), employees.end(),
                       std::back_inserter(departments),
                       std::mem_fn(&Employee::department));

        std::sort(departments.begin(), departments.end());
        departments.erase(std::unique(departments.begin(), departments.end()),
                          departments.end());

        for (const auto& dept : departments) {
            int count = std::count_if(
                employees.begin(), employees.end(),
                [&dept](const Employee& emp) {
                    return std::mem_fn(&Employee::department)(emp) == dept;
                });
            std::cout << dept << ": " << count << " 人" << std::endl;
        }
    }
}  // namespace PRACTICAL_APPLICATIONS

namespace ADVANCED_USAGE {
    void task() {
        std::cout << "\n=== MEM_FN 高级用法和注意事项 ===" << std::endl;

        std::cout << "1. mem_fn vs 直接调用的性能对比:" << std::endl;
        std::cout << "   - mem_fn 有轻微的函数对象开销" << std::endl;
        std::cout << "   - 在循环中大量使用时可能影响性能" << std::endl;
        std::cout << "   - 编译器优化通常能消除大部分开销" << std::endl;

        std::cout << "\n2. 与 lambda 表达式的对比:" << std::endl;

        // 示例：两种方式提取成员变量
        std::vector<STD_USAGE::Person> people = {{"Alice", 25}, {"Bob", 30}};
        std::vector<int> ages1, ages2;

        // 使用 mem_fn
        std::transform(people.begin(), people.end(), std::back_inserter(ages1),
                       std::mem_fn(&STD_USAGE::Person::age));

        // 使用 lambda
        std::transform(people.begin(), people.end(), std::back_inserter(ages2),
                       [](const STD_USAGE::Person& p) { return p.age; });

        std::cout << "   mem_fn: 简洁，但可读性可能较差" << std::endl;
        std::cout << "   lambda: 更明确，可读性更好" << std::endl;

        std::cout << "\n3. 适用场景:" << std::endl;
        std::cout << "   - 简单的成员访问：推荐 mem_fn" << std::endl;
        std::cout << "   - 复杂逻辑：推荐 lambda" << std::endl;
        std::cout << "   - 函数式编程风格：mem_fn 更符合" << std::endl;

        std::cout << "\n4. C++20 的改进:" << std::endl;
        std::cout << "   - 概念(Concepts)使模板约束更清晰" << std::endl;
        std::cout << "   - ranges 库提供了更好的函数式操作" << std::endl;
    }
}  // namespace ADVANCED_USAGE

/*
    std::mem_fn 是一个函数模板，用于生成一个可调用对象，该对象可以调用给定的成员函数指针或成员变量指针。
    它可以用于将成员函数或成员变量绑定到一个对象上，从而可以像普通函数一样调用它们。
*/

int main() {
    STD_USAGE::task();
    MY_IMPL::task();
    PRACTICAL_APPLICATIONS::task();
    ADVANCED_USAGE::task();

    return 0;
}