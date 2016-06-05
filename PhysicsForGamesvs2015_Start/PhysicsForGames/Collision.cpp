#include "Collision.h"
#include "CustomPhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"

const std::array<CollisionDetectionFunction,4> Collision::CollisionDetectionFunctions
{
	plane2Plane,		plane2Sphere,
	sphere2Plane,		sphere2Sphere,
};

Collision::Collision()
{
}

Collision::Collision(CustomPhysicsScene* scene)
{
	m_customScene = scene;
}

Collision::~Collision()
{
}

void Collision::CheckForCollision()
{
	int actorCount = m_customScene->m_actors.size();
	//need to check for collisions against all objects except this one.

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_customScene->m_actors[outer];
			PhysicsObject* object2 = m_customScene->m_actors[inner];
			int _shapeID1 = object1->m_shapeID;
			int _shapeID2 = object2->m_shapeID;

			int functionIndex = (_shapeID1 * NUMBERSHAPE) + _shapeID2;
			CollisionDetectionFunction collisionFunctionPtr = CollisionDetectionFunctions[functionIndex];
			if (collisionFunctionPtr != NULL)
			{
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool Collision::sphere2Sphere(PhysicsObject* sphereOnject1, PhysicsObject* sphereObject2)
{
	//try to cast objects to sphere and sphere
	Sphere *sphere1 = dynamic_cast<Sphere*>(sphereOnject1);
	Sphere *sphere2 = dynamic_cast<Sphere*>(sphereObject2);

	//if we are successful then test for collision
	if (sphere1 != NULL && sphere2 != NULL)
	{

	}
	return false;
}