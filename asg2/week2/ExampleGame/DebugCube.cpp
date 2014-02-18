#include "DebugCube.h"
#include <ctime>


DebugCube::DebugCube(int p_iID, const glm::vec3 &p_vPos, glm::vec3 p_vDim) : Node(p_iID, p_vPos)
{
	m_fBVRadius = glm::length(p_vDim*0.5f);

	float x = p_vDim.x/2.0;
	float y = p_vDim.y/2.0;
	float z = p_vDim.z/2.0;

	Vertex cubeVertices[] = {
	// Front
		{ -x, -y, z, 255, 0, 0, 255 },
		{ -x,  y, z, 255, 0, 0, 255 },
		{  x,  y, z, 255, 0, 0, 255 },
		{  x,  y, z, 255, 0, 0, 255 },
		{  x, -y, z, 255, 0, 0, 255 },
		{ -x, -y, z, 255, 0, 0, 255 },

		// Back
		{  x,  y,-z, 128, 0, 0, 255 },
		{ -x,  y,-z, 128, 0, 0, 255 },
		{ -x, -y,-z, 128, 0, 0, 255 },
		{ -x, -y,-z, 128, 0, 0, 255 },
		{  x, -y,-z, 128, 0, 0, 255 },
		{  x,  y,-z, 128, 0, 0, 255 },

		// Left
		{ -x,  y,-z, 0, 255, 0, 255 },
		{ -x,  y, z, 0, 255, 0, 255 },
		{ -x, -y, z, 0, 255, 0, 255 },
		{ -x, -y, z, 0, 255, 0, 255 },
		{ -x, -y,-z, 0, 255, 0, 255 },
		{ -x,  y,-z, 0, 255, 0, 255 },

		// Right
		{  x,  y, z, 0, 128, 0, 255 },
		{  x,  y,-z, 0, 128, 0, 255 },
		{  x, -y,-z, 0, 128, 0, 255 },
		{  x, -y,-z, 0, 128, 0, 255 },
		{  x, -y, z, 0, 128, 0, 255 },
		{  x,  y, z, 0, 128, 0, 255 },

		// Top
		{ -x,  y, z, 0, 0, 255, 255 },
		{ -x,  y,-z, 0, 0, 255, 255 },
		{  x,  y,-z, 0, 0, 255, 255 },
		{  x,  y,-z, 0, 0, 255, 255 },
		{  x,  y, z, 0, 0, 255, 255 },
		{ -x,  y, z, 0, 0, 255, 255 },

		// Bottom
		{ -x, -y, z, 0, 0, 128, 255 },
		{  x, -y, z, 0, 0, 128, 255 },
		{  x, -y,-z, 0, 0, 128, 255 },
		{  x, -y,-z, 0, 0, 128, 255 },
		{ -x, -y,-z, 0, 0, 128, 255 },
		{ -x, -y, z, 0, 0, 128, 255 },
	};


	wolf::VertexBuffer* pVB = wolf::BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 6 * 3 * 2);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
	m_pDecl->SetVertexBuffer(pVB);
	m_pDecl->End();
	
	long double ran1 = rand();
	long double ran2 = rand();
	m_pMaterial = wolf::MaterialManager::CreateMaterial(std::to_string((long double)m_iID));
	m_pMaterial->SetProgram("data/cube.vsh", "data/cube.fsh");

}


void DebugCube::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	glm::mat4 mWorld = glm::mat4(glm::translate(GetWorldTranslation()));

	m_pDecl->Bind();

    m_pMaterial->SetUniform("world", mWorld);
	m_pMaterial->SetUniform("projection", p_mProj);
	m_pMaterial->SetUniform("view", p_mView);

	m_pMaterial->Apply();

    // Draw!
	glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2);


	for (int i=0;i<m_lChildren.size();i++)
	{
		Node *node = m_lChildren.at(i);
		node->Render(p_mView, p_mProj);
	}
}