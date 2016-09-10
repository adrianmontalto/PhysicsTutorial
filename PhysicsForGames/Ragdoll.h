#pragma once
#include <PxPhysicsAPI.h>
#include <PxArticulation.h>


//parts which make up the ragdoll
enum RagDollParts
{
	NO_PARENT = -1,
	LOWER_SPINE,
	LEFT_PELVIS,
	RIGHT_PELVIS,
	LEFT_UPPER_LEG,
	RIGHT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_LOWER_LEG,
	UPPER_SPINE,
	LEFT_CLAVICLE,
	RIGHT_CLAVICLE,
	NECK,
	HEAD,
	LEFT_UPPER_ARM,
	RIGHT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_LOWER_ARM,
};

struct RagdollNode
{
	physx::PxQuat globalRotation;//rotation of the link in model space

	physx::PxVec3 scaledGlobalPos;//position of the link centre in world space which is
	//calculated when we process the node

	int parentNodeIdx;//Index of the parent node
	float halfLength;//half length of the capsule for this node
	float radius;//radius of the capsule for thisnode
	float parentLinkPos;//relative position of link centre in parent to this node
	float childLinkPos;//relative position of link centre in child
	char* name;//name of link
	physx::PxArticulationLink* linkptr;
	//constructor
	RagdollNode(physx::PxQuat _globalRotation, int _parentNodeIdx, float _halfLength,
		float _radius, float _parentLinkPos, float _childLinkPos, char* _name) {
		globalRotation =_globalRotation, parentNodeIdx = _parentNodeIdx; 
		halfLength = _halfLength; radius = _radius; parentLinkPos = _parentLinkPos;
		childLinkPos = _childLinkPos; name = _name;};
};
class Ragdoll
{
public:
	const physx::PxVec3 X_AXIS = physx::PxVec3(1,0,0);
	const physx::PxVec3 Y_AXIS = physx::PxVec3(0, 1, 0);
	const physx::PxVec3 Z_AXIS = physx::PxVec3(0, 0, 1);
	Ragdoll();
	~Ragdoll();
	physx::PxArticulation* MakeRagdoll(physx::PxPhysics* physics,RagdollNode** nodeArray,
									  physx::PxTransform worldPos,float scaleFactor,
		                              physx::PxMaterial* ragdollMaterial);
};

