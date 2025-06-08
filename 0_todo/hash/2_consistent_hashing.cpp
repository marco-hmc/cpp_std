#include <functional>
#include <iostream>
#include <map>
#include <string>

using HashFunction = std::function<size_t(const std::string &)>;

class ConsistentHashing {
  public:
    ConsistentHashing(const HashFunction &hashFunction)
        : hashFunction(hashFunction) {}

    void addNode(const std::string &node) {
        size_t hash = hashFunction(node);
        ring[hash] = node;
    }

    void removeNode(const std::string &node) {
        size_t hash = hashFunction(node);
        ring.erase(hash);
    }

    std::string getNode(const std::string &key) {
        size_t hash = hashFunction(key);
        auto it = ring.lower_bound(hash);
        if (it == ring.end()) {
            it = ring.begin();
        }
        return it->second;
    }

  private:
    std::map<size_t, std::string> ring;
    HashFunction hashFunction;
};

int main() {
    HashFunction hashFunction = std::hash<std::string>();

    ConsistentHashing ch(hashFunction);

    ch.addNode("Node1");
    ch.addNode("Node2");
    ch.addNode("Node3");

    std::string key = "Key1";
    std::string node = ch.getNode(key);
    std::cout << "Node responsible for key " << key << ": " << node << '\n';

    return 0;
}