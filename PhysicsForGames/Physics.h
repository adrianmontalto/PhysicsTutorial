#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"

#include <PxPhysicsAPI.h>
#include <PxScene.h>

using namespace physx;

class CustomPhysicsScene;

class Physics : public Application
{
public:
	CustomPhysicsScene* m_customPhysicsScene;
	Renderer* m_renderer;
	PxFoundation* m_physicsFoundation;
	PxPhysics* m_physics;
	PxScene* m_physicsScene;
	PxMaterial* m_physicsMaterial;
	PxMaterial* m_boxMaterial;
	PxCooking* m_physicsCooker;
	PxControllerManager* m_controllerManager;
	PxDefaultErrorCallback m_defaultErrorCallback;
	PxDefaultAllocator m_defaultAllocator;
	PxSimulationFilterShader m_defaultFilterShader = PxDefaultSimulationFilterShader;
	FlyCamera m_camera;
	class Collision* m_collisionManager;
	float m_delta_time;
	float m_ballTimer;
	float m_ballresetTimer;

	virtual bool startup();
	virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void renderGizmos(PxScene* physics_scene);
	void SetUpPhysX();
	PxScene* CreateDefaultScene();
	void UpDatePhysX(float deltaTime);
	void SetUpCustomPhysics();
	void UpdateCustomPhysics();
	void SetUpCustomBorders(float tableSize,float borderHeight,glm::vec4 colour);
	void SetUpVisualDebugger();
	void SetUpTutorial1();
	void SetUpIntroductionToPhysx();
	void SetupRBDTutorial();
	void CreateDynamicSphere();
	void AddPhysXBorders();
};

class MyAllocator : public PxAllocatorCallback
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
