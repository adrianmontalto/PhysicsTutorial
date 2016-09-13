#include "Ragdoll.h"



Ragdoll::Ragdoll()
{
}


Ragdoll::~Ragdoll()
{
}

physx::PxArticulation*  Ragdoll::MakeRagdoll(physx::PxPhysics* physics, RagdollNode** nodeArray,
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

		//build the transform for the link
		physx::PxTransform linkTransform = physx::PxTransform(currentNodePtr->scaledGlobalPos,
	    currentNodePtr->globalRotation);
		//create the link in the articulation
		physx::PxArticulationLink* link = articulation->createLink(parentLinkPtr,linkTransform);
		//add a pointer to this link into the ragdoll data so we have it for later when we want to link it
		currentNodePtr->linkptr = link;
		float jointSpace = .01f;//gap between joints
		float capsuleHalfLength = (halflength > jointSpace ? halflength - jointSpace : 0) + 0.01f;
		physx::PxCapsuleGeometry capsule(radius, capsuleHalfLength);
		link->createShape(capsule, *ragdollMaterial);//adds a capsule collider to the link
		physx::PxRigidBodyExt::updateMassAndInertia(*link,50.0f);//adds some mass

		if (currentNodePtr->parentNodeIdx != -1)
		{
			//get the pointer to joint from the link
			physx::PxArticulationJoint * joint = link->getInboundJoint();
			//get the relative rotation of this link
			physx::PxQuat frameRotation = parentNode->globalRotation.getConjugate() *
			currentNodePtr->globalRotation;
			//set the parent constraint frame
			physx::PxTransform parentConstraintFrame = physx::PxTransform(physx::PxVec3(currentNodePtr->
			parentLinkPos * parentHalfLength, 0, 0), frameRotation);
			//set the child constraint frame
			physx::PxTransform thisConstraintFrame = physx::PxTransform(physx::PxVec3
			(currentNodePtr->childLinkPos * childHalfLength, 0, 0));
			//set up the poses for the joint so it is in the correct place
			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisConstraintFrame);
			//set up some constraints to stop it flopping around
			joint->setStiffness(20);
			joint->setDamping(20);
			joint->setSwingLimit(0.4f,0.4f);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}

		//get a pointer to the current node
		currentNode++;
	}
	return articulation;
}