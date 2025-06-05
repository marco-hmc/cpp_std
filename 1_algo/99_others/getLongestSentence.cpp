#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>

std::string filter_ws(const std::string &s) {
    const char *ws{" \r\n\t"};
    const auto a(s.find_first_not_of(ws));
    const auto b(s.find_last_not_of(ws));
    if (a == std::string::npos) {
        return {};
    }
    return s.substr(a, b);
}

std::multimap<size_t, std::string> get_sentence_stats(
    const std::string &content) {
    std::multimap<size_t, std::string> ret;
    const auto end_it(std::end(content));
    auto it1(std::begin(content));
    auto it2(std::find(it1, end_it, '.'));

    while (it1 != end_it && std::distance(it1, it2) > 0) {
        std::string s{filter_ws({it1, it2})};

        if (s.length() > 0) {
            const auto words(std::count(std::begin(s), std::end(s), ' ') + 1);
            ret.emplace(std::make_pair(words, std::move(s)));
        }

        it1 = std::next(it2, 1);
        it2 = std::find(it1, end_it, '.');
    }

    return ret;
}

int main() {
    std::cin.unsetf(std::ios::skipws);
    std::string content{std::istream_iterator<char>{std::cin}, {}};

    for (const auto &[word_count, sentence] : get_sentence_stats(content)) {
        std::cout << word_count << " words: " << sentence << ".\n";
    }
}