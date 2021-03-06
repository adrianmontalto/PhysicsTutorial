#include "Plane.h"
#include "Gizmos.h"
#include <limits>

Plane::Plane()
{
	m_velocity = glm::vec3(0);
	m_normal = glm::vec3(0,1,0);
	m_distance = 0.0f;
	m_colour = glm::vec4(1,1,1,1);
	m_shapeID = PLANE;
	m_mass = std::numeric_limits<float>::max();
}

Plane::Plane(glm::vec3 position, glm::vec3 normal, glm::vec4 colour, float distance,float size, bool aStatic)
{
	m_velocity = glm::vec3(0);
	m_normal = normal;
	m_distance = distance;
	m_colour = colour;
	m_shapeID = PLANE;
	m_mass = std::numeric_limits<float>::max();
	m_isStatic = aStatic;
	m_position = position;
	m_size = size;
}

Plane::~Plane()
{
}

void Plane::MakeGizmo()
{
	Gizmos::addAABBFilled(m_position,glm::vec3(m_size,0.0f,m_size),m_colour);
}

glm::vec3 Plane::GetNormal()
{
	return m_normal;
}

float Plane::GetDistance()
{
	return m_distance;
}