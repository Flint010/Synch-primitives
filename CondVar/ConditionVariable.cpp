#include "ConditionVariable.h"

ConditionVariable::ConditionVariable()
	: waiter_(false)
{
}

void ConditionVariable::NotifyOne()
{
	waiter_.store(true);
	waiter_.notify_one();
}

void ConditionVariable::NotifyAll()
{
	waiter_.store(true);
	waiter_.notify_all();
}
