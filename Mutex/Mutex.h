#pragma once

#include <atomic>

namespace my {

class Mutex
{
public:
	void Lock();
	void Unlock();
	
	bool TryLock();

private:
	std::atomic<bool> flag_ = false;
	//std::atomic_flag at_flag_ = ATOMIC_FLAG_INIT;
};

} // namespace my


