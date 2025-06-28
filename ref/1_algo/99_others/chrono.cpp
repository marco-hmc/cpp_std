
#include <chrono>
#include <iostream>

int main() {
    std::chrono::system_clock::time_point now =
        std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);

    std::cout << "当前时间戳: " << timestamp << '\n';
    std::chrono::system_clock::time_point timepoint =
        std::chrono::system_clock::from_time_t(timestamp);

    std::string timeString = std::ctime(&timestamp);

    std::cout << "当前日期和时间: " << timeString << '\n';

    std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();

    for (int i = 0; i < 100000000; i++) {
        // do something
    }

    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();

    std::chrono::duration<double> duration =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "程序运行时间: " << duration.count() << " 秒" << '\n';

    return 0;
}
