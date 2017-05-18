#pragma once
#include <unordered_map>
#include <stdexcept>

template <typename TItem, typename TKey>
class KeyedSingleton {
public:
	class WTFKeyNotFoundAfterInsertionException : public std::exception {
	private:
		TKey key;

	public:
		TKey getKey() { return key;  }
		WTFKeyNotFoundAfterInsertionException(TKey key) : key(key) {}
	};

	template <typename ... TItemConstructorArgs>
	static TItem* getInstance(TKey key, TItemConstructorArgs... args) {
		static std::unordered_map<TKey, TItem> items;
		auto& item_it = items.find(key);

		if (item_it == items.end()) {
			items.insert(std::make_pair(key, TItem(key, args...)));
			item_it = items.find(key);

			if (item_it == items.end()) {
				throw WTFKeyNotFoundAfterInsertionException(key);
			}

		}

		return &item_it->second;
	}

};

template <typename TItem>
class Singleton {
public:
	static TItem* getInstance() {
		static TItem item;
		return &item;
	}
};