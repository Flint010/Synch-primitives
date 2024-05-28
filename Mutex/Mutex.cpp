#include "Mutex.h"

void my::Mutex::Lock()
{
	//at_flag_.wait(true);
	//at_flag_.test_and_set(std::memory_order_acquire);

	//while (at_flag_.test_and_set(std::memory_order_acquire))
	//{
	//	at_flag_.wait(true);
	//}



	while (flag_.exchange(true)) {
		flag_.wait(true);
	}

	// For flag_ std::atomic<bool>
	//flag_.wait(true);
	//flag_.store(true);
}

void my::Mutex::Unlock()
{
	//at_flag_.clear(std::memory_order_release);
	//at_flag_.notify_one();





	// For flag_ std::atomic<bool>
	flag_.store(false, std::memory_order_release);
	flag_.notify_one();
}

bool my::Mutex::TryLock()
{
	bool false_flag = false;
	return flag_.compare_exchange_strong(false_flag, true);
}
