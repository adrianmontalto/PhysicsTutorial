#pragma once
#include <array>
#include "glm\vec3.hpp"
class PhysicsObject;

typedef bool(*CollisionDetectionFunction)(PhysicsObject*,PhysicsObject*);

class Collision
{
public:
	class CustomPhysicsScene* m_customScene;
	Collision();
	Collision(CustomPhysicsScene* scene);
	~Collision();
	void CheckForCustomCollision();
	static void Seperate(PhysicsObject* object1,PhysicsObject* object2,float overlap,glm::vec3 normal);
	static void Response(PhysicsObject* object1, PhysicsObject* object2, float overlap, glm::vec3 normal);
	
private:
	static const std::array<CollisionDetectionFunction, 9> CollisionFunctions;

	//plane Collisions
	static bool Plane2Sphere(PhysicsObject* planeObject, PhysicsObject* SphereObject);
	static bool Plane2AABB(PhysicsObject* planeObject,PhysicsObject* AABBObject);
	static bool Plane2Plane(PhysicsObject* planeObject1, PhysicsObject* planeObject2);

	//sphere collisions
	static bool Sphere2Plane(PhysicsObject* sphereObject, PhysicsObject* planeObject);
	static bool Sphere2AABB(PhysicsObject* sphereObject, PhysicsObject* AABBObject);
	static bool Sphere2Sphere(PhysicsObject* sphereObject1,PhysicsObject* sphereObject2);

	//AABB Colisions
	static bool AABB2Plane(PhysicsObject* AABBObject, PhysicsObject* planeObject);
	static bool AABB2Sphere(PhysicsObject* AABBObject,PhysicsObject* sphereObject);
	static bool AABB2AABB(PhysicsObject* AABBObject1,PhysicsObject* AABBObject2);
};

