#include "gtest/gtest.h"
#include "../CyclicBarrier.h"
#include "../CyclicBarrier.cpp"


TEST(Barrier, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

//TEST(Barrier, OneThread) {
//    CyclicBarrier barrier{ 1 };
//
//    for (size_t i = 0; i < 10; ++i) {
//        barrier.Arrive();
//    }
//}

TEST(Barrier, TwoThreads) {
    CyclicBarrier barrier{ 2 };

    int my = 0;
    int that = 0;

    auto that_routine = [&]() {
        that = 1;
        barrier.Arrive();
        EXPECT_EQ(my, 1);
        barrier.Arrive();
        that = 2;
        barrier.Arrive();
        EXPECT_EQ(my, 2);
        };

    std::thread that_thread(that_routine);

    my = 1;
    barrier.Arrive();
    EXPECT_EQ(that, 1);
    barrier.Arrive();
    my = 2;
    barrier.Arrive();
    EXPECT_EQ(that, 2);

    that_thread.join();
}

TEST(Barrier, Runners) {
    static const size_t kThreads = 10;
    CyclicBarrier barrier{ kThreads };
    

    static const size_t kIterations = 280;

    auto runner_routine = [&barrier]() {
        for (size_t i = 0; i < kIterations; ++i) {
            barrier.Arrive();
        }
    };

    std::vector<std::thread> runners;

    for (size_t i = 0; i < kThreads; ++i) {
        runners.emplace_back(runner_routine);
    }

    for (auto& runner : runners) {
        runner.join();
    }
}
