#include <iostream>

#include "CyclicBarrier.h"
#include  <cassert>


int main() {


    CyclicBarrier barrier{ 2 };

    int my = 0;
    int that = 0;

    auto that_routine = [&]() {
        that = 1;
        barrier.Arrive();
        assert(my == 1);
        barrier.Arrive();
        that = 2;
        barrier.Arrive();
        assert(my == 2);
    };

    std::thread that_thread(that_routine);

    my = 1;
    barrier.Arrive();
    assert(that == 1);
    barrier.Arrive();
    my = 2;
    barrier.Arrive();
    assert(that == 2);

    that_thread.join();


	return 0;
}