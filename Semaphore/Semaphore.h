#pragma once

#include <atomic>
#include <condition_variable>

class Semaphore
{
public:
	Semaphore(size_t initial_count);

	void Acquire();

	void Release();


	Semaphore(const Semaphore&) = delete;
	Semaphore(Semaphore&&) = delete;
	Semaphore& operator=(const Semaphore&) = delete;
	Semaphore& operator=(Semaphore&&) = delete;

private:
	size_t counter_;
	std::condition_variable cv_;
	std::mutex mutex_;
};

