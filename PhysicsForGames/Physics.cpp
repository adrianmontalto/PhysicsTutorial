#include "Physics.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "CustomPhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "SpringJoint.h"
#include "Collision.h"
#include "Ragdoll.h"
#include "MyControllerHitReport.h"
#include "ParticleFluidEmitter.h"

#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"

#define Assert(val) if (val){}else{ *((char*)0) = 0;}
#define ArrayCount(val) (sizeof(val)/sizeof(val[0]))

bool Physics::startup()
{
    if (Application::startup() == false)
    {
        return false;
    }
	
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    Gizmos::create();

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(20,30,10), vec3(5,0,0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;
	m_ballTimer = 0.0f;
	m_ballresetTimer = 0.5f;
	m_characterYVelocity = 0.0f;
	m_characterRotation = 0.0f;
	m_playerGravity = 0.0f;

	m_renderer = new Renderer();

	SetUpPhysX();
	//SetUpIntroductionToPhysx();
	SetupPhysXScene();
	SetupCharacterController();
	//SetupRBDTutorial();
	//SetUpTutorial1();
	//SetUpVisualDebugger();
	//SetUpCustomPhysics();
	//SetUpCustomBorders(40.0f, 10.0f, glm::vec4(1, 0, 0, 1));
	ParticleTestScene();
	//m_collisionManager = new Collision(m_customPhysicsScene);
    return true;
}

void Physics::shutdown()
{
	m_physicsScene->release();
	m_physics->release();
	m_physicsFoundation->release();
	delete m_renderer;
    Gizmos::destroy();
    Application::shutdown();
}

bool Physics::update()
{
    if (Application::update() == false)
    {
        return false;
    }

    Gizmos::clear();

    float dt = (float)glfwGetTime();
    m_delta_time = dt;
    glfwSetTime(0.0);

    vec4 red(1,0,0,1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10),
            i == 10 ? red : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i),
            i == 10 ? red : black);
    }

	renderGizmos(m_physicsScene);
    m_camera.update(1.0f / 60.0f);
	UpDatePhysX(m_delta_time);
	//UpdateCustomPhysics();
	//m_collisionManager->CheckForCustomCollision();
    return true;
}

