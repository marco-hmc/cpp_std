#排序

## [最大数](https://leetcode-cn.com/problems/largest-number/)

- 思路

  - 要想组成最大的整数，**一种直观的想法是把数值大的数放在高位**。于是我们**可以比较输入数组的每个元素的最高位，最高位相同的时候比较次高位，以此类推，完成排序，然后把它们拼接起来**。这种排序方式对于输入数组 没有相同数字开头的时候是有效的

  <img src="imgs/algorithm/max_num.jpg" alt="max_num" style="zoom: 80%;" />

- 代码实现

  ```c++
  string largestNumber(vector<int>& nums) {
    //
    auto comp = [](int &x, int &y) -> bool {
        long sx = 10, sy = 10;
        while (sx <= x) {
            sx *= 10;
        }
        while (sy <= y) {
            sy *= 10;
        }
        return x * sy + y > y * sx + x;
    };

    sort(nums.begin(), nums.end(), comp);
    if (nums[0] == 0) {
        return "0";
    }
    string ret;
    for (auto &num : nums) {
        ret += to_string(num);
    }
    return ret;
  }
  ```

## 数组中的逆序对

- 题目描述

  - 在数组中的两个数字，如果前面一个数字大于后面的数字，则这两个数字组成一个逆序对。输入一个数组,求出这个数组中的逆序对的总数 P。并将 P 对 1000000007 取模的结果输出。 即输出 P%1000000007

- 解题思路

  - 利用归并排序，自底向上归并过程，如果出现待归并左子数组中 left 下标数字大于右子数组中 right 下标数字，由于左右数组都是正序，则左子数组 left 右边的数都与 right 所指的数组成逆序对，逆序对总数增加 mid-left+1 对

- 代码实现

  ```c++
  class Solution {
  public:
    int count = 0;
    int InversePairs(vector<int> data) {
        if (data.empty()) return 0;
        vector<int> tmp(data);
        division(data, tmp, 0, data.size() - 1);
        return count;
    }

    void division(vector<int> & array, vector<int> & tmp, int left, int right) {
        if (left >= right) {
            return;
        }
        int mid = left + (right - left) / 2;

        division(array, tmp, left, mid);
        division(array, tmp, mid + 1, right);

        int idx = left, rightIdx = mid + 1, leftIdx = left;
        // merge
        while (leftIdx <= mid && rightIdx <= right) {
            if (array[leftIdx] <= array[rightIdx]) {
                tmp[idx++] = array[leftIdx++];
            } else {
                // 逆序，则由于array[left...mid]已经是正序，
                // left右边都与array[right]组成逆序对
                count += mid - leftIdx + 1;
                count %= 1000000007;
                tmp[idx++] = array[rightIdx++];
            }
        }
        while (leftIdx <= mid) tmp[idx++] = array[leftIdx++];
        while (rightIdx <= right) tmp[idx++] = array[rightIdx++];
        for (int i = left; i <= right; ++i) array[i] = tmp[i];
    }
  };
  ```

#include <iostream>
#include <vector>

namespace bubbleSort {
    void bubbleSort(std::vector<int> vec) {
        for (int i = 0; i < vec.size() - 1; i++) {
            for (int j = 0; j < vec.size() - i - 1; j++) {
                if (vec[j] > vec[j + 1]) {
                    std::swap(vec[j], vec[j + 1]);
                }
            }
        }
    }
}  // namespace bubbleSort

namespace QuickSort {
    // 这段代码实现了快速排序算法,这是一种非常高效的排序算法,其基本思想是使用分治策略.

    // [`partition`]函数是快速排序的核心部分.
    // 它接受一个数组和两个索引 `low` 和  `high`
    // 并将数组划分为两部分,使得左边的元素都小于一个选定的枢轴元素,右边的元素都大于枢轴元素.
    // 这个函数首先选择 `high` 索引处的元素作为枢轴,
    // 然后从 `low` 到 `high - 1` 遍历数组.
    // 如果遇到的元素小于枢轴,就将其与数组中的下一个元素交换位置.
    // 最后,将枢轴元素放到正确的位置,使得它左边的元素都小于它,右边的元素都大于它.函数返回枢轴元素的索引.

    // ['swap']函数用于交换两个元素的位置.它接受两个元素的引用,并使用
    // [`iter_swap`]函数交换它们的值.注意,如果两个元素的地址相同,即它们实际上是同一个元素,那么
    // [`swap`]函数不会做任何操作.

