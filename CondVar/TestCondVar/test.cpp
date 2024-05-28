#include "gtest/gtest.h"

#include "../ConditionVariable.h"
#include "../ConditionVariable.cpp"

#include <mutex>
#include <condition_variable>

TEST(CondVar, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


TEST(CondVar, NotifyOne) {
    std::mutex mutex;
    ConditionVariable condvar;

    bool pass{ false };

    auto wait_routine = [&]() {
        std::unique_lock<std::mutex> lock(mutex);

        while (!pass) {
            condvar.Wait(lock);
        }
    };

    std::thread t(wait_routine);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    {
        std::unique_lock<std::mutex> lock(mutex);
        pass = true;
        condvar.NotifyOne();
    }

    t.join();
}

TEST(CondVar, BlockingWait) {
    int state = 0;
    std::mutex mutex;
    ConditionVariable condvar;


    condvar.NotifyOne();
    condvar.NotifyAll();


    auto wait_routine = [&]() {
        std::unique_lock<std::mutex> lock(mutex);
        EXPECT_EQ(state, 0);
        condvar.Wait(lock);
        EXPECT_EQ(state, 1);
        };

    std::thread t(wait_routine);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(state, 0);

    {
        std::unique_lock<std::mutex> lock(mutex);
        state = 1;
        condvar.NotifyOne();
    }

    t.join();
}

TEST(CondVar, NotifyAll) {
    std::mutex mutex;
    ConditionVariable condvar;


    bool pass{ false };

    auto wait_routine = [&]() {
        std::unique_lock<std::mutex> lock(mutex);

        while (!pass) {
            condvar.Wait(lock);
        }
        };

    std::thread t1(wait_routine);
    std::thread t2(wait_routine);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(250));

    {
        std::unique_lock<std::mutex> lock(mutex);
        pass = true;
        condvar.NotifyAll();
    }

    t1.join();
    t2.join();
}

TEST(CondVar, NotifyManyTimes) {
    static const size_t kIterations = 1000 * 1000;

    ConditionVariable condvar;
    for (size_t i = 0; i < kIterations; ++i) {
        condvar.NotifyOne();
    }
}
