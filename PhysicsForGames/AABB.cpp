#include "AABB.h"
#include "Gizmos.h"

AABB::AABB()
{
}

AABB::AABB(glm::vec3 position, glm::vec3 extents, glm::vec4 colour)
{
	m_position = position;
	m_extents = extents;
	m_colour = colour;
	m_shapeID = AXISALIGNEDBOX;
}

AABB::~AABB()
{
}

void AABB::MakeGizmo()
{
	Gizmos::addAABBFilled(m_position, m_extents, m_colour);
}

glm::vec3 AABB::GetExtent()
{
	return m_extents;
}