void Physics::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
    Gizmos::draw(m_camera.proj, m_camera.view);

    m_renderer->RenderAndClear(m_camera.view_proj);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void AddWidget(physx::PxShape* shape, physx::PxRigidActor* actor, vec4 geo_color)
{
	physx::PxTransform full_transform = physx::PxShapeExt::getGlobalPose(*shape, *actor);
    vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
    glm::quat actor_rotation(full_transform.q.w,
        full_transform.q.x,
        full_transform.q.y,
        full_transform.q.z);
    glm::mat4 rot(actor_rotation);

    mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

	physx::PxGeometryType::Enum geo_type = shape->getGeometryType();

    switch (geo_type)
    {
    case (physx::PxGeometryType::eBOX) :
    {
		physx::PxBoxGeometry geo;
        shape->getBoxGeometry(geo);
        vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
        Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
    } break;
    case (physx::PxGeometryType::eCAPSULE) :
    {
		physx::PxCapsuleGeometry geo;
        shape->getCapsuleGeometry(geo);
        Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (physx::PxGeometryType::eSPHERE) :
    {
		physx::PxSphereGeometry geo;
        shape->getSphereGeometry(geo);
        Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (physx::PxGeometryType::ePLANE) :
    {

    } break;
    }
}

void Physics::renderGizmos(physx::PxScene* physics_scene)
{
	physx::PxActorTypeFlags desiredTypes = physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC;
	physx::PxU32 actor_count = physics_scene->getNbActors(desiredTypes);
	physx::PxActor** actor_list = new physx::PxActor*[actor_count];
	physics_scene->getActors(desiredTypes, actor_list, actor_count);
    
    vec4 geo_color(1, 0, 0, 1);
    for (int actor_index = 0;
        actor_index < (int)actor_count;
        ++actor_index)
    {
		physx::PxActor* curr_actor = actor_list[actor_index];
        if (curr_actor->isRigidActor())
        {
			physx::PxRigidActor* rigid_actor = (physx::PxRigidActor*)curr_actor;
			physx::PxU32 shape_count = rigid_actor->getNbShapes();
			physx::PxShape** shapes = new physx::PxShape*[shape_count];
            rigid_actor->getShapes(shapes, shape_count);

            for (int shape_index = 0;
                shape_index < (int)shape_count;
                ++shape_index)
            {
				physx::PxShape* curr_shape = shapes[shape_index];
                AddWidget(curr_shape, rigid_actor, geo_color);
            }

            delete[]shapes;
        }
    }

    delete[] actor_list;

    int articulation_count = physics_scene->getNbArticulations();

    for (int a = 0; a < articulation_count; ++a)
    {
		physx::PxArticulation* articulation;
		physics_scene->getArticulations(&articulation, 1, a);

        int link_count = articulation->getNbLinks();

		physx::PxArticulationLink** links = new physx::PxArticulationLink*[link_count];
        articulation->getLinks(links, link_count);

        for (int l = 0; l < link_count; ++l)
        {
			physx::PxArticulationLink* link = links[l];
            int shape_count = link->getNbShapes();

            for (int s = 0; s < shape_count; ++s)
            {
				physx::PxShape* shape;
                link->getShapes(&shape, 1, s);
                AddWidget(shape, link, geo_color);
            }
        }
        delete[] links;
    }
}

void Physics::SetUpPhysX()
{
	m_physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,m_defaultAllocator,m_defaultErrorCallback);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION,*m_physicsFoundation, physx::PxTolerancesScale());
	PxInitExtensions(*m_physics);

	//create physics material
	m_physicsMaterial = m_physics->createMaterial(1, 1, 0);

	m_physicsCooker = PxCreateCooking(PX_PHYSICS_VERSION, *m_physicsFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
}

physx::PxScene* Physics::CreateDefaultScene()
{
	physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0, -9.807f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	physx::PxScene* result = m_physics->createScene(sceneDesc);

	return result;
}

void Physics::UpDatePhysX(float deltaTime)
{
	m_ballTimer -= deltaTime;
	if (deltaTime > 0)
	{
		m_physicsScene->simulate(deltaTime > 0.033f ? 0.033f : deltaTime);
		while (m_physicsScene->fetchResults() == false);
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (m_ballTimer < 0)
		{
			CreateDynamicSphere();
			m_ballTimer = m_ballresetTimer;
		}                		
	}

	if (m_particleEmitter)
	{
		m_particleEmitter->update(m_delta_time);
		//render all our particles
		m_particleEmitter->renderParticles();
	}

	UpdateCharacterController();
}

void Physics::SetUpCustomPhysics()
{
	//set up the custom physics scene
	m_customPhysicsScene = new CustomPhysicsScene();
	m_customPhysicsScene->m_gravity = glm::vec3(0,-0.2,0);
	m_customPhysicsScene->m_timeStep = 0.4f;

	//add sphere to scene
	Sphere* newBall;
	newBall = new Sphere(glm::vec3(-5,0,0),glm::vec3(2,0,0),3.0f,1.0f,glm::vec4(1,1,0,1),false);
	m_customPhysicsScene->AddPhysicsObject(newBall);
	
	newBall = new Sphere(glm::vec3(5,0,0), glm::vec3(-2,0,0), 3.0f, 1.0f, glm::vec4(1, 0, 0, 1),false);
	m_customPhysicsScene->AddPhysicsObject(newBall);
	
	newBall = new Sphere(glm::vec3(-5,0,-10), glm::vec3(2, 0, 0), 3.0f, 1.0f, glm::vec4(0, 1, 0, 1), false);
	m_customPhysicsScene->AddPhysicsObject(newBall);
	
	newBall = new Sphere(glm::vec3(5,0,-10), glm::vec3(-2, 0, 0), 3.0f, 1.0f, glm::vec4(0, 0, 1, 1), false);
	m_customPhysicsScene->AddPhysicsObject(newBall);

	AABB* newAABB;
	newAABB = new AABB(glm::vec3(0,1,0),glm::vec3(1,1,1),glm::vec4(0,1,0,1),10.0f,true);
	m_customPhysicsScene->AddPhysicsObject(newAABB);
	
	newAABB = new AABB(glm::vec3(-10,1,5),glm::vec3(1,1,1),glm::vec4(1,0,0,1),10.0f,false);
	newAABB->SetVelocity(glm::vec3(1,0,0));
	m_customPhysicsScene->AddPhysicsObject(newAABB);
	
	newAABB = new AABB(glm::vec3(10,1,5),glm::vec3(1,1,1),glm::vec4(0,0,1,1),10.0f,false);
	newAABB->SetVelocity(glm::vec3(-1, 0, 0));
	m_customPhysicsScene->AddPhysicsObject(newAABB);

	newAABB = new AABB(glm::vec3(0,10,-12), glm::vec3(1, 1, 1), glm::vec4(0.25, 0.7, 0.5, 1),10.0f,true);
	m_customPhysicsScene->AddPhysicsObject(newAABB);

	newBall = new Sphere(glm::vec3(0,5,-12), glm::vec3(0,0,0), 30.0f, 1.0f, glm::vec4(0.7, 0.2, 0.3, 1), false);
	m_customPhysicsScene->AddPhysicsObject(newBall);

	SpringJoint* newSpringJoint;
	newSpringJoint = new SpringJoint(newAABB,newBall,2.0f,0);
	m_customPhysicsScene->AddPhysicsObject(newSpringJoint);

	Plane* newPlane;
	newPlane = new Plane(glm::vec3(0,-2,0),glm::vec3(0,1,0),glm::vec4(1,1,1,1),0.0f,20.0f,true);
	m_customPhysicsScene->AddPhysicsObject(newPlane);

	//newPlane = new Plane(glm::vec3(0,10,0),glm::vec3(0,-1,0), glm::vec4(1, 0, 0, 1), 0.0f,0.0f,true);

	//m_customPhysicsScene->AddPhysicsObject(newPlane);
}

void Physics::UpdateCustomPhysics()
{
	m_customPhysicsScene->Update();
	m_customPhysicsScene->UpdateGizmos();
}

void Physics::SetUpCustomBorders(float tableSize, float borderHeight, glm::vec4 colour)
{
	glm::vec3 position = glm::vec3(0, 0.5f, (tableSize / 2) + 1);
	glm::vec3 extent = glm::vec3(tableSize / 2, borderHeight, 1);
	AABB* newBox = new AABB(position,extent,colour,10.0f,true);
	m_customPhysicsScene->AddPhysicsObject(newBox);

	position = glm::vec3(0,0.5f,(-tableSize / 2) - 1);
	extent = glm::vec3(tableSize / 2, borderHeight, 1);
	newBox = new AABB(position, extent, colour,10.0f,true);
	m_customPhysicsScene->AddPhysicsObject(newBox);
	
	position = glm::vec3((tableSize / 2) - 1, 0.5f, 0);
	extent = glm::vec3(1,borderHeight,tableSize/2);
	newBox = new AABB(position, extent, colour,10.0f,true);
	m_customPhysicsScene->AddPhysicsObject(newBox);
	
	position = glm::vec3((-tableSize / 2) + 1, 0.5f, 0);
	extent = glm::vec3(1, borderHeight, tableSize / 2);
	newBox = new AABB(position, extent, colour,10.0f,true);
	m_customPhysicsScene->AddPhysicsObject(newBox);
}

void Physics::SetUpVisualDebugger()
{
	//check if pvdConnection manager is available on this platform
	if (m_physics->getPvdConnectionManager() == NULL)
	{
		return;
	}
	//setup connection parameters
	const char* pvd_host_ip = "127.0.01";
	//IP of the PC which is running PVD
	int port = 5425;
	//TCP port to connect to,where PVD is listening
	unsigned int timeout = 100;
	//timeout in milliseconds to wait for PVD to respond,
	//consoles and remote PCs need a higher timeout.
	physx::PxVisualDebuggerConnectionFlags connectionFlags = physx::PxVisualDebuggerExt::getAllConnectionFlags();
	//and now try to connectPxVisualDebuggerExt
	auto theConnection = physx::PxVisualDebuggerExt::createConnection(
										m_physics->getPvdConnectionManager(), pvd_host_ip, port, 
										timeout, connectionFlags);
}

void Physics::SetUpTutorial1()
{
	//add a plane
	physx::PxTransform pose = physx::PxTransform(physx::PxVec3(0.0f,0,0.0f), physx::PxQuat(physx::PxHalfPi * 1.0f, physx::PxVec3(0.0f,0.0f,1.0f)));
	physx::PxRigidStatic* plane = physx::PxCreateStatic(*m_physics, pose, physx::PxPlaneGeometry(), *m_physicsMaterial);
	//add it to the physX scene
	m_physicsScene->addActor(*plane);

	//add a box
	float density = 10;
	physx::PxBoxGeometry box(2, 2, 2);
	physx::PxTransform transform(physx::PxVec3(0, 5, 0));
	physx::PxRigidDynamic*  dynamicActor = physx::PxCreateDynamic(*m_physics,transform,box,*m_physicsMaterial,density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);
}

void Physics::SetUpIntroductionToPhysx()
{
	m_physicsScene = CreateDefaultScene();

	//add a plane
	physx::PxTransform pose = physx::PxTransform(physx::PxVec3(0.0f, 0, 0.0f), physx::PxQuat(physx::PxHalfPi * 1.0f, physx::PxVec3(0.0f, 0.0f, 1.0f)));
	physx::PxRigidStatic* plane = physx::PxCreateStatic(*m_physics, pose, physx::PxPlaneGeometry(), *m_physicsMaterial);
	
	//add it to the physX scene
	m_physicsScene->addActor(*plane);

	//add a box
	float density = 10;
	physx::PxBoxGeometry box(2, 2, 2);
	physx::PxTransform transform(physx::PxVec3(0, 5, 0));
	physx::PxRigidDynamic*  dynamicActor = physx::PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);
}

