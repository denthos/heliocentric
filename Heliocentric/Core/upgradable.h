#pragma once
template <class T>
class Upgradable {
public:
	Upgradable<T>() : MAX_TIER(3) {};
	Upgradable<T>(int max) : MAX_TIER(max) {};
	void apply_upgrade(T);

protected:
	int MAX_TIER;
};