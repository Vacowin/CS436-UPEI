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

	void Update(float p_fDelta);
	void Render();

	void SetActiveCamera(Camera *p_pCamera);

	void AddTopNode(Node *p_pNode);
	void DoCulling(QuadTree* p_pTree, Frustum* p_Frustum, std::vector<Node*> & p_lNodes);

	void mouse_motion( int x, int y);
private:
	Scene();
	~Scene();
	static Scene* s_pSceneInstance;

	std::vector<Node*> m_lTopNode;
	std::vector<Node*> m_lRenderNode;
	Camera *m_pCamera;
	QuadTree* m_pQuadTree;

	int mouseX ;
	int mouseY;
	glm::vec2 mouse;
	bool m_bToggleQuadLines;
	bool m_bDrawQuadLines;
};

#endif