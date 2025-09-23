// #### string的底层实现和常用操作示例
// string继承自basic_string,其实是对char*进行了封装,封装的string包含了char*数组,容量,长度等等属性.
// string可以进行动态扩展,在每次扩展的时候另外申请一块原空间大小两倍的空间(2^n),然后将原字符串拷贝过去,并加上新增的内容.

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

namespace string_usage {
    
    // 通用的字符串操作辅助类
    class StringHelper {
    public:
        // 安全的字符串查找
        static bool safe_find(const std::string& source, const std::string& target, size_t& pos) {
            pos = source.find(target);
            return pos != std::string::npos;
        }
        
        // 输出操作结果
        static void print_operation_result(const std::string& operation, const std::string& result) {
            std::cout << operation << " -> " << result << std::endl;
        }
        
        // 安全的数值转换
        template<typename T>
        static bool safe_convert(const std::string& str, T& result) {
            try {
                if constexpr (std::is_same_v<T, int>) {
                    result = std::stoi(str);
                } else if constexpr (std::is_same_v<T, long>) {
                    result = std::stol(str);
                } else if constexpr (std::is_same_v<T, double>) {
                    result = std::stod(str);
                }
                return true;
            } catch (const std::exception& e) {
                std::cerr << "转换失败: " << e.what() << std::endl;
                return false;
            }
        }
    };

    namespace string_conversion {
        void demonstrate_conversions() {
            std::cout << "=== 字符串与数值转换示例 ===" << std::endl;
            
            // 数值到字符串
            std::string str_from_int = std::to_string(123456);
            std::string str_from_double = std::to_string(3.14159);
            
            StringHelper::print_operation_result("123456 -> string", str_from_int);
            StringHelper::print_operation_result("3.14159 -> string", str_from_double);
            
            // 字符串到数值（安全转换）
            std::string num_str = "12345";
            std::string double_str = "3.14159";
            std::string invalid_str = "abc123";
            
            int int_result;
            double double_result;
            
            if (StringHelper::safe_convert(num_str, int_result)) {
                std::cout << "成功转换: \"" << num_str << "\" -> " << int_result << std::endl;
            }
            
            if (StringHelper::safe_convert(double_str, double_result)) {
                std::cout << "成功转换: \"" << double_str << "\" -> " << double_result << std::endl;
            }
            
            // 演示转换失败的情况
            int invalid_result;
            StringHelper::safe_convert(invalid_str, invalid_result);
        }
    }

    namespace string_search_and_modify {
        void demonstrate_operations() {
            std::cout << "\n=== 字符串查找和修改示例 ===" << std::endl;
            
            std::string text = "Hello World! This is a test string. Hello again!";
            std::cout << "原始字符串: " << text << std::endl;
            
            // 查找操作
            std::string target = "Hello";
            size_t pos;
            
            if (StringHelper::safe_find(text, target, pos)) {
                std::cout << "找到 \"" << target << "\" 在位置: " << pos << std::endl;
                
                // 查找所有出现位置
                std::cout << "所有 \"" << target << "\" 的位置: ";
                pos = 0;
                while ((pos = text.find(target, pos)) != std::string::npos) {
                    std::cout << pos << " ";
                    pos += target.length();
                }
                std::cout << std::endl;
            }
            
            // 替换操作
            std::string text_copy = text;
            std::string old_word = "Hello";
            std::string new_word = "Hi";
            
            // 替换第一个出现的位置
            if (StringHelper::safe_find(text_copy, old_word, pos)) {
                text_copy.replace(pos, old_word.size(), new_word);
                StringHelper::print_operation_result("替换第一个 \"" + old_word + "\"", text_copy);
            }
            
            // 替换所有出现的位置
            text_copy = text; // 重置
            pos = 0;
            while ((pos = text_copy.find(old_word, pos)) != std::string::npos) {
                text_copy.replace(pos, old_word.size(), new_word);
                pos += new_word.length();
            }
            StringHelper::print_operation_result("替换所有 \"" + old_word + "\"", text_copy);
            
            // 删除操作
            text_copy = text; // 重置
            std::string to_erase = "World! ";
            if (StringHelper::safe_find(text_copy, to_erase, pos)) {
                text_copy.erase(pos, to_erase.size());
                StringHelper::print_operation_result("删除 \"" + to_erase + "\"", text_copy);
            }
        }
    }
    
    namespace string_advanced_operations {
        void demonstrate_advanced() {
            std::cout << "\n=== 高级字符串操作示例 ===" << std::endl;
            
            // 字符串分割
            auto split_string = [](const std::string& str, char delimiter) {
                std::vector<std::string> result;
                std::stringstream ss(str);
                std::string item;
                
                while (std::getline(ss, item, delimiter)) {
                    if (!item.empty()) {
                        result.push_back(item);
                    }
                }
                return result;
            };
            
            std::string csv_data = "apple,banana,cherry,date";
            auto fruits = split_string(csv_data, ',');
            
            std::cout << "分割字符串 \"" << csv_data << "\":" << std::endl;
            for (size_t i = 0; i < fruits.size(); ++i) {
                std::cout << "  [" << i << "] " << fruits[i] << std::endl;
            }
            
            // 字符串连接
            std::string joined = "";
            for (size_t i = 0; i < fruits.size(); ++i) {
                if (i > 0) joined += " | ";
                joined += fruits[i];
            }
            StringHelper::print_operation_result("连接水果名称", joined);
            
            // 字符串修剪
            auto trim = [](const std::string& str) {
                size_t start = str.find_first_not_of(" \t\n\r");
                if (start == std::string::npos) return std::string("");
                size_t end = str.find_last_not_of(" \t\n\r");
                return str.substr(start, end - start + 1);
            };
            
            std::string padded = "  \t  Hello World  \n  ";
            std::string trimmed = trim(padded);
            StringHelper::print_operation_result("修剪空白字符", "\"" + trimmed + "\"");
            
            // 大小写转换
            std::string mixed_case = "Hello World";
            std::string upper_case = mixed_case;
            std::string lower_case = mixed_case;
            
            std::transform(upper_case.begin(), upper_case.end(), upper_case.begin(), ::toupper);
            std::transform(lower_case.begin(), lower_case.end(), lower_case.begin(), ::tolower);
            
            StringHelper::print_operation_result("转换为大写", upper_case);
            StringHelper::print_operation_result("转换为小写", lower_case);
        }
    }
}

int main() {
    string_usage::string_conversion::demonstrate_conversions();
    string_usage::string_search_and_modify::demonstrate_operations();
    string_usage::string_advanced_operations::demonstrate_advanced();
    
    return 0;
}
