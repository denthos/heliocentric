#ifndef GROUP_H_
#define GROUP_H_

#include "Node.h"

#include <list>

class Group : public Node {
public:
	~Group();
	void addChild(Node *);
	void removeChild(Node *);
	virtual void draw(GLuint, glm::mat4);
	virtual void update();
protected:
	std::list<Node *> children;
};

#endif