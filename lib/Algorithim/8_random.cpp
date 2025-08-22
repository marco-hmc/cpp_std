// std 随机数算法用法详解和自定义实现

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <set>
#include <vector>

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD 随机数算法标准用法 ===" << std::endl;

        // 1. random_shuffle (C++14 弃用, C++17 移除)
        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "原数据: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 使用 shuffle 替代 random_shuffle
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(data.begin(), data.end(), g);

        std::cout << "shuffle 后: ";
        for (int n : data) std::cout << n << " ";
        std::cout << std::endl;

        // 2. sample - 随机采样
        std::vector<int> large_data;
        for (int i = 1; i <= 100; ++i) {
            large_data.push_back(i);
        }

        std::vector<int> samples;
        std::sample(large_data.begin(), large_data.end(),
                    std::back_inserter(samples), 10, g);

        std::cout << "\n从1-100中随机采样10个: ";
        for (int s : samples) std::cout << s << " ";
        std::cout << std::endl;

        // 3. generate 和 generate_n - 生成随机数
        std::vector<int> random_nums(10);
        std::uniform_int_distribution<> dis(1, 100);

        std::generate(random_nums.begin(), random_nums.end(),
                      [&]() { return dis(g); });

        std::cout << "\ngenerate 生成10个随机数: ";
        for (int n : random_nums) std::cout << n << " ";
        std::cout << std::endl;

        std::vector<int> more_randoms;
        std::generate_n(std::back_inserter(more_randoms), 5,
                        [&]() { return dis(g); });

        std::cout << "generate_n 生成5个随机数: ";
        for (int n : more_randoms) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace STD_USAGE

namespace MY_IMPL {
    // 自定义实现 shuffle (Fisher-Yates算法)
    template <class RandomIt, class URBG>
    void my_shuffle(RandomIt first, RandomIt last, URBG&& g) {
        using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
        using distr_t = std::uniform_int_distribution<diff_t>;
        using param_t = typename distr_t::param_type;

        distr_t D;
        diff_t n = last - first;
        for (diff_t i = n - 1; i > 0; --i) {
            using std::swap;
            swap(first[i], first[D(g, param_t(0, i))]);
        }
    }

    // 自定义实现 sample (水塘抽样算法)
    template <class PopulationIterator, class SampleIterator, class Distance,
              class URBG>
    SampleIterator my_sample(PopulationIterator first, PopulationIterator last,
                             SampleIterator out, Distance n, URBG&& g) {
        using distr_t = std::uniform_int_distribution<Distance>;
        using param_t = typename distr_t::param_type;

        distr_t D;
        Distance unsampled_sz = std::distance(first, last);

        for (n = std::min(n, unsampled_sz); n != 0; ++first) {
            if (D(g, param_t(0, --unsampled_sz)) < n) {
                *out++ = *first;
                --n;
            }
        }
        return out;
    }

    void task() {
        std::cout << "\n=== 自定义随机数算法实现测试 ===" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());

        // 测试自定义 shuffle
        std::vector<int> test_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "自定义 shuffle 测试:" << std::endl;
        std::cout << "原数据: ";
        for (int n : test_data) std::cout << n << " ";
        std::cout << std::endl;

        my_shuffle(test_data.begin(), test_data.end(), gen);
        std::cout << "自定义shuffle后: ";
        for (int n : test_data) std::cout << n << " ";
        std::cout << std::endl;

        // 测试自定义 sample
        std::vector<int> population;
        for (int i = 1; i <= 50; ++i) {
            population.push_back(i);
        }

        std::vector<int> my_samples;
        my_sample(population.begin(), population.end(),
                  std::back_inserter(my_samples), 8, gen);

        std::cout << "\n自定义 sample 测试 (从1-50中采样8个): ";
        for (int s : my_samples) std::cout << s << " ";
        std::cout << std::endl;
    }
}  // namespace MY_IMPL

