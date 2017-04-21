#pragma once
template <class T>
class Upgradable {
public:
	Upgradable<T>();
	void apply_upgrade(T);

protected:
	int tier;
	int MAX_TIER;
};