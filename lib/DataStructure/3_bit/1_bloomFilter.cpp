#include <bitset>
#include <cmath>
#include <functional>
#include <string>

/*
bloomFilter和哈希表的主要区别就在于bloomFilter不存储数据本身，只判断数据在不在，所以可以轻量化存储。
至于假阳性则是为了实现轻量化存储的trade off。

假设你要处理 1 亿个字符串，并判断某个字符串是否曾经出现过。
    用哈希表：你需要至少几十 GB 的内存存储这些字符串。
    用布隆过滤器：
        你可以只用几百 MB 存储一个位图
        即使有 ~1% 的假阳性率，也能极大地过滤垃圾数据或请求

布隆过滤器不支持删除操作.如果需要支持删除操作,可以使用 Counting Bloom Filter 或 Cuckoo Filter 等变种
*/
template <size_t N>
class BloomFilter {
    static constexpr size_t BitSize = N;
    static constexpr size_t HashCount = 3;  // 可调整哈希函数个数

    std::bitset<BitSize> bits;

    // 利用多个种子来组合多个 hash 函数
    size_t hash(const std::string& data, size_t seed) const {
        std::hash<std::string> hasher;
        return hasher(data + std::to_string(seed)) % BitSize;
    }

  public:
    void add(const std::string& item) {
        for (size_t i = 0; i < HashCount; ++i) {
            bits.set(hash(item, i));
        }
    }

    bool possiblyContains(const std::string& item) const {
        for (size_t i = 0; i < HashCount; ++i) {
            if (!bits.test(hash(item, i))) {
                return false;
            }
        }
        return true;
    }

    void clear() { bits.reset(); }
};
