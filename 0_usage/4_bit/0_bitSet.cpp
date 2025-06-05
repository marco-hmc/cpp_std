#include <iostream>
#include <vector>

class BitSet {
  private:
    std::vector<bool> bits;

  public:
    BitSet(int size) { bits.resize(size); }

    void set(int index) { bits[index] = true; }

    void reset(int index) { bits[index] = false; }

    bool test(int index) { return bits[index]; }
};

int main() {
    BitSet bitSet(10);
    bitSet.set(2);
    bitSet.set(5);
    bitSet.reset(2);

    std::cout << bitSet.test(2) << '\n';  // Output: 0
    std::cout << bitSet.test(5) << '\n';  // Output: 1

    bool getFlag();
    void doFuncA();
    void doFuncB();
    const bool xxx = getFlag();
    xxx ? doFuncA() : doFuncB();
    return 0;
}