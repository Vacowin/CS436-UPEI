#include "QuadTree.h"


Material *QuadTree::s_pMaterial = NULL;


QuadTree::QuadTree( float x, float z, float width, float height, int level )
{
	m_fX = x;
	m_fZ = z;
	m_fWidth = width;
	m_fHeight = height;
	m_iLevel = level;
	m_pNWTree = NULL;
	m_pNETree = NULL;
	m_pSWTree = NULL;
	m_pSETree = NULL;

	m_pAABB = new AABB(glm::vec3(x,0,z), glm::vec3(width,2,height));

	const Vertex lineVertices[] = {

		{ x, 0.0f, z, 255, 0, 0, 255 },
		{ x+width, 0.0f, z, 255, 0, 0, 255 },

		{ x+width, 0.0f, z, 255, 0, 0, 255 },
		{ x+width, 0.0f, z+height, 255, 0, 0, 255 },

		{ x+width, 0.0f, z+height, 255, 0, 0, 255 },
		{ x, 0.0f, z+height, 255, 0, 0, 255 },

		{ x, 0.0f, z+height, 255, 0, 0, 255 },
		{ x, 0.0f, z, 255, 0, 0, 255 },
	};

    m_pVB = wolf::BufferManager::CreateVertexBuffer(lineVertices, sizeof(Vertex) * 8);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();

	if (!s_pMaterial)
	{
		s_pMaterial = wolf::MaterialManager::CreateMaterial("quadtreeline");
		s_pMaterial->SetProgram("data/cube.vsh", "data/cube.fsh");
	}
}

void QuadTree::AddNode( Node *p_pNode )
{
	if (m_pNWTree==NULL)
		m_lNodes.push_back(p_pNode);
	else
	{
		if ( Contains( m_pNWTree, p_pNode ) ) 
			{
				m_pNWTree->AddNode( p_pNode ); 
			}
			else if ( Contains( m_pNETree, p_pNode ) ) 
			{
				m_pNETree->AddNode( p_pNode ); 
			}
			else if ( Contains( m_pSWTree, p_pNode ) ) 
			{
				m_pSWTree->AddNode( p_pNode ); 
			}
			else if ( Contains( m_pSETree, p_pNode ) ) 
			{
				m_pSETree->AddNode( p_pNode ); 
			}
			// Add to parent tree if nodes are not completely inside one tree 
			else
			{
				m_lNodes.push_back(p_pNode);
			}
	}
	
	//  check stop critiria  
	if (!StopCritiria())
	{
		m_pNWTree = new QuadTree( m_fX, m_fZ, m_fWidth / 2.0f, m_fHeight / 2.0f, m_iLevel+1 );
		m_pNETree = new QuadTree( m_fX + m_fWidth / 2.0f, m_fZ, m_fWidth / 2.0f, m_fHeight / 2.0f, m_iLevel+1 );
		m_pSWTree = new QuadTree( m_fX, m_fZ + m_fHeight / 2.0f, m_fWidth / 2.0f, m_fHeight / 2.0f, m_iLevel+1 );
		m_pSETree = new QuadTree( m_fX + m_fWidth / 2.0f, m_fZ + m_fHeight / 2.0f, m_fWidth / 2.0f, m_fHeight / 2.0f, m_iLevel+1 );

		std::vector<Node*> templist = std::vector<Node*>();
		
		while (!m_lNodes.empty())
		{
			Node *node = m_lNodes.back();
			m_lNodes.pop_back();
			
			if ( Contains( m_pNWTree, node ) ) 
			{
				m_pNWTree->AddNode( node ); 
			}
			else if ( Contains( m_pNETree, node ) ) 
			{
				m_pNETree->AddNode( node ); 
			}
			else if ( Contains( m_pSWTree, node ) ) 
			{
				m_pSWTree->AddNode( node ); 
			}
			else if ( Contains( m_pSETree, node ) ) 
			{
				m_pSETree->AddNode( node ); 
			}
			// Add to parent tree if nodes are not completely inside one tree 
			else
			{
				templist.push_back(node);
			}
		}
		for(int i=0;i<templist.size();i++)
		{
			Node *node = templist.at(i);
			m_lNodes.push_back(node);
		}
		templist.clear();
	}
}

bool QuadTree::Contains( QuadTree *p_pChildTree, Node *p_pNode )
{
	float radius = p_pNode->GetBVRadius();
	glm::vec3 translation = p_pNode->GetWorldTranslation();
	float x= translation.x;
	float z = translation.z;

	return	 (  x + 0.5> p_pChildTree->GetX() &&
				z + 0.5> p_pChildTree->GetZ() &&
				x + 0.5< p_pChildTree->GetX() + p_pChildTree->GetWidth()  &&
				z + 0.5< p_pChildTree->GetZ() + p_pChildTree->GetHeight() 
				);
}

bool QuadTree::StopCritiria()
{
	if (m_iLevel > 10)
		return true;
	return (m_lNodes.size()<3) ;
}

void QuadTree::Render(const glm::mat4& mProj, const glm::mat4& mView)
{
	// Draw lines
	glm::mat4 mWorld = glm::mat4(glm::translate(0,0,0));

    m_pDecl->Bind();

    s_pMaterial->SetUniform("projection", mProj);
    s_pMaterial->SetUniform("view", mView);
    s_pMaterial->SetUniform("world", mWorld);    
    
	s_pMaterial->Apply();

    // Draw!
	glDrawArrays(GL_LINES,0,8);

	if (m_pNWTree != NULL)
		m_pNWTree->Render(mProj, mView);
	if (m_pNETree != NULL)
		m_pNETree->Render(mProj, mView);
	if (m_pSWTree != NULL)
		m_pSWTree->Render(mProj, mView);
	if (m_pSETree != NULL)
		m_pSETree->Render(mProj, mView);
}