## [括号的分数](https://leetcode-cn.com/problems/score-of-parentheses/)

```c++
int scoreOfParentheses(string s) {
    stack<int> stk;
    stk.push(0);
    for(auto ch : s) {
        if(ch == '(') {
            stk.push(0);
        }
        else {
            int cur = stk.top();
            stk.pop();
            int last = stk.top();
            stk.pop();
            stk.push(max(cur * 2, 1) + last);
        }
    }
    return stk.top();
}
```

