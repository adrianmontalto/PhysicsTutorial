#pragma once
#include "RigidBody.h"
class Sphere :public RigidBody
{
public:
	float m_radius;
	Sphere();
	Sphere(glm::vec3 position,glm::vec3 velocity,float mass , float radius,glm::vec4 colour,bool aStatic);
	~Sphere();
	void MakeGizmo();
};

