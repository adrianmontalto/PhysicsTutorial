#include "PhysicsObject.h"
#include <limits>
ShapeType PhysicsObject::GetShapeID() 
{
	return m_shapeID; 
}

glm::vec3 PhysicsObject::GetVelocity() 
{ 
	if (m_isStatic == false)
	{
		return m_velocity;
	}
	else
		return glm::vec3(0);
}

void PhysicsObject::AddVelocity(glm::vec3 velocity) 
{ 
	if (m_isStatic == false)
	{
		m_velocity += velocity;
	}	 
}

glm::vec4 PhysicsObject::GetColour() 
{ 
	return m_colour; 
}

bool PhysicsObject::GetStatic() 
{ 
	return m_isStatic; 
}

float PhysicsObject::GetMass() 
{ 
	if (m_isStatic == false)
	{
		return m_mass;
	}
	else
		return std::numeric_limits<float>::max();
}

glm::vec3 PhysicsObject::GetPosition()
{
	return m_position;
}

void PhysicsObject::AddPosition(glm::vec3 position)
{
	if (m_isStatic == false)
	{
		m_position += position;
	}	
}

void PhysicsObject::SubtractPositiion(glm::vec3 position)
{
	if (m_isStatic == false)
	{
		m_position -= position;
	}	
}