void Physics::SetupRBDTutorial()
{
	m_physicsScene = CreateDefaultScene();

	//add a plane to thge scene
	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, 0, 0), physx::PxQuat((float)physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	physx::PxRigidStatic* plane = physx::PxCreateStatic(*m_physics,transform, physx::PxPlaneGeometry(),*m_physicsMaterial);

	m_physicsScene->addActor(*plane);

	AddPhysXBorders();
}

void Physics::SetupPhysXScene()
{
	m_physicsScene = CreateDefaultScene();

	//add a plane to thge scene
	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, 0, 0), physx::PxQuat((float)physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	physx::PxRigidStatic* plane = physx::PxCreateStatic(*m_physics, transform, physx::PxPlaneGeometry(), *m_physicsMaterial);

	m_physicsScene->addActor(*plane);

	AddRagDoll();
	AddPhysXBorders();
	AddBlockTower();
	AddCharacterController();
	CreateParticleSystem();
}

void Physics::CreateDynamicSphere()
{
	//transform
	vec3 cam_pos = m_camera.world[3].xyz();
	vec3 box_vel = -m_camera.world[2].xyz() * 20.0f;
	physx::PxTransform box_transform(physx::PxVec3(cam_pos.x, cam_pos.y -1, cam_pos.z));

	//geometry
	physx::PxSphereGeometry sphere(0.5f);

	//density
	float density = 10.0f;

	float muzzleSpeed = -50.0f;

	physx::PxRigidDynamic* new_actor = physx::PxCreateDynamic(*m_physics,box_transform,sphere,*m_physicsMaterial,density);

	glm::vec3 direction(m_camera.world[2]);
	physx::PxVec3 velocity = physx::PxVec3(direction.x, direction.y, direction.z) * muzzleSpeed;
	new_actor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	m_physicsScene->addActor(*new_actor);
}

