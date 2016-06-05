#include "Plane.h"
#include "Gizmos.h"

Plane::Plane()
{
}

Plane::Plane(glm::vec3 normal, glm::vec3 position , glm::vec4 colour, float distance)
{
	m_normal = normal;
	m_position = position;
	m_distance = distance;
	m_colour = colour;
	m_shapeID = PLANE;
}

Plane::~Plane()
{
}

void Plane::MakeGizmo()
{
	Gizmos::addAABBFilled(m_position, glm::vec3(100.0f, 0.0f, 100.0f), m_colour);
}