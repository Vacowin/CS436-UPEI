#ifndef SCENE_H
#define SCENE_H

#include "Node.h"
#include <vector>
#include "Camera.h"
#include "QuadTree.h"

class Scene
{
public:
	static void CreateInstance();
	static void DestroyInstance();
	static Scene* Instance();

	static int GetCurrentNodeID() { return s_iNodeIndex;}

	void Update(float p_fDelta);
	void Render();

	void SetActiveCamera(Camera *p_pCamera);

	void AddTopNode(Node *p_pNode);
	void RemoveNode(int p_iID);
	void DoCulling(QuadTree* p_pTree, Frustum* p_Frustum, std::vector<Node*> & p_lNodes);

	void MouseMotion( int x, int y);
private:
	Scene();
	~Scene();
	static Scene* s_pSceneInstance;

	std::vector<Node*> m_lTopNode;
	std::vector<Node*> m_lRenderNode;
	Camera *m_pCamera;
	Camera *m_pCamera1;
	Camera *m_pCamera2;
	bool m_bIsCamera1;
	bool m_bToggleCamera;
	QuadTree* m_pQuadTree;

	int mouseX ;
	int mouseY;
	glm::vec2 mouse;
	bool m_bToggleQuadLines;
	bool m_bDrawQuadLines;

	static int s_iNodeIndex;

};

#endif