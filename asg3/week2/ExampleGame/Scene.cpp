#include "Scene.h"
#include <assert.h>
#define MOUSE_SENSITIVITY 0.001
#define WIDTH_QUAD 100
#define HEIGHT_QUAD 100
#define DEPTH_QUAD 100

Scene* Scene::s_pSceneInstance = NULL;
int Scene::s_iNodeIndex = 0;

void Scene::CreateInstance()
{
	assert(s_pSceneInstance == NULL);
	s_pSceneInstance = new Scene();
}


void Scene::DestroyInstance()
{
	assert(s_pSceneInstance != NULL);
	delete s_pSceneInstance;
	s_pSceneInstance = NULL;
}


Scene* Scene::Instance()
{
	assert(s_pSceneInstance);
	return s_pSceneInstance;
}

Scene::Scene()
{
	m_lTopNode = std::vector<Node*>();

	m_bToggleCamera = true;
}

Scene::~Scene()
{
}

void Scene::SetActiveCamera(Camera *p_pCamera)
{
	m_pCamera = p_pCamera;
}

void Scene::AddTopNode(Node *p_pNode)
{
	m_lTopNode.push_back(p_pNode);
	s_iNodeIndex++;
}

void Scene::RemoveNode(int p_iID)
{
	std::vector<Node*>::iterator it;
	for(it = m_lTopNode.begin(); it != m_lTopNode.end(); ++it) 
	{
		if (static_cast<Node*>(*it)->GetID() == p_iID)
			break;
	}
	if (it != m_lTopNode.end())
		m_lTopNode.erase(it);
	
}

void Scene::MouseMotion( int x, int y )
{
	double deltaX = x - mouse.x;
	double deltaY = y - mouse.y;
	
	// Camera Rotation
	m_pCamera->rotateHorizontal(MOUSE_SENSITIVITY * deltaY);
	m_pCamera->rotateVertical(MOUSE_SENSITIVITY * deltaX);
	
	mouse = glm::vec2(x, y);
}

void Scene::Update(float p_fDelta)
{
	UpdateInput(p_fDelta);

	// Update nodes
	for (int i=0;i<m_lTopNode.size(); i++)
	{
		Node *node = m_lTopNode.at(i);
		node->Update(p_fDelta);
	}
	
}

void Scene::UpdateInput(float p_fDelta)
{
	glfwGetMousePos(&mouseX, &mouseY);
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS )
	{
		MouseMotion(mouseX, mouseY);
	}
	else
	{
		mouse.x = mouseX;
		mouse.y = mouseY;
	}

	if( glfwGetKey( 'W' ) == GLFW_PRESS )
		m_pCamera->moveForward(0.1);
	else if( glfwGetKey( 'S' ) == GLFW_PRESS )
		m_pCamera->moveForward(-0.1);
	if ( glfwGetKey( 'A' ) == GLFW_PRESS )
		m_pCamera->moveLeftRight(- 0.7);
	else if ( glfwGetKey( 'D' ) == GLFW_PRESS )
		m_pCamera->moveLeftRight(0.7);

	m_pCamera->calculateCameraRotation();
	m_pCamera->Update(p_fDelta);
}


void Scene::Render()
{
	glm::mat4 projectionMatrix = m_pCamera->GetProjectionMatrix();
	glm::mat4 viewMatrix = m_pCamera->GetViewMatrix();

	for (int i = 0; i < m_lTopNode.size(); i++)
	{
		Node *node = m_lTopNode.at(i);
		node->Render(viewMatrix,projectionMatrix);
	}
}