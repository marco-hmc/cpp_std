#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

namespace vec_distance {
    void calc_vec_distance() {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto dist = std::distance(vec.begin(), vec.end());
        assert(dist == 5);

        auto partial_dist = std::distance(vec.begin(), vec.begin() + 2);
        assert(partial_dist == 2);
    }
}  // namespace vec_distance

namespace vec_emplace_back {
    void print(size_t x) { std::cout << x; }

    void task() {
        std::vector<std::thread> v;
        for (size_t i{0}; i < 10; ++i) {
            // if not use emplace_back, will be like
            // v.push_back(std::thread(print, i));
            v.emplace_back(print, i);
        }

        for (auto &t : v) {
            t.join();
        }
    }
}  // namespace vec_emplace_back

namespace vec_remove {
    void original_method() {
        const int target = 2;
        std::vector<int> numbers = {1, 2, 3, 4, 2, 5, 2, 6, 7, 2};
        for (auto it = numbers.begin(); it != numbers.end();) {
            if (*it == target) {
                it = numbers.erase(it);
            } else {
                ++it;
            }
        }
    }

    void erase_remove_trick() {
        std::vector<int> numbers = {1, 2, 3, 4, 2, 5, 2, 6, 7, 2};
        auto new_end = std::remove(numbers.begin(), numbers.end(), 2);
        numbers.erase(new_end, numbers.end());
    }

    void task() {
        original_method();
        erase_remove_trick();
    }
}  // namespace vec_remove

namespace setOperInVec {
    std::vector<int> unique_element_in_vector(std::vector<int> v) {
        std::sort(v.begin(), v.end());
        auto it = std::unique(v.begin(), v.end());
        if (it != v.end()) {
            v.erase(it, v.end());
        }
        return v;
    }

    std::vector<int> vectors_intersection(std::vector<int> v1,
                                          std::vector<int> v2) {
        std::vector<int> v;
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                              std::back_inserter(v));
        return v;
    }

    std::vector<int> vectors_union(std::vector<int> v1, std::vector<int> v2) {
        std::vector<int> v;
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                       std::back_inserter(v));
        return v;
    }

    std::vector<int> vectors_differ(std::vector<int> v1, std::vector<int> v2) {
        std::vector<int> v;
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
                       std::back_inserter(v));
        return v;
    }
}  // namespace setOperInVec

namespace InsertSorted {
    void insert_sorted(std::vector<std::string> &v, const std::string &word) {
        assert(false == std::is_sorted(std::begin(v), std::end(v)));
        const auto insert_pos(
            std::lower_bound(std::begin(v), std::end(v), word));
        v.insert(insert_pos, word);
    }
}  // namespace InsertSorted

namespace vec_remove {
    void normalRemove(std::vector<int> &v) {
        const auto new_end(std::remove(std::begin(v), std::end(v), 2));
        v.erase(new_end, std::end(v));
    }

    void conditionRemove(std::vector<int> &v) {
        auto odd_number([](int i) { return i % 2 != 0; });
        const auto new_end(
            std::remove_if(std::begin(v), std::end(v), odd_number));
        v.erase(new_end, std::end(v));
    }

}  // namespace vec_remove

int main() {}