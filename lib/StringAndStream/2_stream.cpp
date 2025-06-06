#include <fstream>
#include <iostream>
#include <sstream>

namespace fstream {
    void openFile() {
        std::fstream file;
        file.open("example.txt", std::ios::in);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << '\n';
            }
            file.close();
        } else {
            std::cout << "无法打开文件！" << '\n';
        }
    }
}  // namespace fstream

namespace stream_usage {
    namespace cin_to_stirng {
        void test_1() {
            std::string s;
            std::cin >> s;
            std::cout << s << std::endl;
        }
    }  // namespace cin_to_stirng

    namespace parse_cin {
        void task() {
            std::string str;
            std::getline(std::cin, str, '\n');
            while (!str.empty()) {
                std::getline(std::cin, str, '\n');
            }

            str = "1, 2, 3, 4";
            std::istringstream out(str);
            int num = 0;
            while (out >> num) {
                std::cout << num << std::endl;
            }
        }
    }  // namespace parse_cin

    namespace parse_string {
        void task() {
            std::string str = "1, 2, 3, 4";
            std::istringstream out(str);
            int num = 0;
            while (out >> num) {
                std::cout << num << std::endl;
            }
        }
    }  // namespace parse_string
}  // namespace stream_usage

int main() { return 0; }

// #include <deque>
// #include <iostream>
// #include <iterator>
// #include <sstream>
// #include <string>

// int main() {
//     std::istream_iterator<int> it_cin{std::cin};
//     std::istream_iterator<int> end_cin;

//     std::deque<int> v;
//     std::copy(it_cin, end_cin, std::back_inserter(v));
//     std::istringstream sstr{"123 456 789"};

//     auto deque_middle(std::next(std::begin(v), static_cast<int>(v.size()) / 2));

//     std::copy(std::istream_iterator<int>{sstr}, {},
//               std::inserter(v, deque_middle));

//     std::initializer_list<int> il2{-1, -2, -3};
//     std::copy(std::begin(il2), std::end(il2), std::front_inserter(v));

//     std::copy(std::begin(v), std::end(v),
//               std::ostream_iterator<int>{std::cout, ", "});
//     std::cout << '\n';
// }