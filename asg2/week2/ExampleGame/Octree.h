#ifndef OCTREE_H
#define OCTREE_H

#include "Node.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "AABB.h"
#include <vector>

class OcTree
{
public:
	OcTree( float x, float y, float z, float width, float height,float depth, int level );
	void AddNode( Node *p_pNode );

	bool Contains( OcTree *p_pChildTree, Node *p_pNode );
	bool StopCritiria();

	float GetX() {return m_fX;}
	float GetY() {return m_fY;}
	float GetZ() {return m_fZ;}
	float GetWidth() {return m_fWidth;}
	float GetHeight() {return m_fHeight;}
	float GetDepth() {return m_fDepth;}

	std::vector<OcTree>& GetChildren() { return m_children; }
	const std::vector<Node*> GetNodes() { return m_lNodes;}
	AABB* GetAABB() { return m_pAABB;}

	void Render(const glm::mat4& mProj, const glm::mat4& mView);

private:
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fWidth;
	float m_fHeight;
	float m_fDepth;
	int m_iLevel;
	
	std::vector<Node*> m_lNodes;

	OcTree * m_pParent;
	std::vector<OcTree> m_children;
	bool m_bHasChildren;

	AABB *m_pAABB;

	wolf::VertexBuffer* m_pVB ;
	wolf::VertexDeclaration* m_pDecl;
	static wolf::Material* s_pMaterial;
};

#endif

