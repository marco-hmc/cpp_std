#pragma once

#include <cstddef>

namespace MyStd {

    char* strcpy(char* dest, const char* src);

    void* memcpy(void* dest, const void* src, size_t n);

    void* memmove(void* dest, const void* src, size_t n);

    int atoi(const char* str);
}  // namespace MyStd