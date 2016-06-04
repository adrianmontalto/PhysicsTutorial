#pragma once
#pragma once
#include "glm\vec3.hpp"
#include <vector>

class PhysicsObject;

class CustomPhysicsScene
{
public:
	glm::vec3 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	CustomPhysicsScene();
	~CustomPhysicsScene();

	void addActor(PhysicsObject* object);
	void removeActor(PhysicsObject* object);
	void Update();
	void UpdateGizmos();
	void DebugScene();
	void AddGizmos();
	void CheckForInput();
};