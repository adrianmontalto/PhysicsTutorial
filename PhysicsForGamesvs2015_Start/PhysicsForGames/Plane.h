#pragma once
#include "PhysicsObject.h"
class Plane :public PhysicsObject
{
public:
	glm::vec3 m_normal;
	glm::vec3 m_position;
	float m_distance;
	Plane();
	Plane(glm::vec3 normal,glm::vec3 position,glm::vec4 colour, float distance);
	~Plane();
	void virtual Update(glm::vec3 gravity,float timeStep) {};
	void virtual Debug() {};
	void virtual MakeGizmo();
};

