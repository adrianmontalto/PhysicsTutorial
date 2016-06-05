#pragma once
#include "RigidBody.h"
class Box :public RigidBody
{
public:
	Box();
	~Box();
	virtual void Update(glm::vec3 gravity, float timeStep);
	virtual void Debug();
	virtual void MakeGizmo();
	virtual void resetPosition();
};

