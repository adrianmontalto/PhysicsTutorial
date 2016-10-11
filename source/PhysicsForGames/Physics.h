#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <PxMaterial.h>
#include <vector>

class CustomPhysicsScene;

struct FilterGroup
{
	enum Enum
	{
		ePLAYER = (1 << 0),
		ePLATFORM = (1 << 1),
		eGROUND = (1 << 2)
	};
};
class Physics : public Application
{
public:
	CustomPhysicsScene* m_customPhysicsScene;
	Renderer* m_renderer;
	physx::PxFoundation* m_physicsFoundation;
	physx::PxPhysics* m_physics;
	physx::PxScene* m_physicsScene;
	physx::PxMaterial* m_physicsMaterial;
	physx::PxMaterial* m_boxMaterial;
	physx::PxCooking* m_physicsCooker;
	physx::PxControllerManager* m_controllerManager;
	physx::PxController* m_playerController;
	physx::PxDefaultErrorCallback m_defaultErrorCallback;
	physx::PxDefaultAllocator m_defaultAllocator;
	physx::PxSimulationFilterShader m_defaultFilterShader = physx::PxDefaultSimulationFilterShader;
	std::vector<physx::PxRigidStatic*> m_physXActors;
	FlyCamera m_camera;
	class Collision* m_collisionManager;
	class MyControllerHitReport* myHitReport;
	class ParticleFluidEmitter* m_particleEmitter;
	float m_delta_time;
	float m_ballTimer;
	float m_ballresetTimer;
	float m_characterYVelocity;
	float m_characterRotation;
	float m_playerGravity;
	bool m_isFluid;

	virtual bool startup();
	virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void renderGizmos(physx::PxScene* physics_scene);
	void SetUpPhysX();
	physx::PxScene* CreateDefaultScene();
	void UpDatePhysX(float deltaTime);
	void SetUpCustomPhysics();
	void UpdateCustomPhysics();
	void SetUpCustomBorders(float tableSize,float borderHeight,glm::vec4 colour);
	void SetUpVisualDebugger();
	void SetUpTutorial1();
	void SetUpIntroductionToPhysx();
	void SetupRBDTutorial();
	void SetupPhysXScene();
	void CreateDynamicSphere();
	void AddBlockTower();
	void AddPhysXBorders();
	void AddRagDoll();
	void AddCharacterController();
	void AddCollisionTrigger();
	void SetupCharacterController();
	void UpdateCharacterController();
	void ParticleTestScene();
	void CreateParticleSystem();
	void setupFiltering(physx::PxRigidActor* actor,physx::PxU32 filterGroup,physx::PxU32 filterMask);
	void SetShapeAsTrigger(physx::PxRigidActor* actorIn);
	void SetFluid(bool set);
	bool GetFluid();
	static physx::PxFilterFlags myFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, 
										physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, 
										physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);
};

class MyAllocator : public physx::PxAllocatorCallback
{
public:
	virtual ~MyAllocator() {}
	virtual void* allocate(size_t size,const char* typeName,const char* fileName,int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}

	virtual void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}
};

#endif //CAM_PROJ_H_
