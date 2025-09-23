#include <cassert>
#include <cstring>  // for standard functions
#include <iostream>

#include "BaseFunction.h"

void test_strcpy() {
    char dest[20];
    const char* src = "Hello, World!";
    strcpy(dest, src);
    assert(std::strcmp(dest, src) == 0);
    std::cout << "strcpy passed\n";
}

void test_memcpy() {
    char src[] = "1234567890";
    char dest[20];
    memcpy(dest, src, 10);
    assert(std::memcmp(dest, src, 10) == 0);
    std::cout << "memcpy passed\n";
}

void test_memmove() {
    char buffer[] = "1234567890";
    memmove(buffer + 4, buffer, 6);
    assert(std::memcmp(buffer, "1234123456", 10) == 0);
    std::cout << "memmove passed\n";
}

void test_atoi() {
    assert(atoi("123") == 123);
    assert(atoi("-123") == -123);
    assert(atoi("  456") == 456);
    assert(atoi("+789") == 789);
    assert(atoi("0") == 0);
    assert(atoi("abc") == 0);
    std::cout << "atoi passed\n";
}

int main() {
    test_strcpy();
    test_memcpy();
    test_memmove();
    test_atoi();
    std::cout << "All tests passed!\n";
    return 0;
}