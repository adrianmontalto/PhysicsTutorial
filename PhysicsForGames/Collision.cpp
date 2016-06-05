#include "Collision.h"
#include "PhysicsObject.h"
#include "Sphere.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::CheckForCollision()
{

}

bool Collision::Sphere2Sphere(PhysicsObject* obj1,PhysicsObject* obj2)
{
	//try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	//if we are sucessful then test for collision
	if (sphere1 != NULL && sphere2 != NULL)
	{

	}
	return false;
}

bool Collision::Sphere2Plane(PhysicsObject* obj1,PhysicsObject* obj2)
{
	return false;
}
