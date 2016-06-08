#include "CustomPhysicsScene.h"
#include "GLFW\glfw3.h"
#include "Sphere.h"
#include "RigidBody.h"
#include "Plane.h"

CustomPhysicsScene::CustomPhysicsScene()
{
	m_timeStep = 0.0f;
}

CustomPhysicsScene::~CustomPhysicsScene()
{
}

void CustomPhysicsScene::AddPhysicsObject(PhysicsObject* object)
{
	m_physicObjects.push_back(object);
}

void CustomPhysicsScene::RemovePhysicsObject(PhysicsObject* object)
{
	for (int i = 0; i < m_physicObjects.size(); ++i)
	{
		if (m_physicObjects[i] == object)
		{
			delete m_physicObjects[i];
		}
	}
}

void CustomPhysicsScene::Update()
{
	CheckForInput();
	for (int i = 0; i < m_physicObjects.size(); ++i)
	{
		m_physicObjects[i]->Update(m_gravity,m_timeStep);
	}
}

void CustomPhysicsScene::UpdateGizmos()
{
	for (int i = 0; i < m_physicObjects.size(); ++i)
	{
		m_physicObjects[i]->MakeGizmo();
	}
}

void CustomPhysicsScene::DebugScene()
{

}

void CustomPhysicsScene::AddGizmos()
{

}

void CustomPhysicsScene::CheckForInput()
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(m_physicObjects[0]);
	Sphere* sphere2 = dynamic_cast<Sphere*>(m_physicObjects[1]);
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Z))
	{
		sphere1->ApplyForceToActor(sphere2,glm::vec3(1,0,0));
	}
}