void Physics::AddBlockTower()
{
	//add a box
	float density = 10;
	physx::PxBoxGeometry box(0.3f, 0.3f,0.3f);
	physx::PxTransform transform(physx::PxVec3(-7,1,0));
	physx::PxRigidDynamic*  dynamicActor = PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);

	//add a box
	density = 10;
	box = physx::PxBoxGeometry(0.3f, 0.3f, 0.3f);
	transform = physx::PxTransform(physx::PxVec3(-7, 1,-0.75));
	dynamicActor = PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);

	//add a box
	density = 10;
	box = physx::PxBoxGeometry(0.3f, 0.3f, 0.3f);
	transform = physx::PxTransform(physx::PxVec3(-7, 1, -1.5));
	dynamicActor = physx::PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);

	//add a box
	density = 10;
	box = physx::PxBoxGeometry(0.3f, 0.3f, 0.3f);
	transform = physx::PxTransform(physx::PxVec3(-7, 2, -0.5));
	dynamicActor = PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);

	//add a box
	density = 10;
	box = physx::PxBoxGeometry(0.3f, 0.3f, 0.3f);
	transform = physx::PxTransform(physx::PxVec3(-7, 2, -1));
	dynamicActor = PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);

	//add a box
	density = 10;
	box = physx::PxBoxGeometry(0.3f, 0.3f, 0.3f);
	transform = physx::PxTransform(physx::PxVec3(-7, 3, -0.75));
	dynamicActor = physx::PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);
}

