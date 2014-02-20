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
	m_pQuadTree = new QuadTree(0, 0, WIDTH_QUAD, HEIGHT_QUAD, 0);
	m_pOcTree = new OcTree(0, 0, 0, WIDTH_QUAD, HEIGHT_QUAD, DEPTH_QUAD, 0);

	m_bToggleQuadLines = true;
	m_bDrawQuadLines = true;
	m_bIsCamera1 = true;
	m_bToggleCamera = true;

	m_pCamera1 = new Camera(45.0f, 1280.0f / 720.0f, 0.1f, 200.0f, glm::vec3(0.0f, 30.0f, 10.0f), glm::vec3(1.0f,1.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	m_pCamera2 = new Camera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 30.0f, 10.0f), glm::vec3(1.0f,1.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	SetActiveCamera(m_pCamera1);
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
	if (!m_bRenderOctree)
	{
		glm::vec3 pos = p_pNode->GetTranslation();
		p_pNode->SetTranslation(glm::vec3(pos.x, 0, pos.y));
		m_pQuadTree->AddNode (p_pNode);
	}
	else
		m_pOcTree->AddNode(p_pNode);

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
		m_pCamera->moveForward(0.1);
	else if( glfwGetKey( 'S' ) == GLFW_PRESS )
		m_pCamera->moveForward(-0.1);
	if ( glfwGetKey( 'A' ) == GLFW_PRESS )
		m_pCamera->moveLeftRight(- 0.7);
	else if ( glfwGetKey( 'D' ) == GLFW_PRESS )
		m_pCamera->moveLeftRight(0.7);

	m_pCamera->calculateCameraRotation();
	m_pCamera->Update(p_fDelta);

	if (!m_bIsCamera1)
	{
		if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS )
			m_pCamera1->moveForward(0.1);
		else if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
			m_pCamera1->moveForward(-0.1);
		if ( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
			m_pCamera1->moveLeftRight(- 0.7);
		else if ( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
			m_pCamera1->moveLeftRight(0.7);

		m_pCamera1->Update(p_fDelta);
	}

	// Turn on/off Lines
	if (glfwGetKey('X') == GLFW_PRESS && !m_bToggleQuadLines)
    {
		m_bDrawQuadLines = !m_bDrawQuadLines;
        m_bToggleQuadLines = true;
    }
    else if (glfwGetKey('X') == GLFW_RELEASE)
        m_bToggleQuadLines = false;

	// Switch Cameras
	if( glfwGetKey( 'M' ) == GLFW_PRESS && !m_bToggleCamera)
	{
		if (m_bIsCamera1)
		{
			Scene::Instance()->SetActiveCamera(m_pCamera2);
			m_bIsCamera1 = false;
		}
		else
		{
			Scene::Instance()->SetActiveCamera(m_pCamera1);
			m_bIsCamera1 = true;
		}
		m_bToggleCamera = true;
	}
	else if (glfwGetKey('M') == GLFW_RELEASE)
        m_bToggleCamera = false;



	// Update nodes
	for (int i=0;i<m_lTopNode.size(); i++)
	{
		Node *node = m_lTopNode.at(i);
		node->Update(p_fDelta);
	}
	
	// Culling
	m_lRenderNode.clear();
	DoCulling(m_pQuadTree, m_pCamera1->GetFrustum(), m_lRenderNode);

	m_lRenderNodeOctree.clear();
	OctreeCulling(m_pOcTree, m_pCamera1->GetFrustum(), m_lRenderNodeOctree); 

	if (m_bRenderOctree)
		printf("\nNode: %d    %d", m_lRenderNodeOctree.size());
	else
		printf("\nNode: %d    %d", m_lRenderNode.size());
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

void Scene::OctreeCulling(OcTree* p_pTree, Frustum* p_Frustum, std::vector<Node*> & p_lNodes)
{
	if (p_Frustum->OcTreeInFrustum(p_pTree) == Frustum::OUTSIDE)
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
				m_lRenderNodeOctree.push_back(node);
		}

		for (int i = 0; i < p_pTree->GetChildren().size(); i++)
		{
			OctreeCulling(&p_pTree->GetChildren().at(i), p_Frustum, m_lRenderNodeOctree);
		}
	}
}

void Scene::Render()
{
	glm::mat4 projectionMatrix = m_pCamera->GetProjectionMatrix();
	glm::mat4 viewMatrix = m_pCamera->GetViewMatrix();

	if (!m_bRenderOctree)
	{
		for (int i=0;i<m_lRenderNode.size();i++)
		{
			Node *node = m_lRenderNode.at(i);
			node->Render(viewMatrix,projectionMatrix);
		}
	}
	else
	{
		for (int i=0;i<m_lRenderNodeOctree.size();i++)
		{
			Node *node = m_lRenderNodeOctree.at(i);
			node->Render(viewMatrix,projectionMatrix);
		}
	}

	if (m_bDrawQuadLines)
	{
		if (!m_bRenderOctree)
			m_pQuadTree->Render(projectionMatrix, viewMatrix);
		else
			m_pOcTree->Render(projectionMatrix, viewMatrix);
	}

	// Draw Fustum if using 2nd camera
	if (!m_bIsCamera1)
	{
		glm::mat4 projectionMatrix1 = m_pCamera1->GetProjectionMatrix();
		glm::mat4 viewMatrix1 = m_pCamera1->GetViewMatrix();
		m_pCamera1->GetFrustum()->Render(viewMatrix, projectionMatrix);
	}
}