#pragma once

class PhysicsObject;

typedef bool(*CollisionDetectionFunction)(PhysicsObject*,PhysicsObject*);

class Collision
{
public:
	Collision();
	~Collision();
	void CheckForCollision();
	bool Sphere2Sphere(PhysicsObject* obj1,PhysicsObject* obj2);
	bool Sphere2Plane(PhysicsObject* obj1,PhysicsObject* obj2);
};

