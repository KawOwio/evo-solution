#pragma once
#ifndef _SAFEQUEUE_H_

#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>

template<typename dataType>
class SafeQueue
{
private:
	std::queue<dataType> m_queue;	// Queue
	std::mutex m_mtx;	// Mutex
	std::condition_variable m_cv;	// Conditional variable

public:
	// Add a new element to the queue
	void push(dataType& push_data)
	{
		std::unique_lock<std::mutex> lock(m_mtx);	// lock threads
		m_queue.push(push_data);	// push data
		lock.unlock();	// unlock threads
		m_cv.notify_one();	// notify one waiting thread
	}

	// Remove an element from the queue
	bool pop(dataType& pop_value)
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_cv.wait_for(lock, std::chrono::seconds(1));	// wait for other thread to finish
		if (m_queue.empty())	// if it hasn't finished in a second and the queue is empty then return false
		{
			return false;
		}
		pop_value = m_queue.front();
		m_queue.pop();
		return true;
	}

	// Check if the queue is empty
	bool isEmpty()
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		return m_queue.empty();
	}

	// Return the size of the queue
	int size()
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		return m_queue.size();
	}
};

#endif // !_SAFEQUEUE_H_