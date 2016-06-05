#include "Plane.h"
#include "Gizmos.h"

Plane::Plane()
{
	m_normal = glm::vec3(0,1,0);
	m_distance = 0.0f;
	m_colour = glm::vec4(1,1,1,1);
	m_shapeID = PLANE;
}

Plane::Plane(glm::vec3 normal,glm::vec4 colour, float distance)
{
	m_normal = normal;
	m_distance = distance;
	m_colour = colour;
	m_shapeID = PLANE;
}

Plane::~Plane()
{
}

void Plane::MakeGizmo()
{
	Gizmos::addAABBFilled(m_normal,glm::vec3(m_distance,0.0f,m_distance),m_colour);
}
