#pragma once
#include <array>

//function pointer array for doing our collisions
typedef bool(*CollisionDetectionFunction)(class PhysicsObject*,PhysicsObject*);

class Collision
{
public:
	class CustomPhysicsScene* m_customScene;
	Collision();
	Collision(CustomPhysicsScene* scene);
	~Collision();
	void CheckForCollision();
private:
	//function pointer array for doing our collisions
	static const std::array<CollisionDetectionFunction, 4> CollisionDetectionFunctions;
	//plane collisions
	static bool plane2Plane(PhysicsObject* planeObject1,PhysicsObject* planeObject2);
	static bool plane2Sphere(PhysicsObject* planeObject, PhysicsObject* sphereObject);

	//Sphere collisions
	static bool sphere2Plane(PhysicsObject* sphereObject, PhysicsObject* planeObject);
	static bool sphere2Sphere(PhysicsObject* sphereOnject1, PhysicsObject* sphereObject2);
};

