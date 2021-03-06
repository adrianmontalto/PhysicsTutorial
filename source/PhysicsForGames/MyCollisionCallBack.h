#pragma once
#include "physx\PxSimulationEventCallback.h"

class Physics;

class MyCollisionCallBack :public physx::PxSimulationEventCallback
{
public:
	Physics* m_physics;
	MyCollisionCallBack(Physics* physics);
	virtual void onContact(const physx::PxContactPairHeader& pairHeader,const physx::PxContactPair* pairs,physx::PxU32 nbPairs);
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 nbPairs);
	virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) {};
	virtual void onWake(physx::PxActor**,physx::PxU32) {};
	virtual void onSleep(physx::PxActor**, physx::PxU32) {};
};

