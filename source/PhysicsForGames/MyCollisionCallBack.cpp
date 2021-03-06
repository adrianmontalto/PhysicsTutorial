#include "MyCollisionCallBack.h"
#include <iostream>
#include "physx\PxFiltering.h"
#include "physx\PxRigidActor.h"
#include "physx\PxActor.h"
#include "Physics.h"

MyCollisionCallBack::MyCollisionCallBack(Physics* physics)
{
	m_physics = physics;
}

void MyCollisionCallBack::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; ++i)
	{
		const physx::PxContactPair& cp = pairs[i];
		//only interested in touches found and lost
		if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cout << "Collision Detected between:";
			std::cout << pairHeader.actors[0]->getName();
			std::cout << pairHeader.actors[1]->getName() << std::endl;

		}
	}
}

void MyCollisionCallBack::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; ++i)
	{
		physx::PxTriggerPair* pair = pairs + i;
		physx::PxActor* triggerActor = pair->triggerActor;
		physx::PxActor* otherActor = pair->otherActor;
		std::cout << otherActor->getName();
		std::cout << "Entered Trigger";
		std::cout << triggerActor->getName() << std::endl;
		if (m_physics->GetFluid() == true)
		{
			m_physics->SetFluid(false);
			std::cout << "turn fluids off" << std::endl;
		}
		if (m_physics->GetFluid() == false)
		{
			m_physics->SetFluid(true);
			std::cout << "turn fluid on" << std::endl;
		}
	}
}