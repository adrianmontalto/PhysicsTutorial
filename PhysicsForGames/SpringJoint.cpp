#include "SpringJoint.h"
#include "RigidBody.h"
#include "Gizmos.h"
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
	glm::vec3 distanceBetweenConnectionsVector = m_connections[1]->GetPosition() - m_connections[0]->GetPosition();
	glm::vec3 normal = glm::normalize(distanceBetweenConnectionsVector);
	float distanceBetweenConnections = glm::length(distanceBetweenConnectionsVector);
	float displacement = distanceBetweenConnections - m_restLength;
	glm::vec3 force = normal * (m_springCoefficient * displacement);
	m_connections[0]->ApplyForceToActor(m_connections[1], force);
}

void SpringJoint::Debug()
{

}

void SpringJoint::MakeGizmo()
{
	Gizmos::addLine(m_connections[0]->GetPosition(), m_connections[1]->GetPosition(), glm::vec4(1, 0, 0, 1));
}