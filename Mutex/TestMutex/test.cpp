#include "gtest/gtest.h"

#include "../Mutex.h"
#include "../Mutex.cpp"

#include <thread>
#include <chrono>


TEST(Mutex, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(Mutex, LockUnlock) {
    my::Mutex mutex;

    mutex.Lock();
    mutex.Unlock();
}

TEST(Mutex, SequentialLockUnlock) {
    my::Mutex mutex;
    mutex.Lock();
    mutex.Unlock();
    mutex.Lock();
    mutex.Unlock();
}

TEST(Mutex, SharedMemoryValue) {
    // “естируем использование общей €чейки пам€ти на разные инстансы локов
    my::Mutex mutex;
    mutex.Lock();

    my::Mutex mutex2;
    mutex2.Lock();  // тут не должно блокироватьс€
    mutex2.Unlock();

    mutex.Unlock();
}

TEST(Mutex, TryLock) {
    my::Mutex spinlock;

    ASSERT_TRUE(spinlock.TryLock());
    ASSERT_FALSE(spinlock.TryLock());
    spinlock.Unlock();
    ASSERT_TRUE(spinlock.TryLock());
    spinlock.Unlock();
    spinlock.Lock();
    ASSERT_FALSE(spinlock.TryLock());
}

TEST(Mutex, ConcurrentLock) {
    my::Mutex mutex;

    volatile int counter = 0;

    auto routine = [&mutex, &counter]() {
        mutex.Lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
        mutex.Unlock();
        };

    std::thread t1(routine);
    std::thread t2(routine);
    std::thread t3(routine);
    std::thread t4(routine);
    std::thread t5(routine);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    ASSERT_EQ(5, counter);
}


TEST(Mutex, Blocking) {
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
        EXPECT_TRUE(running_time.count() < 3);
        });

    sleeper.join();
    waiter.join();
}
//
//TEST(Mutex, NoContentionNoFutexes) {
//    static const size_t kNoContentionIterations = 1000;
//
//    size_t futex_calls = std::thread::FutexCallCount();
//
//    my::Mutex mutex;
//    for (size_t i = 0; i < kNoContentionIterations; ++i) {
//        mutex.Lock();
//        mutex.Unlock();
//    }
//
//    // no calls to futex
//    EXPECT_EQ(futex_calls, twist::thread::FutexCallCount());
//}

//TEST(Mutex, NoContentionNoFutexesWithWarmup) {
//    solutions::Mutex mutex;
//
//    static const size_t kWarmupIterations = 100000;
//
//    auto contender = [&]() {
//        for (size_t i = 0; i < kWarmupIterations; ++i) {
//            mutex.Lock();
//            mutex.Unlock();
//        }
//        };
//
//    twist::strand::thread t1(contender);
//    twist::strand::thread t2(contender);
//    t1.join();
//    t2.join();
//
//    size_t futex_calls = twist::thread::FutexCallCount();
//
//    const size_t kNoContentionIterations = 1000;
//
//    for (size_t i = 0; i < kNoContentionIterations; ++i) {
//        mutex.Lock();
//        mutex.Unlock();
//    }
//
//    ASSERT_EQ(futex_calls, twist::thread::FutexCallCount());
//}

