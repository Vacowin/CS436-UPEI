#include "OcTree.h"



OcTree::OcTree( float x, float y, float z, float width, float height,float depth, int level )
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
	m_fWidth = width;
	m_fHeight = height;
	m_fDepth = depth;
	m_iLevel = level;

	m_bHasChildren = false;

	m_pAABB = new AABB(glm::vec3(x,y,z), glm::vec3(width,height,depth));

	const Vertex lineVertices[] = {

		{ x, y, z, 255, 0, 0, 255 },
		{ x+width, y, z, 255, 0, 0, 255 },

		{ x+width, y, z, 255, 0, 0, 255 },
		{ x+width, y+height, z, 255, 0, 0, 255 },

		{ x+width, y+height, z, 255, 0, 0, 255 },
		{ x, y+height, z, 255, 0, 0, 255 },

		{ x, y+height, z, 255, 0, 0, 255 },
		{ x, y, z, 255, 0, 0, 255 },


		{ x, y, z+depth, 255, 0, 0, 255 },
		{ x+width, y, z+depth, 255, 0, 0, 255 },

		{ x+width, y, z+depth, 255, 0, 0, 255 },
		{ x+width, y+height, z+depth, 255, 0, 0, 255 },

		{ x+width, y+height, z+depth, 255, 0, 0, 255 },
		{ x, y+height, z+depth, 255, 0, 0, 255 },

		{ x, y+height, z+depth, 255, 0, 0, 255 },
		{ x, y, z+depth, 255, 0, 0, 255 },


		{ x, y, z, 255, 0, 0, 255 },
		{ x, y, z+depth, 255, 0, 0, 255 },

		{ x+width, y, z, 255, 0, 0, 255 },
		{ x+width, y, z+depth, 255, 0, 0, 255 },

		{ x, y+height, z, 255, 0, 0, 255 },
		{ x, y+height, z+depth, 255, 0, 0, 255 },

		{ x+width, y+height, z, 255, 0, 0, 255 },
		{ x+width, y+height, z+depth, 255, 0, 0, 255 },
	};

	m_pProgram = wolf::ProgramManager::CreateProgram("data/cube.vsh", "data/cube.fsh");
    m_pVB = wolf::BufferManager::CreateVertexBuffer(lineVertices, sizeof(Vertex) * 8*3);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();
}

void OcTree::AddNode( Node *p_pNode )
{
	if (!m_bHasChildren)
		m_lNodes.push_back(p_pNode);
	else
	{
		int i = 0;
		for (i = 0; i < 8; i++)
		{
			if ( Contains(&m_children.at(i), p_pNode) )
			{
				m_children.at(i).AddNode(p_pNode);
				break;
			}
		}
		
		// Add to parent tree if nodes are not completely inside one tree 
		if ( i == 8)
		{
			m_lNodes.push_back(p_pNode);
		}
	}
	
	//  check stop critiria  
	if (!StopCritiria())
	{		
		m_bHasChildren = true;
		m_children.push_back(OcTree( m_fX, m_fY, m_fZ, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));
		m_children.push_back(OcTree( m_fX + m_fWidth/2.0f, m_fY, m_fZ, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));
		m_children.push_back(OcTree( m_fX + m_fWidth/2.0f, m_fY, m_fZ+m_fDepth/2.0f, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));
		m_children.push_back(OcTree( m_fX, m_fY, m_fZ+m_fDepth/2.0f, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));

		m_children.push_back(OcTree( m_fX, m_fY+m_fHeight/2.0f, m_fZ, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));
		m_children.push_back(OcTree( m_fX + m_fWidth/2.0f, m_fY+m_fHeight/2.0f, m_fZ, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));
		m_children.push_back(OcTree( m_fX + m_fWidth/2.0f, m_fY+m_fHeight/2.0f, m_fZ+m_fDepth/2.0f, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));
		m_children.push_back(OcTree( m_fX, m_fY+m_fHeight/2.0f, m_fZ+m_fDepth/2.0f, m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0, m_iLevel+1 ));

		std::vector<Node*> templist = std::vector<Node*>();
		
		while (!m_lNodes.empty())
		{
			Node *node = m_lNodes.back();
			m_lNodes.pop_back();
			
			int j = 0;
			for (j = 0; j < 8; j++)
			{
				
				if ( Contains(&m_children.at(j), node) )
				{
					m_children.at(j).AddNode(node);
					break;
				}
			}
			// Add to parent tree if nodes are not completely inside one tree 
			if (j == 8)
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

bool OcTree::StopCritiria()
{
	return (m_lNodes.size()<5);
}

bool OcTree::Contains( OcTree *p_pChildTree, Node *p_pNode )
{
	float radius = p_pNode->GetBVRadius();
	glm::vec3 translation = p_pNode->GetWorldTranslation();
	float x = translation.x;
	float y = translation.y;
	float z = translation.z;

	return	 (  x > p_pChildTree->GetX() &&
				y > p_pChildTree->GetY() &&
				z > p_pChildTree->GetZ() &&
				x < p_pChildTree->GetX() + p_pChildTree->GetWidth()  &&
				y < p_pChildTree->GetY() + p_pChildTree->GetHeight()  &&
				z < p_pChildTree->GetZ() + p_pChildTree->GetDepth() 
				);
}

void OcTree::Render(const glm::mat4& mProj, const glm::mat4& mView)
{
	// Draw lines
	glm::mat4 mWorld = glm::mat4(glm::translate(0,0,0));

	m_pProgram->Bind();
    
    m_pProgram->SetUniform("projection", mProj);
    m_pProgram->SetUniform("view", mView);
    m_pProgram->SetUniform("world", mWorld);    
    
	m_pDecl->Bind();

    // Draw!
	glDrawArrays(GL_LINES,0,8*3);

	for (int i = 0 ; i < m_children.size(); i++)
	{
		m_children.at(i).Render(mProj, mView);
	}
}