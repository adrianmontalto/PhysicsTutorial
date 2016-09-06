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

physx::PxArticulation*  Ragdoll::MakeRagdol(physx::PxPhysics* physics, RagdollNode** nodeArray,
	physx::PxTransform worldPos, float scaleFactor,
	physx::PxMaterial* ragdollMaterial)
{
	//create the articulation for our ragdoll
	physx::PxArticulation *articulation = physics->createArticulation();
	RagdollNode** currentNode = nodeArray;

	//while there are moves to process......
	while (*currentNode != NULL)
	{
		//get a pointer to the current node
		currentNode++;
		//get a pointer to the current node
		RagdollNode* currentNodePtr = *currentNode;
		//create a pointer ready to hold the parent node pointer if there is one
		RagdollNode* parentNode = nullptr;
		//get scaled values for capsule
		float radius = currentNodePtr->radius * scaleFactor;
		float halflength = currentNodePtr->halfLength * scaleFactor;
		float childHalfLength = radius + halflength;
		float parentHalfLength = 0;//will be set later if there is a parent
		//get a pointer to the parent
		physx::PxArticulationLink* parentLinkPtr = NULL;
		currentNodePtr->scaledGlobalPos = worldPos.p;
		//checks to see if there is a parent node
		if (currentNodePtr->parentNodeIdx != -1)
		{
			//if there is a parent we need to work out our local position for the link
			//get a pointer to the parent node
			parentNode = *(nodeArray + currentNodePtr->parentNodeIdx);
			//get a pointer to the link for the parent
			parentLinkPtr = parentNode->linkptr;
			parentHalfLength = (parentNode->radius + parentNode->halfLength) * scaleFactor;
			//work out the local position of the node
			physx::PxVec3 currentRelative = currentNodePtr->childLinkPos * 
			currentNodePtr->globalRotation.rotate(physx::PxVec3(childHalfLength, 0, 0));
			physx::PxVec3 parrentRelative = -currentNodePtr->parentLinkPos 
			* parentNode->globalRotation.rotate(physx::PxVec3(parentHalfLength,0,0));
			currentNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos - 
			(parrentRelative + currentRelative);
		}
	}
	return articulation;
}