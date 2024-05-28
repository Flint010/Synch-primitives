#include <iostream>

#include <thread>
#include <mutex>
#include <cassert>
#include "ConditionVariable.h"

int main() {

    int state = 0;
    std::mutex mutex;
    ConditionVariable condvar;

    //condvar.NotifyOne();
    //condvar.NotifyAll();

    auto wait_routine = [&]() {
        std::unique_lock<std::mutex> lock(mutex);
        assert(state == 0);
        condvar.Wait(mutex);
        assert(state == 1);
        };

    std::thread t(wait_routine);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    assert(state == 0);

    {
        std::unique_lock<std::mutex> lock(mutex);
        state = 1;
        condvar.NotifyOne();
    }

    t.join();

	return 0;
}