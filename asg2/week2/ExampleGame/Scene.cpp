#include "Scene.h"
#include <assert.h>
#define MOUSE_SENSITIVITY 0.001
#define WIDTH_QUAD 100
#define HEIGHT_QUAD 100

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
	m_pQuadTree = new QuadTree(0, 0, WIDTH_QUAD, HEIGHT_QUAD, 0);

	m_bToggleQuadLines = true;
	m_bDrawQuadLines = true;
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
	m_pQuadTree->AddNode (p_pNode);
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
		m_pCamera->moveForward(0.05);
	else if( glfwGetKey( 'S' ) == GLFW_PRESS )
		m_pCamera->moveForward(-0.05);
	if ( glfwGetKey( 'A' ) == GLFW_PRESS )
		m_pCamera->moveLeftRight(- 0.2);
	else if ( glfwGetKey( 'D' ) == GLFW_PRESS )
		m_pCamera->moveLeftRight(0.2);

	m_pCamera->calculateCameraRotation();
	m_pCamera->Update(p_fDelta);

	if (glfwGetKey('X') == GLFW_PRESS && !m_bToggleQuadLines)
    {
		m_bDrawQuadLines = !m_bDrawQuadLines;
        m_bToggleQuadLines = true;
    }
    else if (glfwGetKey('X') == GLFW_RELEASE)
        m_bToggleQuadLines = false;


	// Update nodes
	for (int i=0;i<m_lTopNode.size(); i++)
	{
		Node *node = m_lTopNode.at(i);
		node->Update(p_fDelta);
	}
	
	// Culling
	m_lRenderNode.clear();
	DoCulling(m_pQuadTree, m_pCamera->GetFrustum(), m_lRenderNode);

	printf("\nNode: %d", m_lRenderNode.size());
}


void Scene::DoCulling(QuadTree* p_pTree, Frustum* p_Frustum, std::vector<Node*> & p_lRenderNodes)
{
	if (p_Frustum->QuadTreeInFrustum(p_pTree) == Frustum::OUTSIDE)
	{
		return;
	}
	else
	{
		std::vector<Node*> lNodes = p_pTree->GetNodes();
		for (int i=0;i<lNodes.size(); i++)
		{
			Node *node = lNodes.at(i);
			if (p_Frustum->sphereInFrustum(node->GetWorldTranslation(), node->GetBVRadius()) != Frustum::OUTSIDE)
				p_lRenderNodes.push_back(node);
		}

		if (p_pTree->GetNWChild() !=NULL)
			DoCulling(p_pTree->GetNWChild(), p_Frustum, p_lRenderNodes);
		if (p_pTree->GetNEChild() !=NULL)
			DoCulling(p_pTree->GetNEChild(), p_Frustum, p_lRenderNodes);
		if (p_pTree->GetSWChild() !=NULL)
			DoCulling(p_pTree->GetSWChild(), p_Frustum, p_lRenderNodes);
		if (p_pTree->GetSEChild() !=NULL)
			DoCulling(p_pTree->GetSEChild(), p_Frustum, p_lRenderNodes);
	}
}

void Scene::Render()
{
	glm::mat4 projectionMatrix = m_pCamera->GetProjectionMatrix();
	glm::mat4 viewMatrix = m_pCamera->GetViewMatrix();

	for (int i=0;i<m_lRenderNode.size();i++)
	{
		Node *node = m_lRenderNode.at(i);
		node->Render(viewMatrix,projectionMatrix);
	}

	if (m_bDrawQuadLines)
		m_pQuadTree->Render(projectionMatrix, viewMatrix);
}