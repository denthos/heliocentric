#pragma once

#include <atomic>

#define INVALID_ID 0

typedef unsigned int UID;

static std::atomic<UID> _uuid_counter = 1;

class Identifiable {
public:
	UID getID() { return this->_id; }
protected:
	Identifiable() { this->_id = _uuid_counter++; }
	Identifiable(UID id) : _id(id) {}
private:
	UID _id;
};