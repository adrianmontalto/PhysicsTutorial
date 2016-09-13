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

static RagdollNode* ragdollData[] =
{
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f, physx::PxVec3(0, 0, 1)),NO_PARENT,1,3,1,1,"lower spine"),
	new RagdollNode(physx::PxQuat(physx::PxPi,physx::PxVec3(0, 0, 1)),LOWER_SPINE,1,1,-1,1,"left pelvis"),
	new RagdollNode(physx::PxQuat(0,physx::PxVec3(0, 0, 1)),LOWER_SPINE,1,1,-1,1,"right pelvis"),
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f + 0.2f,physx::PxVec3(0, 0, 1)),LEFT_PELVIS,5,2,-1,1,"L upper leg"),
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f - 0.2f, physx::PxVec3(0, 0, 1)),RIGHT_PELVIS,5,2,-1,1,"R upper leg"),
	new RagdollNode(physx::PxQuat(physx::PxPi / 2.0f + 0.2f,physx::PxVec3(0, 0, 1)),LEFT_UPPER_LEG,5,1.75,-1,1,"L lower leg"),
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

class Ragdoll
{
public:
	const physx::PxVec3 X_AXIS = physx::PxVec3(1,0,0);
	const physx::PxVec3 Y_AXIS = physx::PxVec3(0, 1, 0);
	const physx::PxVec3 Z_AXIS = physx::PxVec3(0, 0, 1);
	Ragdoll();
	~Ragdoll();
	static physx::PxArticulation* MakeRagdoll(physx::PxPhysics* physics,RagdollNode** nodeArray,
									  physx::PxTransform worldPos,float scaleFactor,
		                              physx::PxMaterial* ragdollMaterial);
};

