#数学

## [计数质数](https://leetcode-cn.com/problems/count-primes/)

## 判断一个数是否 2 的幂次方

- 2 的幂次方的数的二进制表示，都是只有一个 1，后面跟 n 个 0，所以只需判断 num & (num-1)是否等于 0 即可

## [分数到小数](https://leetcode-cn.com/problems/fraction-to-recurring-decimal/)

```c++
//小数部分如果余数重复出现两次就表示该小数是循环小数了
string fractionToDecimal(int numerator, int denominator) {
    if (denominator == 0) return "";  //边界条件，分母为0
    if (numerator == 0) return "0";   //边界条件，分子为0
    string result;

    //转换为longlong防止溢出
    long long num = static_cast<long long>(numerator);
    long long denom = static_cast<long long>(denominator);

    //处理正负号，一正一负取负号
    if ((num > 0) ^ (denom > 0)) result.push_back('-');

    //分子分母全部转换为正数
    num = llabs(num);
    denom = llabs(denom);

    //处理整数部分
    result.append(to_string(num / denom));

    //处理小数部分
    num %= denom;                 //获得余数
    if (num == 0) return result;  //余数为0，表示整除了，直接返回结果
    result.push_back('.');        //余数不为0，添加小数点
    int index = result.size() - 1;  //获得小数点的下标
    unordered_map<int, int>
        record;  //map用来记录出现重复数的下标，然后将'('插入到重复数前面就好了
    while (num && record.count(num) ==
                      0) {  //小数部分：余数不为0且余数还没有出现重复数字
        record[num] = ++index;
        num *= 10;  //余数扩大10倍，然后求商，和草稿本上运算方法是一样的
        result += to_string(num / denom);
        num %= denom;
    }
    if (record.count(num) ==
        1) {  //出现循环余数，我们直接在重复数字前面添加'(',字符串末尾添加')'
        result.insert(record[num], "(");
        result.push_back(')');
    }
    return result;
}
```

## [两数相除](https://leetcode-cn.com/problems/divide-two-integers/)

## pow(x, n)快速幂

```c++
double myPow(double x, int n) {
    bool nagative = n < 0;
    if (n == 0) {
        return 1;
    }
    n = abs(n);
    double ret = 1;
    while (n > 0) {
        if (n & 1) {
            ret *= x;
        }
        x *= x;
        n /= 2;
    }
    if (nagative) {
        return 1 / ret;
    }
    return ret;
}
```
