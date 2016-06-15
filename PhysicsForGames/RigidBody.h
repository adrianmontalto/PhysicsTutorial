
#pragma once
#include "PhysicsObject.h"
#include "glm\gtx\quaternion.hpp"

class RigidBody :public PhysicsObject
{
protected:
	
	glm::vec3 m_acceleration;//the acceleration of the object
	glm::quat m_rotation;//the rotation of the object
public:
	//default constructor
	RigidBody();
	//constructor that takes in a position,velocity,rotation and mass
	RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass,bool aStatic);
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
	void ApplyForceToActor(RigidBody* acrtor2,glm::vec3 force);

	//get the acceleration
	glm::vec3 GetAcceleration();
	//set the acceleration
	void AddAcceleration(glm::vec3 acceleration);
	//subtracts from the position
	void SubtractAcceleration(glm::vec3 acceleration);
	//gets the rotation
	glm::quat GetRotation();
	//sets the rotation
	void AddRotation(glm::quat rotation);
};

