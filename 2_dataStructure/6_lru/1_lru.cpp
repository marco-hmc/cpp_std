#include <iostream>
#include <list>
#include <unordered_map>

class LRUCache {
  private:
    int capacity;
    std::list<int> cacheList;
    std::unordered_map<int, std::list<int>::iterator> cacheMap;

  public:
    explicit LRUCache(int capacity) : capacity(capacity) {}

    int get(int key) {
        if (cacheMap.find(key) == cacheMap.end()) {
            return -1;
        }

        cacheList.splice(cacheList.begin(), cacheList, cacheMap[key]);

        return *cacheMap[key];
    }

    void put(int key, int value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            cacheList.splice(cacheList.begin(), cacheList, cacheMap[key]);
            *cacheMap[key] = value;
        } else {
            if (cacheList.size() == capacity) {
                int leastRecent = cacheList.back();
                cacheList.pop_back();
                cacheMap.erase(leastRecent);
            }

            cacheList.push_front(value);
            cacheMap[key] = cacheList.begin();
        }
    }
};

void test() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    std::cout << cache.get(1) << '\n';
    cache.put(3, 3);
    std::cout << cache.get(2) << '\n';
    cache.put(4, 4);
    std::cout << cache.get(1) << '\n';
    std::cout << cache.get(3) << '\n';
    std::cout << cache.get(4) << '\n';
}

int main() {
    test();
    return 0;
}
