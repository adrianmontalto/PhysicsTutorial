#include "Collision.h"
#include "CustomPhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"

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
	int objectCount = m_customScene->m_physicObjects.size();
	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer < objectCount - 1; ++outer)
	{
		for (int inner = outer + 1; inner < objectCount; ++inner)
		{
			//sets the objects to the outer and inner object
			PhysicsObject* object1 = m_customScene->m_physicObjects[outer];
			PhysicsObject* object2 = m_customScene->m_physicObjects[inner];
			//sets ids to that of the objects
			int shape1ID = object1->m_shapeID;
			int shape2ID = object2->m_shapeID;
			//set an index using the objects shapes id
			int functionIndex = (shape1ID * NUMBERSHAPE) + shape2ID;
			CollisionDetectionFunction collisionFunctionPtr = CollisionFunctions[functionIndex];
			if (collisionFunctionPtr != NULL)
			{
				collisionFunctionPtr(object1,object2);
			}
		}
	}
}

 const std::array<CollisionDetectionFunction, 9> Collision::CollisionFunctions
 {
	Plane2Sphere,Plane2AABB,Plane2Plane,
	Sphere2Plane,Sphere2AABB,Sphere2Sphere,
	AABB2Plane,AABB2Sphere,AABB2AABB,
 };

 bool Collision::Plane2Sphere(PhysicsObject* planeObject, PhysicsObject* SphereObject)
 {
	return Sphere2Plane(SphereObject, planeObject);
 }

 bool Collision::Plane2AABB(PhysicsObject* planeObject, PhysicsObject* AABBObject)
 {
	Plane* plane = dynamic_cast<Plane*>(planeObject);
	AABB* aabb = dynamic_cast<AABB*>(AABBObject);

	glm::vec3 AABBPos = aabb->m_position;
	glm::vec3 minPos = AABBPos - aabb->GetExtent();
	glm::vec3 maxPos = AABBPos + aabb->GetExtent();

	float minPointDistanceAlongPlaneNormal = glm::dot(minPos, plane->m_normal);
	float maxPointDistanceAlongPlaneNormal = glm::dot(maxPos,plane->m_normal);

	float overlap = std::min(minPointDistanceAlongPlaneNormal,maxPointDistanceAlongPlaneNormal);

	if (overlap < 0)
	{
		
		return true;
	}
	return false;
 }

 bool Collision::Plane2Plane(PhysicsObject* planeObject1, PhysicsObject* planeObject2)
 {
	 return false;
 }

 bool Collision::Sphere2Plane(PhysicsObject* sphereObject, PhysicsObject* planeObject)
 {
	Sphere *sphere = dynamic_cast<Sphere*>(sphereObject);
	Plane *plane = dynamic_cast<Plane*>(planeObject);
	 
	glm::vec3 collisionNormal = plane->m_normal;
	float sphereToPlane = glm::dot(sphere->m_position, plane->m_normal) - plane->m_distance;

	//planes are infinetly thin,double sided,objects so if we are behind it we flip the normal
	if (sphereToPlane < 0)
	{
		collisionNormal *= -1;
		sphereToPlane *= -1;
	}
	//intersection between sphere and plane
	float intersection = sphere->m_radius - sphereToPlane;
	if (intersection > 0)
	{
		//find the point where the collision occured(we need this for colision response later)
		//the plane is always static so collision response only applies to the sphere
		glm::vec3 planeNormal = plane->m_normal;
		if (sphereToPlane < 0)
		{
			//flip the normal if we are behind the plane
			planeNormal *= -1;
		}

		glm::vec3 forceVector = -1 * sphere->m_mass * planeNormal 
								*(glm::dot(planeNormal, sphere->m_velocity));
		sphere->ApplyForce(glm::vec3(2) * forceVector);
		sphere->m_position += collisionNormal * intersection * 0.5f;
		return true;
	}
	return false;
 }

 bool Collision::Sphere2AABB(PhysicsObject* sphereObject, PhysicsObject* AABBObject)
 {
	Sphere* sphere = dynamic_cast<Sphere*>(sphereObject);
	AABB* aabb = dynamic_cast<AABB*>(AABBObject);

	glm::vec3 minPos = -aabb->GetExtent();
	glm::vec3 maxPos = aabb->GetExtent();

	glm::vec3 distance = sphere->m_position - aabb->m_position;
	glm::vec3 clampedPoint = distance;

	if (distance.x < minPos.x)
	{
		clampedPoint.x = minPos.x;
	}
	else if (distance.x > maxPos.x)
	{
		clampedPoint.x = maxPos.x;
	}

	if (distance.y < minPos.y)
	{
		clampedPoint.y = minPos.y;
	}
	else if (distance.y > maxPos.y)
	{
		clampedPoint.y = maxPos.y;
	}

	if (distance.z < minPos.z)
	{
		clampedPoint.z = minPos.z;
	}
	else if (distance.z > maxPos.z)
	{
		clampedPoint.z = maxPos.z;
	}

	glm::vec3 clampedDistance = distance - clampedPoint;

	float overlap = glm::length(clampedDistance) - sphere->m_radius;
	if (overlap < 0)
	{
		sphere->m_velocity = glm::vec3(0, 0, 0);
		aabb->m_velocity = glm::vec3(0, 0, 0);
		return true;
	}

	return false;
 }

 bool Collision::Sphere2Sphere(PhysicsObject* sphereObject1, PhysicsObject* sphereObject2)
 {
	 //try to cast objects to sphere and sphere
	 Sphere *sphere1 = dynamic_cast<Sphere*>(sphereObject1);
	 Sphere *sphere2 = dynamic_cast<Sphere*>(sphereObject2);
	 //if we are sucessfull then test for collision
	 if (sphere1 != NULL && sphere2 != NULL)
	 {
		//gets the vector between the two spheres
		glm::vec3 direction = sphere1->m_position - sphere2->m_position;
		float distanceBetweenSpheres = glm::length(direction);
		float combinedSphereRadius = sphere1->m_radius + sphere2->m_radius;

		float overlap = distanceBetweenSpheres - combinedSphereRadius;
		if (overlap < 0)
		{
			glm::vec3 collisionNormal = glm::normalize(direction);
			glm::vec3 relativeVelocity = sphere1->m_velocity - sphere2->m_velocity;
			glm::vec3 collisionVector = collisionNormal *(glm::dot(relativeVelocity,collisionNormal));
			glm::vec3 forceVector = collisionVector * 1.0f / (1 / sphere1->m_mass + 1 / sphere2->m_mass);
			//move our spheres out of collision
			glm::vec3 seperationVector = collisionNormal * overlap * 0.5f;
			sphere1->m_position -= seperationVector;
			sphere2->m_position += seperationVector;
			//use newtons third law to apply collision forces to colliding bodies.
			sphere1->ApplyForceToActor(sphere2,forceVector * glm::vec3(2));

			return true;
		}		
	 }
	 return false;
 }

 bool Collision::AABB2Plane(PhysicsObject* AABBObject, PhysicsObject* planeObject)
 {
	 return Plane2AABB(planeObject, AABBObject);
 }

 bool Collision::AABB2Sphere(PhysicsObject* AABBObject, PhysicsObject* sphereObject)
 {
	 return Sphere2AABB(sphereObject,AABBObject);
 }

 bool Collision::AABB2AABB(PhysicsObject* AABBObject1, PhysicsObject* AABBObject2)
 {
	 //sets the physic objects to AABB
	 AABB* axisAlignedBoundingBox1 = dynamic_cast<AABB*>(AABBObject1);
	 AABB* axisAlignedBoundingBox2 = dynamic_cast<AABB*>(AABBObject2);

	 glm::vec3 box1Pos = axisAlignedBoundingBox1->m_position;
	 glm::vec3 box1Extents = axisAlignedBoundingBox1->GetExtent();

	 glm::vec3 box2Pos = axisAlignedBoundingBox2->m_position;
	 glm::vec3 box2Extents = axisAlignedBoundingBox2->GetExtent();

	 glm::vec3 distanceBetweenBoxes = box2Pos - box1Pos;
	 glm::vec3 boxExtentsCombined = box1Extents + box2Extents;

	 float xOverlap = std::abs(distanceBetweenBoxes.x) - boxExtentsCombined.x;
	 float yOverlap = std::abs(distanceBetweenBoxes.y) - boxExtentsCombined.y;
	 float zOverlap = std::abs(distanceBetweenBoxes.z) - boxExtentsCombined.z;

	 if (xOverlap <= 0 && yOverlap <= 0 && zOverlap <= 0)
	 {
		 float minOverlap = xOverlap;
		 minOverlap = yOverlap < 0 ? std::max(minOverlap,yOverlap) : minOverlap;
		 minOverlap = zOverlap < 0 ? std::max(minOverlap, zOverlap) : minOverlap;

		 glm::vec3 seperationNormal(0);
		 if (xOverlap == minOverlap) seperationNormal.x = std::signbit(distanceBetweenBoxes.x) ? -1.0f : 1.0f;
		 else if (yOverlap == minOverlap) seperationNormal.y = std::signbit(distanceBetweenBoxes.y) ? -1.0f : 1.0f;
		 else if (zOverlap == minOverlap) seperationNormal.z = std::signbit(distanceBetweenBoxes.z) ? -1.0f : 1.0f;

		 return true;
	 }
	 return false;
 }