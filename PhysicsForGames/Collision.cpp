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

void Collision::CheckForCustomCollision()
{
	int objectCount = m_customScene->m_physicObjects.size();
	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer < objectCount - 1; ++outer)
	{
		if (m_customScene->m_physicObjects[outer]->GetShapeID() != JOINT)
		{
			for (int inner = outer + 1; inner < objectCount; ++inner)
			{
				if (m_customScene->m_physicObjects[inner]->GetShapeID() != JOINT)
				{
					//sets the objects to the outer and inner object
					PhysicsObject* object1 = m_customScene->m_physicObjects[outer];
					PhysicsObject* object2 = m_customScene->m_physicObjects[inner];
					//sets ids to that of the objects
					int shape1ID = object1->GetShapeID();
					int shape2ID = object2->GetShapeID();
					//set an index using the objects shapes id
					int functionIndex = (shape1ID * NUMBERSHAPE) + shape2ID;
					CollisionDetectionFunction collisionFunctionPtr = CollisionFunctions[functionIndex];
					if (collisionFunctionPtr != NULL)
					{
						collisionFunctionPtr(object1, object2);
					}
				}
			}
		}
	}
}

 const std::array<CollisionDetectionFunction, 9> Collision::CollisionFunctions
 {
	Plane2Plane,Plane2Sphere,Plane2AABB,
	Sphere2Plane,Sphere2Sphere,Sphere2AABB,
	AABB2Plane,AABB2Sphere,AABB2AABB,
 };

 bool Collision::Plane2Sphere(PhysicsObject* planeObject, PhysicsObject* SphereObject)
 {
	Sphere* sphere = static_cast<Sphere*>(SphereObject);
	Plane* plane = static_cast<Plane*>(planeObject);

	const glm::vec3 sphereVector = sphere->GetPosition();
	const glm::vec3 planeNormal = plane->GetNormal();

	float sphereDistanceAlongPlaneNormal = glm::dot(sphereVector,planeNormal);

	float overlap = sphereDistanceAlongPlaneNormal - (plane->GetDistance() + sphere->m_radius);
	if (overlap < 0)
	{
		Response(plane, sphere,-overlap,planeNormal);
		return true;
	}
	return false;
 }

 bool Collision::Plane2AABB(PhysicsObject* planeObject, PhysicsObject* AABBObject)
 {
	Plane* plane = dynamic_cast<Plane*>(planeObject);
	AABB* aabb = dynamic_cast<AABB*>(AABBObject);

	glm::vec3 AABBPos = aabb->GetPosition();
	glm::vec3 minPos = AABBPos - aabb->GetExtent();
	glm::vec3 maxPos = AABBPos + aabb->GetExtent();
	
	float minPointDistanceAlongPlaneNormal = glm::dot(minPos, plane->GetNormal());
	float maxPointDistanceAlongPlaneNormal = glm::dot(maxPos, plane->GetNormal());
	
	float overlap = std::min(minPointDistanceAlongPlaneNormal,maxPointDistanceAlongPlaneNormal);
	
	if (overlap < 0)
	{
		Response(plane,aabb,-overlap,plane->GetNormal());
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

	return Plane2Sphere(plane,sphere);
 }

 bool Collision::Sphere2AABB(PhysicsObject* sphereObject, PhysicsObject* AABBObject)
 {
	Sphere* sphere = dynamic_cast<Sphere*>(sphereObject);
	AABB* aabb = dynamic_cast<AABB*>(AABBObject);

	glm::vec3 minPos = -aabb->GetExtent();
	glm::vec3 maxPos = aabb->GetExtent();

	glm::vec3 distance = sphere->GetPosition() - aabb->GetPosition();
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
		Response(aabb,sphere,-overlap,glm::normalize(clampedDistance));
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
		glm::vec3 direction = sphere2->GetPosition() - sphere1->GetPosition();
		float distanceBetweenSpheres = glm::length(direction);
		float combinedSphereRadius = sphere1->m_radius + sphere2->m_radius;

		float overlap = distanceBetweenSpheres - combinedSphereRadius;
		if (overlap < 0)
		{
			Response(sphere1,sphere2,- overlap,glm::normalize(direction));
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

	 glm::vec3 box1Pos = axisAlignedBoundingBox1->GetPosition();
	 glm::vec3 box1Extents = axisAlignedBoundingBox1->GetExtent();

	 glm::vec3 box2Pos = axisAlignedBoundingBox2->GetPosition();
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

		 Response(axisAlignedBoundingBox1, axisAlignedBoundingBox2, -minOverlap, seperationNormal);
		 return true;
	 }
	 return false;
 }

 void Collision::Seperate(PhysicsObject* object1, PhysicsObject* object2, float overlap, glm::vec3 normal)
 {
	float totalMass = object1->GetMass() + object2->GetMass();
	float massRatio1 = object1->GetMass() / totalMass;
	float massRatio2 = object2->GetMass() / totalMass;

	glm::vec3 separationVector = normal * overlap;
	object1->AddPosition(-separationVector * massRatio2);
	object2->AddPosition(separationVector * massRatio1);
 }

 void Collision::Response(PhysicsObject* object1, PhysicsObject* object2, float overlap, glm::vec3 normal)
 {
	Seperate(object1, object2, overlap, normal);

	const float coefficientOfRestitution = 0.5f;

	glm::vec3 relativeVel = object2->GetVelocity() - object1->GetVelocity();
	float velocityAlongNormal = glm::dot(relativeVel, normal);
	float impulseAmount = -(1 + coefficientOfRestitution) * velocityAlongNormal;
	impulseAmount /= 1 / object1->GetMass() + 1 / object2->GetMass();

	glm::vec3 impulse = impulseAmount * normal;
	object1->AddVelocity(1/ object1->GetMass() * -impulse);
	object2->AddVelocity(1 / object2->GetMass() * +impulse);
 }