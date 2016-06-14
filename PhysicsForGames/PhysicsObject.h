#pragma once
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

enum ShapeType
{
	PLANE,
	SPHERE,
	AXISALIGNEDBOX,
	NUMBERSHAPE,
	JOINT,
};

class PhysicsObject
{
protected:
	ShapeType m_shapeID;
	glm::vec3 m_velocity;
	glm::vec4 m_colour;
	glm::vec3 m_position;//the objects position
	bool m_isStatic;
	float m_mass;
public:
	virtual void Update(glm::vec3 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};
	//returns the shape id
	ShapeType GetShapeID();
	//return the velocity
	glm::vec3 GetVelocity();
	void SetVelocity(glm::vec3 velocity) { m_velocity = velocity; };
	//adds to velocity
	void AddVelocity(glm::vec3 velocity);
	//return the colour
	glm::vec4 GetColour();
	//returns the static value
	bool GetStatic();
	//returns the mass of the object
	float GetMass();
	//get the position
	glm::vec3 GetPosition();
	//adds to the position
	void AddPosition(glm::vec3 position);
	//subtracts from the position
	void SubtractPositiion(glm::vec3 position);
};