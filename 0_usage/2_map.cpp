#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

namespace map_usage {

    std::string filter_punctuation(const std::string &s) {
        const char *forbidden{".,:; "};
        const auto idx_start(s.find_first_not_of(forbidden));
        const auto idx_end(s.find_last_not_of(forbidden));
        return s.substr(idx_start, idx_end - idx_start + 1);
    }
    void task() {
        std::map<std::string, size_t> words;
        int max_word_len{0};
        std::string s;

        while (std::cin >> s) {
            auto filtered(filter_punctuation(s));
            max_word_len = std::max<int>(max_word_len, filtered.length());
            ++words[filtered];
        }

        std::vector<std::pair<std::string, size_t>> word_counts;
        word_counts.reserve(words.size());
        std::move(std::begin(words), std::end(words),
                  std::back_inserter(word_counts));
        std::sort(
            std::begin(word_counts), std::end(word_counts),
            [](const auto &a, const auto &b) { return a.second > b.second; });

        std::cout << "# " << std::setw(max_word_len) << "<WORD>"
                  << " #<COUNT>\n";
        for (const auto &[word, count] : word_counts) {
            std::cout << std::setw(max_word_len + 2) << word << " #" << count
                      << '\n';
        }
    }
}  // namespace map_usage

namespace unordered_map_usage {
    void task() {
        std::unordered_map<int, int> hashmap{{0, 0}, {2, 3}};
        hashmap[1] = 2;
        std::cout << "The value of key 1 is: " << hashmap[1] << std::endl;
        hashmap.erase(2);
        if (!hashmap.contains(2)) {
            std::cout << "Key 2 is not in the hash map." << std::endl;
        }
        std::cout << "the size of hash map is: " << hashmap.size() << std::endl;
        for (auto it = hashmap.begin(); it != hashmap.end(); ++it) {
            std::cout << "(" << it->first << "," << it->second << ") ";
        }
        std::cout << "are in the hash map." << std::endl;
        hashmap.clear();
        if (hashmap.empty()) {
            std::cout << "hash map is empty now!" << std::endl;
        }
    }
}  // namespace unordered_map_usage

int main() {}