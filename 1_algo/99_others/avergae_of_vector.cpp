#include <immintrin.h>  // AVX512头文件

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

// Implementation 1: 简单的累加求平均值
namespace Implementation1 {
    double calculateAverage1(const std::vector<int>& nums) {
        if (nums.empty()) {
            return 0.0;  // 如果vector为空，返回0.0
        }
        double sum = std::accumulate(nums.begin(), nums.end(), 0.0);
        return sum / nums.size();
    }
}  // namespace Implementation1

namespace Implementation2 {
    // Implementation 2: 使用std::int64_t避免溢出
    double calculateAverage2(const std::vector<int>& nums) {
        if (nums.empty()) {
            return 0.0;  // 空向量检查
        }
        std::int64_t sum = std::accumulate(nums.begin(), nums.end(), 0LL);
        return static_cast<double>(sum) / nums.size();
    }
}  // namespace Implementation2

namespace Implementation3 {
    // Implementation 3: 分批处理避免溢出
    double calculateAverage3(const std::vector<int>& nums) {
        if (nums.empty()) {
            return 0.0;
        }

        std::vector<std::int64_t> partialSums;  // 用于分批累加的容器
        std::int64_t sum = 0;
        for (int num : nums) {
            // 检查加法前是否接近溢出
            if (num > 0 &&
                sum > std::numeric_limits<std::int64_t>::max() - num) {
                // 将当前总和加到分批累加的容器中
                partialSums.push_back(sum);
                sum = 0;  // 重置当前总和
            }
            sum += num;
        }
        // 确保最后一批也被加入
        partialSums.push_back(sum);

        // 计算所有分批累加的总和
        std::int64_t finalSum = 0;
        for (std::int64_t partialSum : partialSums) {
            finalSum += partialSum;
        }

        return static_cast<double>(finalSum) / nums.size();
    }
}  // namespace Implementation3

namespace Implementation4 {
    // Implementation 4: 使用AVX512进行向量加法
    // double calculateAverageAVX512(const std::vector<int>& nums) {
    //     if (nums.empty()) {
    //         return 0.0;
    //     }

    //     __m512i sumVec = _mm512_setzero_si512();  // 初始化为0
    //     int i = 0;
    //     for (; i + 15 < nums.size(); i += 16) {
    //         __m512i dataVec = _mm512_loadu_si512(
    //             reinterpret_cast<const __m512i*>(&nums[i]));  // 加载数据
    //         sumVec = _mm512_add_epi32(sumVec, dataVec);       // 向量加法
    //     }

    //     // 将向量和转换为标量
    //     int sumArray[16];
    //     _mm512_storeu_si512(reinterpret_cast<__m512i*>(sumArray), sumVec);
    //     std::int64_t sum = 0;
    //     for (int j = 0; j < 16; ++j) {
    //         sum += sumArray[j];
    //     }

    //     // 处理剩余的元素
    //     for (; i < nums.size(); ++i) {
    //         sum += nums[i];
    //     }

    //     return static_cast<double>(sum) / nums.size();
    // }
}  // namespace Implementation4

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    assert(Implementation1::calculateAverage1(nums) == 5.5);
    assert(Implementation2::calculateAverage2(nums) == 5.5);
    assert(Implementation3::calculateAverage3(nums) == 5.5);
    // assert(expr(Implementation4::calculateAverageAVX512(nums) == 5.5));

    return 0;
}