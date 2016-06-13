#pragma once
#include "PhysicsObject.h"
class SpringJoint :public PhysicsObject
{
private:
	class RigidBody* m_connections[2];
	float m_damping;
	float m_restLength;
	float m_springCoefficient;

	void virtual Update(glm::vec3 gravity,float timeStep);
	void virtual Debug();
	void virtual MakeGizmo();
public:
	SpringJoint();
	SpringJoint(class RigidBody* connection1,RigidBody* connection2,float springCoefficient,float damping);
	~SpringJoint();
};