    // [`quickSort`]函数是快速排序的主函数.它接受一个数组和两个索引 `low` 和
    // `high`,并对索引范围内的数组进行排序.首先,它检查 `low` 是否小于
    // `high`,如果不是,那么数组已经是有序的,函数直接返回.否则,它使用
    // [`partition`]函数将数组划分为两部分,并得到枢轴元素的索引.然后,它对枢轴左边和右边的子数组分别进行快速排序.这是一个递归过程,直到所有的子数组都被排序.

    int partition(std::vector<int> &arr, int low, int high) {
        int pivot = arr[high];
        int i = low;

        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                std::swap(arr[i], arr[j]);
                i++;
            }
        }
        std::swap(arr[i], arr[high]);
        return i;
    }

    void quickSort(std::vector<int> &arr, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(arr, low, high);
            quickSort(arr, low, pivotIndex - 1);
            quickSort(arr, pivotIndex + 1, high);
        }
    }
}  // namespace QuickSort

namespace shellSort {
    void shellSort(std::vector<int> &arr) {
        int n = arr.size();
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                int temp = arr[i];
                int j;
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                    arr[j] = arr[j - gap];
                }
                arr[j] = temp;
            }
        }
    }
}  // namespace shellSort

namespace MergeSort {
    void merge(std::vector<int> &arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> L(n1);
        std::vector<int> R(n2);

        for (int i = 0; i < n1; i++) {
            L[i] = arr[left + i];
        }
        for (int j = 0; j < n2; j++) {
            R[j] = arr[mid + 1 + j];
        }

        int i = 0;
        int j = 0;
        int k = left;

        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }
    void mergeSort(std::vector<int> &arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
}  // namespace MergeSort

namespace InsertionSort {
    // Time Complexity: O(N^2)
    // Space Complexity: O(N)
    void InsertionSort(std::vector<int> vec) {
        for (int i = 1; i < vec.size(); i++) {
            int key = vec[i];
            int j = i - 1;
            while (j >= 0 && vec[j] > key) {
                vec[j + 1] = vec[j];
                j = j - 1;
            }
            vec[j + 1] = key;
        }
    }

}  // namespace InsertionSort

namespace HeapSort {
    void swap(std::vector<int> &arr, int a, int b) {
        arr[a] = arr[a] ^ arr[b];
        arr[b] = arr[a] ^ arr[b];
        arr[a] = arr[a] ^ arr[b];
    }
    void adjust(std::vector<int> &arr, int len, int index) {
        int maxid = index;

        int left = 2 * index + 1, right = 2 * index + 2;

        if (left < len and arr[left] < arr[maxid]) maxid = left;
        if (right < len and arr[right] < arr[maxid]) maxid = right;

        if (maxid != index) {
            swap(arr, maxid, index);
            adjust(arr, len, maxid);
        }
    }
    void heapsort(std::vector<int> &arr, int len) {
        for (int i = (len - 1 - 1) / 2; i >= 0; i--) {
            adjust(arr, len, i);
        }

        for (int i = len - 1; i > 0; i--) {
            swap(arr, 0, i);
            adjust(arr, i, 0);
        }
    }
}  // namespace HeapSort

namespace SelectSort {
    void selectSort(std::vector<int> vec) {
        for (int i = 0; i < vec.size() - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < vec.size(); j++) {
                if (vec[j] < vec[minIndex]) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                std::swap(vec[i], vec[minIndex]);
            }
        }
    }

}  // namespace SelectSort

namespace BuckerSort {
    struct Node {
        float data;
        Node *next;
    };

    void selectionSort(Node *head) {
        Node *temp = head;

        while (temp != nullptr) {
            Node *m = temp;
            Node *r = temp->next;

            while (r != nullptr) {
                if (m->data > r->data) {
                    m = r;
                }
                r = r->next;
            }

            float x = temp->data;
            temp->data = m->data;
            m->data = x;
            temp = temp->next;
        }
    }

    void bucket_sort(float arr[], int n) {
        std::vector<Node *> bucket(n, nullptr);
        for (int i = 0; i < n; i++) {
            int bi = n * arr[i];
            Node *newNode = new Node();
            newNode->data = arr[i];
            newNode->next = nullptr;
            if (bucket[bi] == nullptr) {
                bucket[bi] = newNode;
            } else {
                Node *temp = bucket[bi];
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }

        for (int i = 0; i < n; i++) {
            selectionSort(bucket[i]);
        }
        int ct = 0;
        for (int i = 0; i < n; i++) {
            while (bucket[i] != nullptr) {
                arr[ct] = bucket[i]->data;
                ct++;
                bucket[i] = bucket[i]->next;
            }
        }
        for (int i = 0; i < n; i++) {
            std::cout << arr[i] << " ";
        }
    }

}  // namespace BuckerSort

int main() {
    return 0; }
