#include "Particle.h"
#include "W_BufferManager.h"

Material *Particle::s_pMaterial = NULL;

Particle::Particle(int p_iID, const glm::vec3 &p_vPos) : Node(p_iID, p_vPos)
{
	m_pNext = nullptr;
	m_pPre = nullptr;
	m_fSize = 5.0f;

	Vertex cubeVertices[] = {
	// Front
		{ -5, -5, 5, 255, 0, 0, 255 },
		{ -5,  5, 5, 255, 0, 0, 255 },
		{  5,  5, 5, 255, 0, 0, 255 },
		{  5,  5, 5, 255, 0, 0, 255 },
		{  5, -5, 5, 255, 0, 0, 255 },
		{ -5, -5, 5, 255, 0, 0, 255 },
	};

	wolf::VertexBuffer* pVB = wolf::BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 6);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
	m_pDecl->SetVertexBuffer(pVB);
	m_pDecl->End();
	
	if (!s_pMaterial)
	{
		s_pMaterial = wolf::MaterialManager::CreateMaterial("debugcube");
		s_pMaterial->SetProgram("data/cube.vsh", "data/cube.fsh");
	}
}


Particle::~Particle(void)
{
}

Vertex* Particle::GetVertices()
{
	glm::mat4 mWorld = GetWorldTransform();

	glm::vec4 posArray[6];

	posArray[0] = glm::vec4(-0.5 * m_fSize, -0.5* m_fSize, 0.0, 1);
	posArray[1] = glm::vec4(-0.5 * m_fSize, 0.5* m_fSize, 0.0, 1);
	posArray[2] = glm::vec4(0.5 * m_fSize, 0.5* m_fSize, 0.0, 1);
	posArray[3] = glm::vec4(0.5 * m_fSize, 0.5* m_fSize, 0.0, 1);
	posArray[4] = glm::vec4(0.5 * m_fSize, -0.5* m_fSize, 0.0, 1);
	posArray[5] = glm::vec4(-0.5 * m_fSize, -0.5* m_fSize, 0.0, 1);

	glm::vec4 temp;
	for (int i=0;i<6;i++)
	{
		temp = mWorld * posArray[i];
		m_vertices[i].x = temp.x;
		m_vertices[i].y = temp.y;
		m_vertices[i].z = temp.z;
	}

	return m_vertices;
}

void Particle::Update(float m_pDelta)
{
	Translate(m_vVelocity*m_pDelta);
}

void Particle::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	/*
	glm::mat4 mWorld = glm::translate(GetTranslation());
	mWorld *= glm::mat4(glm::transpose(glm::mat3(p_mView)));
	m_pDecl->Bind();

    s_pMaterial->SetUniform("world", mWorld);
	s_pMaterial->SetUniform("projection", p_mProj);
	s_pMaterial->SetUniform("view", p_mView);

	s_pMaterial->Apply();

    // Draw!
	glDrawArrays(GL_TRIANGLES, 0, 6 );
	*/
}
