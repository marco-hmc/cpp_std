## [基本计算器 II](https://leetcode-cn.com/problems/basic-calculator-ii/)

- 题目描述

  <img src="imgs/algorithm/problem227.png" alt="problem227" style="zoom:80%;" />

- 解题代码

  ```c++
  int calculate(string s) {
      int n = s.size();
      int num = 0;
      char sign = '+';
      vector<int> stk;
      for(int i = 0; i < n; ++i) {
          char ch = s[i];
          if(isdigit(ch)) {
              num = num * 10 + (ch - '0');
          }
          if(!isdigit(ch) && ch != ' ' || i == n-1) {
              switch (sign) {
                  case '+':
                      stk.push_back(num);
                      break;
                  case '-':
                      stk.push_back(-num);
                      break;
                  case '*':
                      stk.back() *= num;
                      break;
                  case '/':
                      stk.back() /= num;
              }
              sign = ch;
              num = 0;
          }
      }
      return accumulate(stk.begin(), stk.end(), 0);
  }
  ```
