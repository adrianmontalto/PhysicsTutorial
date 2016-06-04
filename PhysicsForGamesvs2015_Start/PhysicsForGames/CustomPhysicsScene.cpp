#include "CustomPhysicsScene.h"
#include "PhysicsObject.h"
#include "GLFW\glfw3.h"
#include "Sphere.h"
#include "RigidBody.h"

	CustomPhysicsScene::CustomPhysicsScene()
	{
	}

	CustomPhysicsScene::~CustomPhysicsScene()
	{
	}

	void CustomPhysicsScene::addActor(PhysicsObject* object)
	{
		m_actors.push_back(object);
	}

	void CustomPhysicsScene::removeActor(PhysicsObject* object)
	{
		for (int i = 0; i < m_actors.size(); ++i)
		{
			if (m_actors[i] == object)
			{
				delete m_actors[i];
			}
		}
	}

	void CustomPhysicsScene::Update()
	{
		CheckForInput();
		for (int i = 0; i < m_actors.size(); ++i)
		{
			m_actors[i]->Update(m_gravity, m_timeStep);
		}
	}

	void CustomPhysicsScene::UpdateGizmos()
	{
		for (int i = 0; i < m_actors.size(); ++i)
		{
			m_actors[i]->MakeGizmo();
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
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A))
		{
			//m_actors[0]->ApplyForceToActor(m_actors[1]);
		}
	}