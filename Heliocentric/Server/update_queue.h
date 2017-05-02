#pragma once

#include <queue>
#include <memory>
#include <mutex>

template <typename TUpdate>
class UpdateQueue {
private:
	std::mutex queue_lock;
	std::queue<std::shared_ptr<TUpdate>> updates;

public:

	/**
	Get the queue of updates. The caller of this function is
	responsible for making sure the mutex is locked before reading
	or writing the queue.

	@return The queue of updates
	*/
	std::queue<std::shared_ptr<TUpdate>>& get_queue() {
		return this->updates;
	}

	/**
	Get the lock associated with the queue. The user of this
	UpdateQueue is responsible for using the mutex to lock
	the queue before accessing it.

	@return The mutex associated with this queue
	*/
	std::mutex& get_mutex() {
		return this->queue_lock;
	}
};