#include <cassert>
#include <climits>
#include <numeric>
#include <vector>

namespace BinarySearch_findTarget {
    int binarySearch(std::vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size();

        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return mid;
            }

            if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return -1;
    }

    void task() {
        std::vector<int> nums = {1, 3, 5, 7, 9, 11, 13};
        int result = binarySearch(nums, 7);
        assert(result == 3);
    }
}  // namespace BinarySearch_findTarget

namespace FastAndSlowPtr {
    int localRemoveVal(std::vector<int>& nums, int val) {
        int slowIdx = 0;
        for (int fastIdx = 0; fastIdx < nums.size(); fastIdx++) {
            if (nums[fastIdx] != val) {
                nums[slowIdx] = nums[fastIdx];
                slowIdx++;
            }
        }
        return slowIdx;
    }

    void task() {
        std::vector<int> nums{1, 2, 2, 3};
        int ret = localRemoveVal(nums, 2);
        assert(ret == 2);
    }
}  // namespace FastAndSlowPtr

namespace MiniumLengthSubArr {
    int sumUp(const std::vector<int>& nums, int left, int right) {
        if (right >= nums.size()) {
            return std::accumulate(nums.begin() + left, nums.end(), 0);
        }
        return std::accumulate(nums.begin() + left, nums.begin() + right, 0);
    }

    int miniumLengthSubArr(std::vector<int>& nums, int sum) {
        if (nums.empty()) {
            return 0;
        }
        if (nums.size() == 1) {
            return nums[0] == sum ? 1 : 0;
        }

        int ret = INT_MAX;
        int right = 1;
        for (int left = 0; left < nums.size();) {
            if (right >= nums.size()) {
                left++;
                continue;
            }
            int curSum = sumUp(nums, left, right);
            if (curSum == sum) {
                ret = std::min(ret, right - left);
                left++;
            } else if (curSum < sum) {
                right++;
            } else {
                left++;
            }
        }
        return ret == INT_MAX ? 0 : ret;
    }

    void task() {
        std::vector<int> nums{2, 3, 1, 2, 4, 3};
        int ret = miniumLengthSubArr(nums, 7);
        assert(ret == 2);
    }

}  // namespace MiniumLengthSubArr

int main() {
    MiniumLengthSubArr::task();
    FastAndSlowPtr::task();
    BinarySearch_findTarget::task();
    return 0;
}
