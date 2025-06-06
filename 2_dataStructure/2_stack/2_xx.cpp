## [字符串解码](https://leetcode-cn.com/problems/decode-string/)

> 给定一个经过编码的字符串，返回它解码后的字符串。编码规则为: k[encoded_string]，表示其中方括号内部的 encoded_string 正好重复 k 次。注意 k 保证为正整数。你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 k ，例如不会出现像 3a 或 2[4] 的输入

- 解题思路：

  - 本题难点在于括号内嵌套括号，需要**从内向外**生成与拼接字符串，这与栈的**先入后出**特性对应
  - 构建辅助栈 `stk`， 遍历字符串 `s` 中每个字符 `c`
    - 当 `c` 为数字时，将数字字符转化为数字 `num`，用于后续倍数计算
    - 当 `c` 为字母时，在 `ret` 尾部添加 `c`
    - 当 `c` 为 `[` 时，将当前 `num`和 `ret` 入栈，并分别置空置 0，用于重新记录
      - 记录此`[` 前的临时结果 `ret` 至栈，用于发现对应 `]` 后的拼接操作；
      - 记录此 `[ `前的倍数 `num`至栈，用于发现对应 `]` 后，获取 `num`× [...] 字符串
      - 进入到新 `[` 后，`ret` 和` num` 重新记录
    - 当 `c` 为 `] `时，`stk` 出栈，拼接字符串 `ret = last_ret + num* ret`，其中:
    - last_ret 是上个 [ 到当前 [ 的字符串，例如 "3[a2[c]]" 中的 a；
    - `num`是当前 `[ `到 `]` 内字符串的重复倍数，例如 "3[a2[c]]" 中的 2
    - 返回字符串 `ret`

- 代码实现

  ```c++
  string decodeString(string s) {
      int n = s.size();
      string ret, next;
      stack<pair<int, string> > stk;
      int num = 0;
      for(int i = 0; i < n; i++) {
          if(s[i]<='9' && s[i]>='0'){
              num = num*10 + s[i]-'0';
          }
          else if(s[i] == '[') {
              stk.emplace(num, ret);
              num = 0;	// 置零置空
              ret = "";
          }
          else if(s[i] == ']') {
              auto p = stk.top();
              stk.pop();
              while(p.first--) {
                  p.second = p.second + ret;
              }
              ret = std::move(p.second);
          }
          else {
              ret += s[i];
          }
      }
      return ret;
  }
  ```
