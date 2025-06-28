#include <iostream>
#include <unordered_map>

class TrieNode {
  public:
    std::unordered_map<char, TrieNode *> children;
    bool isEndOfWord{false};

    TrieNode() = default;
};

class Trie {
  private:
    TrieNode *root;

  public:
    Trie() : root(new TrieNode()) {}

    void insert(const std::string &word) {
        TrieNode *current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    bool search(const std::string &word) {
        TrieNode *current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        return current->isEndOfWord;
    }

    bool startsWith(const std::string &prefix) {
        TrieNode *current = root;
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        return true;
    }
};

void test() {
    Trie trie;
    trie.insert("apple");
    trie.insert("banana");
    trie.insert("orange");

    std::cout << "Search 'apple': "
              << (trie.search("apple") ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 'banana': "
              << (trie.search("banana") ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 'orange': "
              << (trie.search("orange") ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 'grape': "
              << (trie.search("grape") ? "Found" : "Not Found") << std::endl;

    std::cout << "Starts with 'app': "
              << (trie.startsWith("app") ? "Yes" : "No") << std::endl;
    std::cout << "Starts with 'ban': "
              << (trie.startsWith("ban") ? "Yes" : "No") << std::endl;
    std::cout << "Starts with 'ora': "
              << (trie.startsWith("ora") ? "Yes" : "No") << std::endl;
    std::cout << "Starts with 'gr': " << (trie.startsWith("gr") ? "Yes" : "No")
              << std::endl;
}

int main() {
    test();
    return 0;
}