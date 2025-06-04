#include <iostream>
#include <vector>

// 这个问题是经典的 0/1 背包问题。给定一个背包的容量和一组物品，每个物品都有一个重量和一个价值。目标是确定哪些物品可以放入背包，以使得在不超过背包容量的前提下，背包中的物品总价值最大。

// 具体来说：
// - 有一个背包，容量为 `capacity`。
// - 有 `n` 个物品，每个物品有一个重量 `weights[i]` 和一个价值 `values[i]`。
// - 每个物品只能选择一次（0/1 背包问题）。
int knapsack(int capacity, std::vector<int> &weights,
             std::vector<int> &values) {
    int n = weights.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= capacity; j++) {
            if (weights[i - 1] <= j) {
                dp[i][j] =
                    std::max(dp[i - 1][j],
                             dp[i - 1][j - weights[i - 1]] + values[i - 1]);
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    return dp[n][capacity];
}

int main() {
    int capacity = 10;
    std::vector<int> weights = {2, 3, 4, 5};
    std::vector<int> values = {3, 4, 5, 6};

    int max_value = knapsack(capacity, weights, values);
    std::cout << "The maximum value that can be obtained is: " << max_value
              << std::endl;

    return 0;
}