namespace RANDOM_ENGINES {
    void task() {
        std::cout << "\n=== 随机数引擎详解 ===" << std::endl;

        // 1. 不同的随机数引擎
        std::cout << "不同随机数引擎对比:" << std::endl;

        // 线性同余引擎
        std::minstd_rand lcg;
        std::cout << "线性同余引擎 (minstd_rand): ";
        for (int i = 0; i < 5; ++i) {
            std::cout << lcg() << " ";
        }
        std::cout << std::endl;

        // 梅森旋转算法
        std::mt19937 mt;
        std::cout << "梅森旋转 (mt19937): ";
        for (int i = 0; i < 5; ++i) {
            std::cout << mt() << " ";
        }
        std::cout << std::endl;

        // subtract_with_carry 引擎
        std::ranlux24 swc;
        std::cout << "subtract_with_carry (ranlux24): ";
        for (int i = 0; i < 5; ++i) {
            std::cout << swc() << " ";
        }
        std::cout << std::endl;

        // 2. 随机设备
        std::cout << "\n随机设备特性:" << std::endl;
        std::random_device rd;
        std::cout << "random_device 熵值: " << rd.entropy() << std::endl;
        std::cout << "random_device 随机数: ";
        for (int i = 0; i < 3; ++i) {
            std::cout << rd() << " ";
        }
        std::cout << std::endl;

        // 3. 种子设置
        std::cout << "\n种子设置演示:" << std::endl;
        std::mt19937 gen1(12345);  // 固定种子
        std::mt19937 gen2(12345);  // 相同种子

        std::cout << "相同种子的两个引擎: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << "(" << gen1() << "," << gen2() << ") ";
        }
        std::cout << std::endl;
    }
}  // namespace RANDOM_ENGINES

namespace DISTRIBUTIONS {
    void task() {
        std::cout << "\n=== 概率分布详解 ===" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());

        // 1. 均匀分布
        std::uniform_int_distribution<> int_dis(1, 6);
        std::cout << "均匀整数分布 (1-6): ";
        for (int i = 0; i < 10; i++) {
            std::cout << int_dis(gen) << " ";
        }
        std::cout << std::endl;

        std::uniform_real_distribution<> real_dis(0.0, 1.0);
        std::cout << "均匀实数分布 (0-1): ";
        for (int i = 0; i < 5; i++) {
            std::cout << std::fixed << std::setprecision(3) << real_dis(gen)
                      << " ";
        }
        std::cout << std::endl;

        // 2. 正态分布
        std::normal_distribution<> normal_dis(0.0, 1.0);
        std::cout << "标准正态分布: ";
        for (int i = 0; i < 5; i++) {
            std::cout << std::fixed << std::setprecision(3) << normal_dis(gen)
                      << " ";
        }
        std::cout << std::endl;

        // 3. 伯努利分布
        std::bernoulli_distribution bernoulli_dis(0.7);
        std::cout << "伯努利分布 (p=0.7): ";
        for (int i = 0; i < 10; i++) {
            std::cout << bernoulli_dis(gen) << " ";
        }
        std::cout << std::endl;

        // 4. 泊松分布
        std::poisson_distribution<> poisson_dis(2.0);
        std::cout << "泊松分布 (λ=2): ";
        for (int i = 0; i < 10; i++) {
            std::cout << poisson_dis(gen) << " ";
        }
        std::cout << std::endl;
    }
}  // namespace DISTRIBUTIONS

namespace PRACTICAL_APPLICATIONS {
    void task() {
        std::cout << "\n=== 实际应用场景 ===" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());

        // 1. 随机密码生成
        std::string charset =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::uniform_int_distribution<> char_dis(0, charset.size() - 1);

        std::cout << "随机密码生成: ";
        std::string password;
        std::generate_n(std::back_inserter(password), 12,
                        [&]() { return charset[char_dis(gen)]; });
        std::cout << password << std::endl;

        // 2. 随机选择算法
        std::vector<std::string> options = {"选项A", "选项B", "选项C", "选项D",
                                            "选项E"};
        std::uniform_int_distribution<> option_dis(0, options.size() - 1);

        std::cout << "随机选择: " << options[option_dis(gen)] << std::endl;

