#include <algorithm>
#include <iostream>

class SortTracer {
  public:
    static long creations() { return n_created; }
    static long destructions() { return n_destroyed; }
    static long assignments() { return n_assigned; }
    static long comparisons() { return n_compared; }
    static long max_live() { return n_max_live; }

  public:
    SortTracer(int v = 0) : value(v), generation(1) {
        ++n_created;
        update_max_live();
        std::cerr << "SortTracer #" << n_created << ", created generation "
                  << generation << " (total: " << n_created - n_destroyed
                  << ")\n";
    }

    SortTracer(const SortTracer& rhs)
        : value(rhs.value), generation(rhs.generation + 1) {
        ++n_created;
        update_max_live();
        std::cerr << "SortTracer #" << n_created << ", copied as generation "
                  << generation << " (total: " << n_created - n_destroyed
                  << ")\n";
    }

    ~SortTracer() {
        ++n_destroyed;
        update_max_live();
        std::cerr << "SortTracer generation " << generation
                  << " destroyed (total: " << n_created - n_destroyed << ")\n";
    }

    SortTracer& operator=(const SortTracer& rhs) {
        ++n_assigned;
        std::cerr << "SortTracer assignment #" << n_assigned << " (generation "
                  << generation << " = " << rhs.generation << ")\n";
        value = rhs.value;
        return *this;
    }

    friend bool operator<(const SortTracer& lhs, const SortTracer& rhs) {
        ++n_compared;
        std::cerr << "SortTracer comparison #" << n_compared << " (generation "
                  << lhs.generation << " < " << rhs.generation << ")\n";
        return lhs.value < rhs.value;
    }

    int val() const { return value; }

  private:
    int value;                           // integer value to be sorted
    int generation;                      // generation of this tracer
    inline static long n_created = 0;    // number of constructor calls
    inline static long n_destroyed = 0;  // number of destructor calls
    inline static long n_assigned = 0;   // number of assignments
    inline static long n_compared = 0;   // number of comparisons
    inline static long n_max_live = 0;   // maximum of existing objects

    // recompute maximum of existing objects
    static void update_max_live() {
        if (n_created - n_destroyed > n_max_live) {
            n_max_live = n_created - n_destroyed;
        }
    }
};

int main() {
    SortTracer input[] = {7, 3, 5, 6, 4, 2, 0, 1, 9, 8};

    // 打印初始值
    for (int i = 0; i < 10; ++i) {
        std::cerr << input[i].val() << ' ';
    }
    std::cerr << '\n';

    // 记录初始条件
    long created_at_start = SortTracer::creations();
    long max_live_at_start = SortTracer::max_live();
    long assigned_at_start = SortTracer::assignments();
    long compared_at_start = SortTracer::comparisons();

    // 执行
    std::cerr << "---[ Start std::sort() ]--------------------\n";
    std::sort<>(&input[0], &input[9] + 1);
    std::cerr << "---[ End std::sort() ]----------------------\n";

    // 检查结果
    for (int i = 0; i < 10; ++i) {
        std::cerr << input[i].val() << ' ';
    }
    std::cerr << "\n\n";

    // final report
    std::cerr << "std::sort() of 10 SortTracer's was performed by:\n"
              << SortTracer::creations() - created_at_start
              << " temporary tracers\n"
              << "up to " << SortTracer::max_live()
              << " tracers at the same time (" << max_live_at_start
              << " before)\n"
              << SortTracer::assignments() - assigned_at_start
              << " assignments\n"
              << SortTracer::comparisons() - compared_at_start
              << " comparisons\n\n";
}