void Physics::AddPhysXBorders()
{
	//add left box
	physx::PxBoxGeometry box(10, 2,0.5);
	physx::PxTransform transform = physx::PxTransform(0, 2, 10);
	physx::PxRigidStatic*  staticActor = physx::PxCreateStatic(*m_physics, transform, box, *m_physicsMaterial);

	//add it to the physX scene
	m_physicsScene->addActor(*staticActor);

	//add right box
	box = physx::PxBoxGeometry(10, 2, 0.5);
	transform = physx::PxTransform(0, 2, -10);
	staticActor = physx::PxCreateStatic(*m_physics, transform, box, *m_physicsMaterial);
	
	//add it to the physX scene
	m_physicsScene->addActor(*staticActor);
	
	//add top box
	box = physx::PxBoxGeometry(0.5, 2, 9.5);
	transform = physx::PxTransform(-9.5, 2, 0);
	staticActor = physx::PxCreateStatic(*m_physics, transform, box, *m_physicsMaterial);
	
	//add it to the physX scene
	m_physicsScene->addActor(*staticActor);
	
	//add bottom box
	box = physx::PxBoxGeometry(0.5, 2, 9.5);
	transform = physx::PxTransform(9.5 , 2, 0);
	staticActor = physx::PxCreateStatic(*m_physics, transform, box, *m_physicsMaterial);
	
	//add it to the physX scene
	m_physicsScene->addActor(*staticActor);
}

void Physics::AddRagDoll()
{
	physx::PxArticulation* ragDollArticulation;
	ragDollArticulation = Ragdoll::MakeRagdoll(m_physics, ragdollData, physx::PxTransform(physx::PxVec3(5.0f, 0.0f,5.0f)), 0.1f, m_physicsMaterial);
	m_physicsScene->addArticulation(*ragDollArticulation);
}

void Physics::AddCharacterController()
{
	float density = 10.0f;
	physx::PxCapsuleGeometry capsule = physx::PxCapsuleGeometry(0.5f,1.0f);
	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(5, 10, 0));
	physx::PxRigidDynamic*  dynamicActor = physx::PxCreateDynamic(*m_physics, transform, capsule,*m_physicsMaterial ,density);

	m_physicsScene->addActor(*dynamicActor);
}

void Physics::SetupCharacterController()
{
	myHitReport = new MyControllerHitReport();
	m_controllerManager = PxCreateControllerManager(*m_physicsScene);

	//describe our controller
	physx::PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0,0,0);
	desc.material = m_physicsMaterial;
	desc.reportCallback = myHitReport;//connect it to our collision detection routine
	desc.density = 10.0f;

	//create the layer controller
	m_playerController = m_controllerManager->createController(desc);
	m_playerController->setPosition(physx::PxExtendedVec3(5,0,0));

	//set up some variables to control the player with
	 m_characterYVelocity = 0; //initialise the character velocity
	 m_characterRotation = 0;//and rotation
	 m_playerGravity = -0.5f;//set up player gravity

	myHitReport->clearPlayerContactNormal();//initialise the contact normal(what we are in contact with)
	m_physicsScene->addActor(*m_playerController->getActor());
}

