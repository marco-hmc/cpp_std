#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace DFS_GenerateParenthesis {
    void dfs(std::vector<std::string> &ret, std::string &curStr, int left,
             int right) {
        if (left == 0 && right == 0) {
            ret.push_back(curStr);
            return;
        }
        if (left > 0) {
            curStr.push_back('(');
            dfs(ret, curStr, left - 1, right);
            curStr.pop_back();
        }
        if (right > left) {
            curStr.push_back(')');
            dfs(ret, curStr, left, right - 1);
            curStr.pop_back();
        }
    }

    std::vector<std::string> generateParenthesis(int n) {
        if (n <= 0) return {};
        std::vector<std::string> ret;
        std::string tmp = std::string("");
        dfs(ret, tmp, n, n);
        return ret;
    }

    void task() {
        int n = 3;  // Replace with desired value
        std::vector<std::string> result = generateParenthesis(n);
        for (const std::string &str : result) {
            std::cout << str << std::endl;
        }
    }
}  // namespace DFS_GenerateParenthesis

namespace dfs_EightQueen {

    std::vector<std::vector<int>> solutions;

    bool isSafe(const std::vector<int> &board, int N, int row, int col) {
        for (int i = 0; i < row; i++) {
            if (board[i] == col) {
                return false;
            }
        }

        for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
            if (board[i] == j) {
                return false;
            }
        }

        for (int i = row, j = col; i >= 0 && j < N; i--, j++) {
            if (board[i] == j) {
                return false;
            }
        }

        return true;
    }

    void solveNQueens(std::vector<int> &board, int N, int row) {
        if (row == N) {
            solutions.push_back(board);
            return;
        }

        for (int col = 0; col < N; col++) {
            if (isSafe(board, N, row, col)) {
                board[row] = col;
                solveNQueens(board, N, row + 1);
            }
        }
    }

    void printSolution(const std::vector<std::vector<int>> &solutions, int N) {
        for (const auto &solution : solutions) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (solution[i] == j) {
                        std::cout << "Q ";
                    } else {
                        std::cout << ". ";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    void task() {
        std::vector<std::thread> threads;
        const int N = 8;
        for (int i = 0; i < N; i++) {
            std::vector<int> board(N, -1);
            board[0] = i;
            threads.push_back(std::thread(solveNQueens, std::ref(board), 1));
        }

        for (auto &thread : threads) {
            thread.join();
        }
        printSolution(solutions, N);
    }
}  // namespace dfs_EightQueen

int main() {
    DFS_GenerateParenthesis::task();
    dfs_EightQueen::task();

    return 0;
}