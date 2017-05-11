#pragma once

#include "mesh.h"
class QuadMesh : public Mesh {
public:
	QuadMesh();

	virtual void Draw(Shader &shader);
	void Draw();
private:
	void genMesh();
};