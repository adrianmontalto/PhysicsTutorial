#pragma once
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

enum ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	NUMBERSHAPE = 3,
};

class PhysicsObject
{
public:
	ShapeType m_shapeID;
	glm::vec4 m_colour;
	virtual void Update(glm::vec3 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};
};