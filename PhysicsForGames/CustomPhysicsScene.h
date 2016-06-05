#pragma once
#include "glm\vec3.hpp"
#include <vector>

class PhysicsObject;

class CustomPhysicsScene
{
public:
	glm::vec3 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_physicObjects;
	CustomPhysicsScene();
	~CustomPhysicsScene();

	void AddPhysicsObject(PhysicsObject* body);
	void RemovePhysicsObject(PhysicsObject* body);
	void Update();
	void UpdateGizmos();
	void DebugScene();
	void AddGizmos();
	void CheckForInput();
};

