#pragma once

#include <atomic>


class ConditionVariable
{
public:
	ConditionVariable();
	ConditionVariable(const ConditionVariable&) = delete;
	ConditionVariable& operator=(const ConditionVariable&) = delete;
	ConditionVariable(ConditionVariable&&) = delete;
	ConditionVariable& operator=(ConditionVariable&&) = delete;

	template<typename Mutex>
	void Wait(Mutex& mutex);

	void NotifyOne();
	void NotifyAll();


private:
	std::atomic<bool> waiter_;
};

template<typename Mutex>
inline void ConditionVariable::Wait(Mutex& mutex)
{
	waiter_.store(false);
	mutex.unlock();
	waiter_.wait(false);
	mutex.lock();
}
