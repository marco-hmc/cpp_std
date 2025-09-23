#include <stdio.h>

#include "StringEager.h"
#include "StringSso.h"

int main() {
    printf("%zd\n", sizeof(muduo::StringEager));
    printf("%zd\n", sizeof(muduo::StringSso));
    muduo::StringEager x;
}
