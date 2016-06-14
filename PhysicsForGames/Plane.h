#pragma once
#include "PhysicsObject.h"
#include "glm\vec3.hpp"
class Plane :public PhysicsObject
{
private:
	glm::vec3 m_normal;//the normal of the plane
	glm::vec3 m_position;
	float m_distance;//the length and width of plane
	float m_size;
public:
	//default constructor
	Plane();
	//constructor that takes in the planes normal,colour and distance
	Plane(glm::vec3 position,glm::vec3 normal,glm::vec4 colour,float distance,float size,bool aStatic);
	//default destructor
	~Plane();
	//virtual update and debug
	void virtual Update(glm::vec3 gravity,float timestep) {};
	void virtual Debug() {};
	//makes the plane using gizmos
	void MakeGizmo();
	glm::vec3 GetNormal();
	float GetDistance();
};

