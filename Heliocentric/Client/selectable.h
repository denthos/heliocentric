#pragma once

class GUI;
class Client;

class Selectable {
public:
	virtual void select(GUI* gui, Client* client) {}
	virtual void unselect(GUI* gui, Client* client) {}
	virtual Selectable* getSelection() {
		return this;
	}
};