#ifndef QUADTREE_H
#define QUADTREE_H

#include "Node.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "AABB.h"

class QuadTree
{
public:
	QuadTree( float x, float z, float width, float height, int level );
	void AddNode( Node *p_pNode );

	bool Contains( QuadTree *p_pChildTree, Node *p_pNode );
	bool StopCritiria();

	float GetX() {return m_fX;}
	float GetZ() {return m_fZ;}
	float GetWidth() {return m_fWidth;}
	float GetHeight() {return m_fHeight;}

	QuadTree *GetNWChild() { return m_pNWTree;}
	QuadTree *GetNEChild() { return m_pNETree;}
	QuadTree *GetSWChild() { return m_pSWTree;}
	QuadTree *GetSEChild() { return m_pSETree;}

	std::vector<Node*> GetNodes() { return m_lNodes;}
	AABB* GetAABB() { return m_pAABB;}

	void Render(glm::mat4 mProj, glm::mat4 mView);

private:
	float m_fX;
	float m_fZ;
	float m_fWidth;
	float m_fHeight;
	int m_iLevel;
	
	std::vector<Node*> m_lNodes;

	QuadTree * m_pParent;
	QuadTree * m_pNWTree;
	QuadTree * m_pNETree;
	QuadTree * m_pSWTree;
	QuadTree * m_pSETree;

	AABB *m_pAABB;

	wolf::VertexBuffer* m_pVB ;
	wolf::VertexDeclaration* m_pDecl;
	wolf::Program* m_pProgram ;
};

#endif

