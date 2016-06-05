#pragma once
#include "PhysicsObject.h"
#include "glm\gtx\quaternion.hpp"

class RigidBody :public PhysicsObject
{
public:
	glm::vec3 m_position;//the rigid bodies position
	glm::vec3 m_velocity;//the velocity of the rigid body
	glm::vec3 m_acceleration;//the acceleration of the rigidbody

	float m_mass;//the mass of the object
	glm::quat m_rotation;//the rotation of the object

	//default constructor
	RigidBody();
	//constructor that takes in a position,velocity,rotation and mass
	RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass);
	//default destructor
	~RigidBody();

	//function to make the object
	virtual void MakeGizmo() = 0;
	//update the object using gravity and timestep
	void Update(glm::vec3 gravity, float timeStep);
	//used to debug scene
	void Debug();
	//applies force to object
	void ApplyForce(glm::vec3 force);
	//applies force to actor
	void ApplyForceToActor(RigidBody* acrtor2, glm::vec3 force);
};

