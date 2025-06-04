#include <bitset>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

constexpr int MAX_N = 10;

int N = 4;
std::vector<std::vector<int>> board(MAX_N, std::vector<int>(MAX_N, 0));
std::vector<std::bitset<MAX_N>> xmask(MAX_N), ymask(MAX_N);
int64_t count = 0;

void backtrack(int x, int y)
{
    assert(x < N && y < N);
    std::bitset<MAX_N> mask = xmask[x] | ymask[y];
    std::bitset<MAX_N> avail = ~mask;
    for (int i = 0; i < N; ++i) {
        if (avail[i]) {
            board[x][y] = i + 1;
            std::bitset<MAX_N> oldxmask = xmask[x];
            std::bitset<MAX_N> oldymask = ymask[y];
            assert(!oldxmask[i]);
            assert(!oldymask[i]);
            xmask[x][i] = true;
            ymask[y][i] = true;
            if (x == N - 1 && y == N - 1)
                ++count;
            else if (x == N - 1)
                backtrack(1, y + 1);
            else
                backtrack(x + 1, y);
            xmask[x] = oldxmask;
            ymask[y] = oldymask;
        }
    }
}

void put(int x, int y, int i)
{
    board[x][y] = i;
    xmask[x][i - 1] = true;
    ymask[y][i - 1] = true;
}

int main(int argc, char* argv[])
{
    N = argc > 1 ? std::stoi(argv[1]) : 4;

    for (int i = 0; i < N; ++i) {
        put(i, 0, i + 1);
        put(0, i, i + 1);
    }

    backtrack(1, 1);
    std::cout << count << std::endl;
}