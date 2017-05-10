#pragma once

#include <mutex>

namespace Lib {

	template <typename T>
	class Key;

	/**
	A class used to protect an item behind a lock. When using this class, the user must
	access the locked item via a Key object, which uses RAII for protection.

	To protect a type behind a lock:
	Lib::Lock<type> item;

	To access the underlying item:
	Key<type>& key = Lib::key_acquire(item);
	key.get().do_something();

	The lock will automatically lock when calling `key_acquire` and will automatically
	unlock when the key goes out of scope.
	*/
	template <typename T>
	class Lock {

		/* 
		We should only be able to access the underlying item and lock via
		a key object.
		*/
		friend class Key<T>;

	private:
		std::mutex lock;
		T item;
	};

	/**
	The object which allows access to a Locked item. Upon construction, the
	lock is locked. Upon destruction, the lock is unlocked

	The underlying item can be retrieved with get()
	*/
	template <typename T>
	class Key {

		/* We can only construct a key with `key_acquire`, which allows type-deduction */
		template <typename T>
		friend Key<T> key_acquire(Lock<T>&);

	private:
		Lock<T>& lock;

		Key(Lock<T>& lock) : lock(lock) {
			lock.lock.lock();
		}

	public:

		Key(const Key&) = delete;
		Key(Key&&) = default;

		~Key() {
			lock.lock.unlock();
		}

		T& get() {
			return lock.item;
		}
	};

	/**
	Proxy function used to construct a key. This allows for type deduction since
	C++ constructors can't type deduct
	*/
	template <typename T>
	Key<T> key_acquire(Lock<T>& lock) {
		return Key<T>(lock);
	}
}