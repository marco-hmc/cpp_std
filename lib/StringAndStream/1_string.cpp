// #### (48)string的底层实现
// string继承自basic_string,其实是对char\*进行了封装,封装的string包含了char\*数组,容量,长度等等属性.

// string可以进行动态扩展,在每次扩展的时候另外申请一块原空间大小两倍的空间(2^n),然后将原字符串拷贝过去,并加上新增的内容.
// #### (6) 实现一个strcpy函数(或者memcpy),如果内存可能重叠呢

#include <iostream>
#include <string>

namespace string_usage {
    namespace string_to_num {
        void test_1() {
            std::string str1 = std::to_string(123456);
            std::string str = "123";
            int num = std::stoi(str);
            long num1 = std::stol(str);
            double num2 = std::stod(str);
        }
    }  // namespace string_to_num

    namespace string_find {
        void test_1() {
            std::string str = "abcdefg";
            std::string str1 = "fg";
            if (str.find(str1) != std::string::npos) {
                std::cout << "found" << std::endl;
            } else {
                std::cout << "not found" << std::endl;
            }
        }
    }  // namespace string_find

    namespace string_replace {
        void test_1() {
            std::string str = "abcdefg";
            std::string str1 = "fg";
            std::string str2 = "123";
            if (str.find(str1) != std::string::npos) {
                int pos = str.find(str1);
                str.replace(pos, str1.size(), str2);
                std::cout << str << std::endl;
            }
        }
    }  // namespace string_replace

    namespace string_erase {
        void test_1() {
            std::string str = "abcdefg";
            std::string str1 = "fg";
            if (str.find(str1) != std::string::npos) {
                int pos = str.find(str1);
                str.erase(pos, str1.size());
                std::cout << str << std::endl;
            }
        }
    }  // namespace string_erase
}  // namespace string_usage

int main() { return 0; }
