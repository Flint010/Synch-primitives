#pragma once

#include <deque>

#include "Semaphore.h"

template<typename T>
class BufferedChannel
{
public:
	BufferedChannel(size_t capacity);

	void Send(T item);

	T Receive();

private:
	Semaphore ping_; // Makes sure that the buf_ is not overflow when Send data
	Semaphore pong_; // Makes sure that the buf_ is not empty when Receive data
	Semaphore mutex_;

	std::deque<T> buf_; // Guarded by mutex_
};

template<typename T>
inline BufferedChannel<T>::BufferedChannel(size_t capacity)
	: ping_(capacity), pong_(0), mutex_(1)
{
}

template<typename T>
inline void BufferedChannel<T>::Send(T item)
{
	ping_.Acquire();

	mutex_.Acquire();
	buf_.push_back(std::move(item));
	mutex_.Release();

	pong_.Release();
}

template<typename T>
inline T BufferedChannel<T>::Receive()
{
	pong_.Acquire();

	mutex_.Acquire();
	T front(std::move(buf_.front()));
	buf_.pop_front();
	mutex_.Release();

	ping_.Release();

	return front;
}
