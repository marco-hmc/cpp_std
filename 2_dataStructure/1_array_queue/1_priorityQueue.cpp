#include <cassert>
#include <iostream>
#include <queue>
#include <vector>

// 1. priority_queue如何自定义比较函数
// 2. priority_queue如何构造

namespace priority_queue {
    void task() {
        using item_type = std::pair<int, std::string>;
        std::priority_queue<item_type> q;
        std::initializer_list<item_type> il{
            {1, "dishes"},
            {0, "watch tv"},
            {2, "do homework"},
            {0, "read comics"},
        };
        for (const auto& p : il) {
            q.push(p);
        }
        while (!q.empty()) {
            std::cout << q.top().first << ": " << q.top().second << '\n';
            q.pop();
        }
        std::cout << '\n';
    }
}  // namespace priority_queue

namespace FindSmallestNumbers {
    std::vector<int> findSmallestNumbers(const std::vector<int>& arr, int k) {
        std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
        for (int i = 0; i < k; i++) {
            minHeap.push(arr[i]);
        }

        for (int i = k; i < arr.size(); i++) {
            if (arr[i] < minHeap.top()) {
                minHeap.pop();
                minHeap.push(arr[i]);
            }
        }

        std::vector<int> smallestNumbers;
        while (!minHeap.empty()) {
            smallestNumbers.push_back(minHeap.top());
            minHeap.pop();
        }

        return smallestNumbers;
    }

    void task() {
        std::vector<int> arr = {9, 4, 2, 7, 5, 1, 8, 3, 6, 10, 12, 11};

        std::vector<int> smallestNumbers = findSmallestNumbers(arr, 3);
        assert(smallestNumbers[0] == 1);
        assert(smallestNumbers[1] == 2);
        assert(smallestNumbers[2] == 3);
    }
}  // namespace FindSmallestNumbers

namespace FindClosestNumbers {
    std::vector<int> findClosestNumbers(const std::vector<int>& arr, int k,
                                        int m) {
        // 找出排序数组中离第k个数最近的m个数
        if (k < 0 || k >= static_cast<int>(arr.size()) || m <= 0) {
            return {};
        }

        auto cmp = [&arr, k](int i, int j) {
            return std::abs(arr[i] - arr[k]) < std::abs(arr[j] - arr[k]);
        };

        std::priority_queue<int, std::vector<int>, decltype(cmp)> maxHeap(cmp);

        for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
            if (i == k) continue;
            maxHeap.push(i);
            if (static_cast<int>(maxHeap.size()) > m) {
                maxHeap.pop();
            }
        }

        std::vector<int> closestNumbers;
        while (!maxHeap.empty()) {
            closestNumbers.push_back(arr[maxHeap.top()]);
            maxHeap.pop();
        }

        std::sort(closestNumbers.begin(), closestNumbers.end());
        return closestNumbers;
    }

    void task() {
        std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
        int k = 7;
        int m = 3;

        std::vector<int> closestNumbers = findClosestNumbers(arr, k, m);

        std::cout << "Closest numbers to " << k << ": ";
        for (int num : closestNumbers) {
            std::cout << num << " ";
        }
        std::cout << '\n';
    }
}  // namespace FindClosestNumbers

int main() {
    FindSmallestNumbers::task();
    FindClosestNumbers::task();
    return 0;
}
