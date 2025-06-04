// 分苹果

// 题目描述:把m个同样的苹果放在n个同样的盘子里,允许有的盘子空着不放,问共有多少种不同的分法?
// 5,1,1和1,5,1 是同一种分法

#include <iostream>
#include <vector>

int putApples(int n, int m) {
    if (n < 0 || m < 0) return 0;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 0; i <= n; i++) dp[i][0] = dp[i][1] = 1;

    for (int i = 1; i <= n; i++) {
        for (int j = 2; j <= m; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j >= i) dp[i][j] += dp[i][j - i];
        }
    }
    return dp[n][m];
}

int main() {
    int n = 3;  // Number of plates
    int m = 5;  // Number of apples

    int result = putApples(n, m);
    std::cout << "Number of different ways to distribute " << m
              << " apples among " << n << " plates: " << result << '\n';

    return 0;
}
