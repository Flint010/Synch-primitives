#pragma once

#include <condition_variable>
#include <atomic>
#include <mutex>


class CyclicBarrier
{
public:
	CyclicBarrier(size_t num);

	void Arrive();

private:
	size_t numb_of_threads_;
	size_t counter_;
	size_t generation_;
	std::condition_variable cv_;
	std::mutex mutex_;
};

