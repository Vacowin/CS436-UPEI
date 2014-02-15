#include "Node.h"


Node::Node(const glm::vec3 &p_vPos)
{
	SetTranslation(p_vPos);
	m_pParent = NULL;
	m_lChildren = vector<Node*>();
}


const glm::vec3 Node::GetWorldTranslation()
{
	glm::vec3  pos = m_Transform.GetTranslation();
	if (m_pParent)
	{
		return pos += m_pParent->GetWorldTranslation();
	}
	return pos;

}

const glm::vec3 Node::GetWorldScale()
{
	glm::vec3 scale = GetScale();
	if (m_pParent)
	{
		scale *= m_pParent->GetWorldScale();
	}
	return scale;
}

const glm::vec3 Node::GetWorldRotation()
{
	glm::vec3 rotation = GetRotation();
	if (m_pParent)
	{
		rotation += m_pParent->GetWorldRotation();
	}
	return rotation;
}

const glm::mat4 Node::GetWorldTransform()
{
	glm::mat4 transform = GetTransform();
	if (m_pParent)
	{
		transform *= m_pParent->GetWorldTransform();
	}
	return transform;
}

void Node::Update(float p_fDelta)
{
	for (int i=0;i<m_lChildren.size();i++)
	{
		Node *kid = m_lChildren.at(i);
		glm::vec3 kidPos = kid->GetTranslation();;
	
		float newRadius = glm::length(kidPos) + kid->GetBVRadius(); 

		if (newRadius > m_fBVRadius)
			m_fBVRadius = newRadius;
	}

	for (int i=0;i<m_lChildren.size();i++)
	{
		Node *kid = m_lChildren.at(i);
		kid->Update(p_fDelta);
	}
}