void Physics::UpdateCharacterController()
{
	bool onGround;//set to true if we are on the ground
	float movementSpeed = 10.0f;//forward and back movement
	float rotationSpeed = 1.0f;//turn speed
	//check if we have a contact normal
	if (myHitReport->getPlayerContactNormal().y > 0.3f)
	{
		m_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		m_characterYVelocity += m_playerGravity * m_delta_time;
		onGround = false;
	}
	
	myHitReport->clearPlayerContactNormal();
	const physx::PxVec3 up(0, 1, 0);
	
	//scan the keys and set up our intended velocity based on a global transform
	physx::PxVec3 velocity(0, m_characterYVelocity, 0);
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		velocity.x -= movementSpeed * m_delta_time;
	}
	
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		velocity.x += movementSpeed * m_delta_time;
	}

	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		velocity.z += movementSpeed * m_delta_time;
	}

	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		velocity.z -= movementSpeed * m_delta_time;
	}
	
	float minDistance = 0.001f;
	physx::PxControllerFilters filter;
	
	//make controls relative to player facing
	physx::PxQuat rotation(m_characterRotation,physx::PxVec3(0,1,0));
	//velocity = physx::PxVec3(0,m_characterYVelocity,0);
	
	//move the controller
	m_playerController->move(rotation.rotate(velocity), minDistance, m_delta_time, filter);
}

void Physics::ParticleTestScene()
{

	physx::PxTransform pose = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f), 
							  physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f)));

	physx::PxRigidStatic* plane = physx::PxCreateStatic(*m_physics,pose,physx::PxPlaneGeometry(),
								  *m_physicsMaterial);

	const physx::PxU32 numShapes = plane->getNbShapes();
	m_physicsScene->addActor(*plane);

	physx::PxBoxGeometry side1(4.5f, 1.0f, 0.5f);
	physx::PxBoxGeometry side2(0.5f,1.0f,4.5f);
	pose = physx::PxTransform(physx::PxVec3(0.0f, 0.5f, 4.0f));
	physx::PxRigidStatic* box = physx::PxCreateStatic(*m_physics, pose, side1, *m_physicsMaterial);

	m_physicsScene->addActor(*box);
	m_physXActors.push_back(box);

	pose = physx::PxTransform(physx::PxVec3(0.0f, 0.5f, -4.0f));
	box = PxCreateStatic(*m_physics, pose, side1, *m_physicsMaterial);
	m_physicsScene->addActor(*box);
	m_physXActors.push_back(box);

	pose = physx::PxTransform(physx::PxVec3(4.0f, 0.5f, 0.0f));
	box = physx::PxCreateStatic(*m_physics, pose, side2,*m_physicsMaterial);
	m_physicsScene->addActor(*box);
	m_physXActors.push_back(box);

	pose = physx::PxTransform(physx::PxVec3(-4.0f, 0.5f, 0.0f));
	box = physx::PxCreateStatic(*m_physics, pose, side2, *m_physicsMaterial);
	m_physicsScene->addActor(*box);
	m_physXActors.push_back(box);
}

void Physics::CreateParticleSystem()
{
	physx::PxParticleFluid* pf;

	//create particle system in PhysX SDK
	//set immutable properties.
	physx::PxU32 maxParticles = 4000;
	bool perParticleRestOffset = false;
	pf = m_physics->createParticleFluid(maxParticles, perParticleRestOffset);
	pf->setRestParticleDistance(0.3f);
	pf->setDynamicFriction(0.1f);
	pf->setStaticFriction(0.1f);
	pf->setDamping(0.1f);
	pf->setParticleMass(0.1f);
	pf->setRestitution(0);
	pf->setParticleBaseFlag(physx::PxParticleBaseFlag::eCOLLISION_TWOWAY,true);
	pf->setStiffness(100);

	if (pf)
	{
		m_physicsScene->addActor(*pf);
		m_particleEmitter = new ParticleFluidEmitter(maxParticles, physx::PxVec3(0.0f, 10.0f, 0.0f), pf, 0.1f);
		m_particleEmitter->setStartVelocityRange(-0.001f, -250.0f, -0.001f, 0.001f, -250.0f, 0.001f);
	}
 }