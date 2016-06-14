#include "Physics.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "CustomPhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Collision.h"

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
    m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;

	m_renderer = new Renderer();

	//SetUpPhysX();
	//SetUpTutorial1();
	//SetUpVisualDebugger();
	SetUpCustomPhysics();
	SetUpCustomBorders(40.0f, 10.0f, glm::vec4(1, 0, 0, 1));
	m_collisionManager = new Collision(m_customPhysicsScene);
    return true;
}

void Physics::shutdown()
{
	//m_physicsScene->release();
	//m_physics->release();
	//m_physicsFoundation->release();
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

    m_camera.update(1.0f / 60.0f);
	//UpDatePhysX(m_delta_time);
	UpdateCustomPhysics();
	m_collisionManager->CheckForCustomCollision();
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

void AddWidget(PxShape* shape, PxRigidActor* actor, vec4 geo_color)
{
    PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
    vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
    glm::quat actor_rotation(full_transform.q.w,
        full_transform.q.x,
        full_transform.q.y,
        full_transform.q.z);
    glm::mat4 rot(actor_rotation);

    mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

    PxGeometryType::Enum geo_type = shape->getGeometryType();

    switch (geo_type)
    {
    case (PxGeometryType::eBOX) :
    {
        PxBoxGeometry geo;
        shape->getBoxGeometry(geo);
        vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
        Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
    } break;
    case (PxGeometryType::eCAPSULE) :
    {
        PxCapsuleGeometry geo;
        shape->getCapsuleGeometry(geo);
        Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::eSPHERE) :
    {
        PxSphereGeometry geo;
        shape->getSphereGeometry(geo);
        Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::ePLANE) :
    {

    } break;
    }
}

void Physics::renderGizmos(PxScene* physics_scene)
{
    PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
    PxU32 actor_count = physics_scene->getNbActors(desiredTypes);
    PxActor** actor_list = new PxActor*[actor_count];
	physics_scene->getActors(desiredTypes, actor_list, actor_count);
    
    vec4 geo_color(1, 0, 0, 1);
    for (int actor_index = 0;
        actor_index < (int)actor_count;
        ++actor_index)
    {
        PxActor* curr_actor = actor_list[actor_index];
        if (curr_actor->isRigidActor())
        {
            PxRigidActor* rigid_actor = (PxRigidActor*)curr_actor;
            PxU32 shape_count = rigid_actor->getNbShapes();
            PxShape** shapes = new PxShape*[shape_count];
            rigid_actor->getShapes(shapes, shape_count);

            for (int shape_index = 0;
                shape_index < (int)shape_count;
                ++shape_index)
            {
                PxShape* curr_shape = shapes[shape_index];
                AddWidget(curr_shape, rigid_actor, geo_color);
            }

            delete[]shapes;
        }
    }

    delete[] actor_list;

    int articulation_count = physics_scene->getNbArticulations();

    for (int a = 0; a < articulation_count; ++a)
    {
        PxArticulation* articulation;
		physics_scene->getArticulations(&articulation, 1, a);

        int link_count = articulation->getNbLinks();

        PxArticulationLink** links = new PxArticulationLink*[link_count];
        articulation->getLinks(links, link_count);

        for (int l = 0; l < link_count; ++l)
        {
            PxArticulationLink* link = links[l];
            int shape_count = link->getNbShapes();

            for (int s = 0; s < shape_count; ++s)
            {
                PxShape* shape;
                link->getShapes(&shape, 1, s);
                AddWidget(shape, link, geo_color);
            }
        }
        delete[] links;
    }
}

void Physics::SetUpPhysX()
{
	PxAllocatorCallback *myCallback = new MyAllocator();
	m_physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,*myCallback,m_defaultErrorCallback);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION,*m_physicsFoundation,PxTolerancesScale());
	PxInitExtensions(*m_physics);

	//create physics material
	m_physicsMaterial = m_physics->createMaterial(0.5f, 0.5f, 0.5f);

	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0,-10.0f,0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	m_physicsScene = m_physics->createScene(sceneDesc);
}

void Physics::UpDatePhysX(float deltaTime)
{
	if (deltaTime <= 0)
	{
		return;
	}
	m_physicsScene->simulate(deltaTime);
	while (m_physicsScene->fetchResults() == false)
	{

	}
}

void Physics::SetUpCustomPhysics()
{
	//set up the custom physics scene
	m_customPhysicsScene = new CustomPhysicsScene();
	m_customPhysicsScene->m_gravity = glm::vec3(0,0,0);
	m_customPhysicsScene->m_timeStep = 0.4f;

	//add sphere to scene
	Sphere* newBall;
	newBall = new Sphere(glm::vec3(0,10,0),glm::vec3(0,-0.2,0),3.0f,1.0f,glm::vec4(1,1,0,1),false);
	
	m_customPhysicsScene->AddPhysicsObject(newBall);

	newBall = new Sphere(glm::vec3(-8.2,10, 0), glm::vec3(0,-0.2,0), 3.0f, 1.0f, glm::vec4(1, 0, 0, 1),false);
	m_customPhysicsScene->AddPhysicsObject(newBall);

	Plane* newPlane;
	newPlane = new Plane(glm::vec3(0,-2,0),glm::vec3(0,1,0),glm::vec4(1,1,1,1),0.0f,20.0f,true);

	m_customPhysicsScene->AddPhysicsObject(newPlane);

	newPlane = new Plane(glm::vec3(0,10,0),glm::vec3(0, 1, 0), glm::vec4(1, 0, 0, 1), 0.0f,0.0f,true);

	m_customPhysicsScene->AddPhysicsObject(newPlane);
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
	AABB* newBox = new AABB(position,extent,colour,true);
	m_customPhysicsScene->AddPhysicsObject(newBox);

	position = glm::vec3(0,0.5f,(-tableSize / 2) - 1);
	extent = glm::vec3(tableSize / 2, borderHeight, 1);
	newBox = new AABB(position, extent, colour, true);
	m_customPhysicsScene->AddPhysicsObject(newBox);
	
	position = glm::vec3((tableSize / 2) - 1, 0.5f, 0);
	extent = glm::vec3(1,borderHeight,tableSize/2);
	newBox = new AABB(position, extent, colour, true);
	m_customPhysicsScene->AddPhysicsObject(newBox);
	
	position = glm::vec3((-tableSize / 2) + 1, 0.5f, 0);
	extent = glm::vec3(1, borderHeight, tableSize / 2);
	newBox = new AABB(position, extent, colour, true);
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
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	//and now try to connectPxVisualDebuggerExt
	auto theConnection = PxVisualDebuggerExt::createConnection(
										m_physics->getPvdConnectionManager(), pvd_host_ip, port, 
										timeout, connectionFlags);
}

void Physics::SetUpTutorial1()
{
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f,0,0.0f),PxQuat(PxHalfPi * 1.0f,PxVec3(0.0f,0.0f,1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*m_physics, pose, PxPlaneGeometry(), *m_physicsMaterial);
	//add it to the physX scene
	m_physicsScene->addActor(*plane);

	//add a box
	float density = 10;
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(0, 5, 0));
	PxRigidDynamic*  dynamicActor =PxCreateDynamic(*m_physics,transform,box,*m_physicsMaterial,density);

	//add it to the physX scene
	m_physicsScene->addActor(*dynamicActor);
}