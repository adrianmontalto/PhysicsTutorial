#include "SpringJoint.h"
#include "RigidBody.h"
//#include "glm\glm.hpp"

SpringJoint::SpringJoint()
{
}

SpringJoint::SpringJoint(class RigidBody* connection1, RigidBody* connection2, 
					     float springCoefficient, float damping)
{
	m_connections[0] = connection1;
	m_connections[1] = connection2;
	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_restLength = glm::length(m_connections[0]->GetPosition() - m_connections[1]->GetPosition());
	m_shapeID = JOINT;
}

SpringJoint::~SpringJoint()
{
}

void SpringJoint::Update(glm::vec3 gravity, float timeStep)
{

}

void SpringJoint::Debug()
{

}

void SpringJoint::MakeGizmo()
{

}