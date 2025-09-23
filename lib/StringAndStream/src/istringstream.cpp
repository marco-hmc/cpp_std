#include <iostream>
#include <sstream>

int main() {
    std::istringstream iss("Hello, World!\nThis is a test string.\n1234567890");

    char ch = iss.get();
    std::cout << "get: " << ch << '\n';

    char peek_ch = iss.peek();
    std::cout << "peek: " << peek_ch << '\n';

    iss.unget();
    char unget_ch = iss.get();
    std::cout << "unget: " << unget_ch << '\n';

    iss.putback('X');
    char putback_ch = iss.get();
    std::cout << "putback: " << putback_ch << '\n';

    std::string line;
    std::getline(iss, line);
    std::cout << "getline: " << line << '\n';

    iss.ignore(5, '\n');
    std::getline(iss, line);
    std::cout << "ignore: " << line << '\n';

    char buffer[6];
    iss.read(buffer, 5);
    buffer[5] = '\0';
    std::cout << "read: " << buffer << '\n';

    char readsome_buffer[11];
    std::streamsize n = iss.readsome(readsome_buffer, 10);
    readsome_buffer[n] = '\0';
    std::cout << "readsome: " << readsome_buffer << '\n';

    std::streampos position = iss.tellg();
    std::cout << "tellg: " << position << '\n';

    iss.seekg(0, iss.beg);
    std::getline(iss, line);
    std::cout << "seekg: " << line << '\n';

    std::ostringstream oss;
    oss.seekp(5, std::ostringstream::cur);
    oss << "C++";
    std::cout << "seekp: " << oss.str() << '\n';
    std::cout << "seekp: " << iss.str() << '\n';
    return 0;
}