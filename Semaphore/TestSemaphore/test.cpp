#include "gtest/gtest.h"

#include "Semaphore.h"
#include "BufferedChannel.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(Semaphore, NonBlocking) {
    Semaphore semaphore(2);

    semaphore.Acquire();  // -1
    semaphore.Release();  // +1

    semaphore.Acquire();  // -1
    semaphore.Acquire();  // -1
    semaphore.Release();  // +1
    semaphore.Release();  // +1
}

TEST(Semaphore, Blocking) {
    Semaphore semaphore(0);

    bool touched = false;

    auto touch_routine = [&touched, &semaphore]() {
        semaphore.Acquire();
        touched = true;
        };

    std::thread toucher(touch_routine);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(250));

    EXPECT_FALSE(touched);

    semaphore.Release();
    toucher.join();

    EXPECT_TRUE(touched);
}

TEST(Semaphore, PingPong) {
    Semaphore my{ 1 };
    Semaphore that{ 0 };

    int step = 0;

    auto opponent_routine = [&]() {
        that.Acquire();
        EXPECT_EQ(step, 1);
        step = 0;
        my.Release();
        };

    std::thread opponent(opponent_routine);

    my.Acquire();
    EXPECT_EQ(step, 0);
    step = 1;
    that.Release();

    my.Acquire();
    EXPECT_TRUE(step == 0);

    opponent.join();
}

TEST(Channel, SendThenReceive) {
    BufferedChannel<int> chan{ 1 };
    chan.Send(42);
    EXPECT_EQ(chan.Receive(), 42);
}

TEST(Channel, MoveIt) {
    BufferedChannel<std::unique_ptr<std::string>> channel{ 1 };

    channel.Send(
        std::make_unique<std::string>("Move it"));

    auto message = channel.Receive();
    ASSERT_EQ(*message, "Move it");
}

TEST(Channel, Buffer) {
    BufferedChannel<std::string> channel{ 2 };

    channel.Send("hello");
    channel.Send("world");

    EXPECT_EQ(channel.Receive(), "hello");
    EXPECT_EQ(channel.Receive(), "world");
}

TEST(Channel, FifoSmall) {
    BufferedChannel<std::string> chan{ 1 };

    std::thread producer(
        [&chan]() {
            chan.Send("hello");
            chan.Send("world");
            chan.Send("!");
        });


    EXPECT_EQ(chan.Receive(), "hello");
    EXPECT_EQ(chan.Receive(), "world");
    EXPECT_EQ(chan.Receive(), "!");


  

    producer.join();
}

TEST(Channel, Fifo) {
    BufferedChannel<int> chan{ 3 };

    static const int kItems = 1024;

    auto producer_routine = [&]() {
        for (int i = 0; i < kItems; ++i) {
            chan.Send(i);
        }
        chan.Send(-1);  // Poison pill
        };

    std::thread producer(producer_routine);

    // Consumer

    for (int i = 0; i < kItems; ++i) {
        EXPECT_EQ(chan.Receive(), i);
    }
    EXPECT_EQ(chan.Receive(), -1);

    producer.join();
}

TEST(Channel, Capacity) {
    BufferedChannel<int> chan{ 3 };
    std::atomic<size_t> send_count{ 0 };

    auto producer_routine = [&]() {
        for (size_t i = 0; i < 100; ++i) {
            chan.Send(i);
            send_count.store(i);
        }
        chan.Send(-1);
        };

    std::thread producer(producer_routine);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    ASSERT_TRUE(send_count.load() <= 3);

    for (size_t i = 0; i < 14; ++i) {
        (void)chan.Receive();
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    ASSERT_TRUE(send_count.load() <= 17);

    while (chan.Receive() != -1) {
        // Pass
    }

    producer.join();
}

TEST(Channel, Pill) {
    static const size_t kThreads = 10;
    BufferedChannel<int> chan{ 1 };

    auto routine = [&]() {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(std::rand() % 999 + 1));

        ASSERT_EQ(chan.Receive(), -1);
        chan.Send(-1);
        };

    std::vector<std::thread> executors;
    for (size_t i = 0; i < kThreads; ++i) {
        executors.emplace_back(routine);
    }

    chan.Send(-1);

    for (auto& t : executors) {
        t.join();
    }

}
