#include "CyclicBarrier.h"
#include <iostream>

CyclicBarrier::CyclicBarrier(size_t num)
	: numb_of_threads_(num), counter_(0), generation_(0)
{
}

void CyclicBarrier::Arrive()
{
	std::unique_lock<std::mutex> lock(mutex_);
	
	size_t gen = generation_;

	if (++counter_ >= numb_of_threads_ ) {
		counter_ = 0;
		generation_++;
		cv_.notify_all();
		return;
	}
	
	cv_.wait(lock, [&]() { return gen != generation_; });

	/*while (gen == generation_) {
		cv_.wait(lock);
	}*/

}
