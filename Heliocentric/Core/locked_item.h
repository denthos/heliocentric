#pragma once

#include <mutex>

namespace Lib {
	template <typename TProtectedItem>
	class LockedItem {
	private:
		std::mutex lock;
		TProtectedItem item;

	public:
		/**
		Get a reference to the protected item. The user is responsible for locking
		and unlocking the lock before messing with the item.

		@return The protected item.
		*/
		TProtectedItem& get_item() {
			return this->item;
		}

		/**
		Get a reference to the lock. The user is responsible for locking/unlocking this
		before messing with the item

		@return The lock
		*/
		std::mutex& get_lock() {
			return this->lock;
		}
	};
}