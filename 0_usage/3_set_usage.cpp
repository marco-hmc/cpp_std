#include <iostream>
#include <unordered_set>

int main() {
    std::unordered_set<int> hashSet;
    hashSet.insert(3);
    hashSet.insert(2);
    hashSet.insert(1);
    hashSet.erase(2);
    if (!hashSet.contains(2)) {
        std::cout << "Key 2 is not in the hash set." << std::endl;
    }

    std::cout << "The size of hash set is: " << hashSet.size() << std::endl;
    for (auto it = hashSet.begin(); it != hashSet.end(); ++it) {
        std::cout << (*it) << " ";
    }

    std::cout << "are in the hash set." << std::endl;
    hashSet.clear();
    if (hashSet.empty()) {
        std::cout << "hash set is empty now!" << std::endl;
    }
}