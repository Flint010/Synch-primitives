#include <iostream>

#include <chrono>

#include "Mutex.h"


int main() {

    my::Mutex mutex;

    std::thread sleeper([&]() {
        mutex.Lock();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        mutex.Unlock();
        });

    std::thread waiter([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        const auto start = std::chrono::system_clock::now();
        mutex.Lock();
        mutex.Unlock();
        const auto finish = std::chrono::system_clock::now();
        const auto running_time = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
        std::cout << "Lock/Unlock cpu time in sleeper thread: " << running_time.count() << " seconds\n";
        //EXPECT_TRUE(running_time.count() < 0.1);
        });

    sleeper.join();
    waiter.join();

	return 0;
}