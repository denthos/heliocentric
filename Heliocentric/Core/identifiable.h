#pragma once

#include <atomic>

typedef unsigned int UID;

static std::atomic<UID> _uuid_counter = 0;

class Identifiable {
public:
	UID getID() { return this->_id; }
protected:
	Identifiable() { this->_id = _uuid_counter++; }
	Identifiable(UID id) : _id(id) {}
private:
	UID _id;
};