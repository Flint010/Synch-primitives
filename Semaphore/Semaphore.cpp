#include "Semaphore.h"

Semaphore::Semaphore(size_t initial_count)
	:counter_(initial_count)
{
}

void Semaphore::Acquire()
{
	std::unique_lock<std::mutex> lock(mutex_);
	
	if (counter_ == 0)
		cv_.wait(lock, [&]() { return counter_ != 0; });
	//else
	--counter_;
}

void Semaphore::Release()
{
	std::unique_lock<std::mutex> lock(mutex_);
	++counter_;
	cv_.notify_one();
}