        // 3. 蒙特卡罗方法估算π
        std::uniform_real_distribution<> unit_dis(-1.0, 1.0);
        int inside_circle = 0;
        int total_points = 100000;

        for (int i = 0; i < total_points; i++) {
            double x = unit_dis(gen);
            double y = unit_dis(gen);
            if (x * x + y * y <= 1.0) {
                inside_circle++;
            }
        }

        double pi_estimate = 4.0 * inside_circle / total_points;
        std::cout << "蒙特卡罗估算π: " << std::fixed << std::setprecision(6)
                  << pi_estimate << std::endl;

        // 4. 数据随机化测试
        std::vector<int> test_data;
        for (int i = 1; i <= 20; i++) {
            test_data.push_back(i);
        }

        std::cout << "\n数据随机化测试:" << std::endl;
        std::cout << "原数据: ";
        for (int n : test_data) std::cout << n << " ";
        std::cout << std::endl;

        // 随机打乱用于测试
        std::shuffle(test_data.begin(), test_data.end(), gen);
        std::cout << "随机化后: ";
        for (int n : test_data) std::cout << n << " ";
        std::cout << std::endl;
    }
}  // namespace PRACTICAL_APPLICATIONS

namespace PERFORMANCE_TIPS {
    void task() {
        std::cout << "\n=== 性能提示和最佳实践 ===" << std::endl;

        std::cout << "1. 引擎选择建议:" << std::endl;
        std::cout << "   - mt19937: 质量高，适合大多数应用" << std::endl;
        std::cout << "   - minstd_rand: 速度快，质量一般，适合简单应用"
                  << std::endl;
        std::cout << "   - random_device: 真随机，速度慢，适合种子生成"
                  << std::endl;

        std::cout << "\n2. 性能对比测试:" << std::endl;

        // 性能测试
        const int iterations = 1000000;

        auto start = std::chrono::high_resolution_clock::now();
        std::mt19937 mt_gen;
        for (int i = 0; i < iterations; i++) {
            mt_gen();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto mt_duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        start = std::chrono::high_resolution_clock::now();
        std::minstd_rand lcg_gen;
        for (int i = 0; i < iterations; i++) {
            lcg_gen();
        }
        end = std::chrono::high_resolution_clock::now();
        auto lcg_duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "   mt19937 (" << iterations
                  << "次): " << mt_duration.count() << "μs" << std::endl;
        std::cout << "   minstd_rand (" << iterations
                  << "次): " << lcg_duration.count() << "μs" << std::endl;

        std::cout << "\n3. 最佳实践:" << std::endl;
        std::cout << "   - 重复使用引擎对象，避免频繁创建" << std::endl;
        std::cout << "   - 使用thread_local避免多线程竞争" << std::endl;
        std::cout << "   - 根据需要选择合适的分布" << std::endl;
        std::cout << "   - 对于加密应用，使用random_device" << std::endl;
    }
}  // namespace PERFORMANCE_TIPS

int main() {
    // 原始示例
    std::cout << "=== 原始示例 ===" << std::endl;

    const size_t data_points{1000};  // 减少数据量便于演示
    const size_t sample_points{20};
    const int mean{10};
    const size_t dev{3};
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{mean, dev};
    std::vector<int> v;
    v.reserve(data_points);

    std::generate_n(std::back_inserter(v), data_points, [&] { return d(gen); });
    std::vector<int> samples;
    samples.reserve(sample_points);  // 修正错误：应该是samples而不是v
    std::sample(std::begin(v), std::end(v), std::back_inserter(samples),
                sample_points, std::mt19937{std::random_device{}()});
    std::map<int, size_t> hist;

    for (int i : samples) {
        ++hist[i];
    }
    for (const auto& [value, count] : hist) {
        std::cout << std::setw(2) << value << " " << std::string(count, '*')
                  << '\n';
    }

    // 调用详细示例
    STD_USAGE::task();
    MY_IMPL::task();
    RANDOM_ENGINES::task();
    DISTRIBUTIONS::task();
    PRACTICAL_APPLICATIONS::task();
    PERFORMANCE_TIPS::task();

    return 0;
}