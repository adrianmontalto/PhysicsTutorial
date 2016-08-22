#include "Ragdoll.h"

RagdollNode* ragdollData[] = 
{
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f, physx::PxVec3(0, 0, 1)),NO_PARENT,1,3,1,1,"lower spine"),
	new RagdollNode(physx::PxQuat(physx::PxPi,physx::PxVec3(0, 0, 1)),LOWER_SPINE,1,1,-1,1,"left pelvis"),
	new RagdollNode(physx::PxQuat(0,physx::PxVec3(0, 0, 1)),LOWER_SPINE,1,1,-1,1,"right pelvis"),
	new RagdollNode(physx::PxQuat(physx::PxPi /2.0f +0.2f,physx::PxVec3(0, 0, 1)),LEFT_PELVIS,5,2,-1,1,"L upper leg"),
	new RagdollNode(physx::PxQuat(physx::PxPi /2.0f - 0.2f, physx::PxVec3(0, 0, 1)),RIGHT_PELVIS,5,2,-1,1,"R upper leg"),
	new RagdollNode(physx::PxQuat(physx::PxPi /2.0f + 0.2f,physx::PxVec3(0, 0, 1)),LEFT_UPPER_LEG,5,1.75,-1,1,"L lower leg"),
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f - 0.2f,physx::PxVec3(0, 0, 1)),RIGHT_UPPER_LEG,5,1.75,-1,1,"R lower leg"),
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f,physx::PxVec3(0, 0, 1)),LOWER_SPINE,1,3,1,-1,"upper spine"),
	new RagdollNode(physx::PxQuat(physx::PxPi,physx::PxVec3(0, 0, 1)),UPPER_SPINE,1,1.5,1,1,"left clavicle"),
	new RagdollNode(physx::PxQuat(0,physx::PxVec3(0, 0, 1)),UPPER_SPINE,1,1.5,1,1,"right clavicle"),
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f,physx::PxVec3(0, 0, 1)),UPPER_SPINE,1,1,1,-1,"neck"),
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f,physx::PxVec3(0, 0, 1)),NECK,1,3,1,-1,"HEAD"),
	new RagdollNode(physx::PxQuat(physx::PxPi - 0.3f,physx::PxVec3(0, 0, 1)),LEFT_CLAVICLE,3,1.5,1,-1,"left upper arm"),
	new RagdollNode(physx::PxQuat(0.3f,physx::PxVec3(0, 0, 1)),RIGHT_CLAVICLE,3,1.5,-1,1,"right upper arm"),
	new RagdollNode(physx::PxQuat(physx::PxPi - 0.3f,physx::PxVec3(0, 0, 1)),LEFT_UPPER_ARM,3,1,-1,1,"left lower arm"),
	new RagdollNode(physx::PxQuat(0.3,physx::PxVec3(0, 0, 1)),RIGHT_UPPER_ARM,3,1,-1,1,"right lower arm"),
	NULL
};

Ragdoll::Ragdoll()
{
}


Ragdoll::~Ragdoll()
{
}

