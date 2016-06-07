#include "RigidBody.h"
RigidBody::RigidBody()
{
	m_position = glm::vec3(0,0,0);
	m_velocity = glm::vec3(0,0,0);
	m_rotation = glm::quat();
	m_mass = 1.0f;
}

RigidBody::RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_acceleration = glm::vec3(0);
	m_mass = mass;
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update(glm::vec3 gravity, float timeStep)
{
	m_velocity += m_acceleration * timeStep;
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;
	m_acceleration = glm::vec3(0);
}

void RigidBody::Debug()
{

}

void RigidBody::ApplyForce(glm::vec3 force)
{
	if (force != glm::vec3(0) && m_mass != 0)
	{
		m_acceleration = force / m_mass;
	}
}

void RigidBody::ApplyForceToActor(RigidBody* acrtor2, glm::vec3 force)
{
	if (force != glm::vec3(0))
	{
		if (m_mass != 0)
		{
			m_acceleration -= force / m_mass;
		}
		
		if (acrtor2->m_mass != 0)
		{
			acrtor2->m_acceleration += force / acrtor2->m_mass;
		}		
	}

}