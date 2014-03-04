#ifndef SCENE_H
#define SCENE_H

#include "Node.h"
#include <vector>
#include "Camera.h"

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

private:
	Scene();
	~Scene();
	void MouseMotion( int x, int y);
	void UpdateInput(float p_fDelta);

	static Scene* s_pSceneInstance;

	std::vector<Node*> m_lTopNode;
	std::vector<Node*> m_lRenderNode;
	std::vector<Node*> m_lRenderNodeOctree;
	Camera *m_pCamera;

	bool m_bToggleCamera;

	int mouseX ;
	int mouseY;
	glm::vec2 mouse;

	static int s_iNodeIndex;

};

#endif