#include "BaseFunction.h"

#include <cctype>  // for isdigit

char* MyStd::strcpy(char* dest, const char* src) {
    char* original = dest;
    while ((*dest++ = *src++) != '\0');
    return original;
}

void* MyStd::memcpy(void* dest, const void* src, size_t n) {
    char* d = static_cast<char*>(dest);
    const char* s = static_cast<const char*>(src);
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    return dest;
}

void* MyStd::memmove(void* dest, const void* src, size_t n) {
    char* d = static_cast<char*>(dest);
    const char* s = static_cast<const char*>(src);
    if (d < s) {
        for (size_t i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}

int MyStd::atoi(const char* str) {
    int result = 0;
    int sign = 1;

    // 跳过空白字符
    while (isspace(*str)) {
        ++str;
    }

    // 处理符号
    if (*str == '-' || *str == '+') {
        sign = (*str == '-') ? -1 : 1;
        ++str;
    }

    // 转换数字
    while (isdigit(*str)) {
        result = result * 10 + (*str - '0');
        ++str;
    }

    return sign * result;
}