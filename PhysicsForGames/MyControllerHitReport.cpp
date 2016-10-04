#include "MyControllerHitReport.h"
#include "physx\PxPhysics.h"
#include "physx\PxRigidDynamic.h"

void MyControllerHitReport::onShapeHit(const physx::PxControllerShapeHit &hit)
{
	//gets a reference to a structure which tells us what has been hit and where
	//get the actor from the shape we hit
	physx::PxRigidActor* actor = hit.shape->getActor();
	//get the normal of the thing we hit and store it so the player controller can respond correctly
	m_playerContactNormal = hit.worldNormal;
	//try to cast a dynamic actor
	physx::PxRigidDynamic* myActor = actor->is<physx::PxRigidDynamic>();
	if (myActor)
	{
		
	}
}