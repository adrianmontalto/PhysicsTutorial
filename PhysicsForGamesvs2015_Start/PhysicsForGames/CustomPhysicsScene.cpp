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
		Sphere* sphere1 = dynamic_cast<Sphere*>(m_actors[0]);
		Sphere* sphere2 = dynamic_cast<Sphere*>(m_actors[1]);

		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A))
		{
			sphere1->ApplyForceToActor(sphere2,glm::vec3(0.01f,0.0f,0.0f));
		}
	}