#include "Sphere.h"
#include "Gizmos.h"

Sphere::Sphere()
{
	m_position = glm::vec3(0);
	m_velocity = glm::vec3(0);
	m_mass = 0.0f;
	m_radius = 1.0f;
	m_colour = glm::vec4(0);
	m_shapeID = SPHERE;
}

Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_radius = radius;
	m_colour = colour;
	m_shapeID = SPHERE;
}

Sphere::~Sphere()
{
}

void Sphere::MakeGizmo()
{
	Gizmos::addSphereFilled(m_position,m_radius,10,10,m_colour);
}

