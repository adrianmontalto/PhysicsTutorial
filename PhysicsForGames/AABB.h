#pragma once
#include "RigidBody.h"
class AABB :public RigidBody
{
private:
	glm::vec3 m_extents;
	glm::vec4 m_colour;
public:
	AABB();
	AABB(glm::vec3 position,glm::vec3 extents,glm::vec4 colour,float mass,bool isStatic);
	~AABB();
	void MakeGizmo();
	glm::vec3 GetExtent();
};

