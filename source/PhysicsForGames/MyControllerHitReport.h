#pragma once
#include "physx\characterkinematic\PxController.h"

class MyControllerHitReport :public physx::PxUserControllerHitReport
{
public:
	
	//overload the onShapeHitFunction
	virtual void onShapeHit(const physx::PxControllerShapeHit &hit) override;
	//other collision functions which we must overload
	//these handle collision with other controllers and hitting obstacles
	virtual void onControllerHit(const physx::PxControllersHit &hit) override {};
	//called with current controller hits another controller
	virtual void onObstacleHit(const physx::PxControllerObstacleHit &hit) override {};
	//called when the current controller hits a user-defined obstacle
	MyControllerHitReport() :physx::PxUserControllerHitReport() {};
	physx::PxVec3 getPlayerContactNormal() { return m_playerContactNormal; };
	void clearPlayerContactNormal() { m_playerContactNormal = physx::PxVec3(0, 0, 0); };
	physx::PxVec3 m_playerContactNormal = physx::PxVec3(0);
};

