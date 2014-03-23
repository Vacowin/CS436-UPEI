#include "Particle.h"
#include "Affector.h"

Particle::Particle(int p_iID, const glm::vec3 &p_vPos) : Node(p_iID, p_vPos)
{
	m_pNext = nullptr;
	m_pPre = nullptr;
	m_fSize = 5.0f;
	m_vColor = glm::vec4(1.0f,0.7f,0.1f,1.0f);
	m_fFade = 1.0f;
	m_fAge = 0.0f;
	m_fLifeTime = 2.0f;

	/*
	Vertex cubeVertices[] = {
	// Front
		{ -5, -5, 5, 255, 0, 0, 255, 0 ,0 },
		{ -5,  5, 5, 255, 0, 0, 255, 0, 1 },
		{  5,  5, 5, 255, 0, 0, 255, 1, 1 },
		{  5,  5, 5, 255, 0, 0, 255, 1 ,1 },
		{  5, -5, 5, 255, 0, 0, 255, 1, 0 },
		{ -5, -5, 5, 255, 0, 0, 255, 0, 0 },
	};

	wolf::VertexBuffer* pVB = wolf::BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 6);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
	m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	m_pDecl->SetVertexBuffer(pVB);
	m_pDecl->End();
	
	//if (!s_pMaterial)
	{
		s_pMaterial = wolf::MaterialManager::CreateMaterial("debugcube");
		s_pMaterial->SetProgram("data/cube.vsh", "data/cube.fsh");
	}

	m_pTexture = wolf::TextureManager::CreateTexture("data/glow.tga");
	*/

	m_vertices[0].u = 0.0f; m_vertices[0].v = 0.0f;
	m_vertices[1].u = 0.0f; m_vertices[1].v = 1.0f;
	m_vertices[2].u = 1.0f; m_vertices[2].v = 1.0f;
	m_vertices[3].u = 1.0f; m_vertices[3].v = 1.0f;
	m_vertices[4].u = 1.0f; m_vertices[4].v = 0.0f;
	m_vertices[5].u = 0.0f; m_vertices[5].v = 0.0f;
	
}


Particle::~Particle(void)
{
}

void Particle::Reset()
{
	m_fAge = 0.0f;
	m_fFade = 1.0f;
	m_lAffectors.clear();
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
		m_vertices[i].r = m_vColor.r;
		m_vertices[i].g = m_vColor.g;
		m_vertices[i].b = m_vColor.b;
		if (m_iID == 1)
			printf("%f\n", m_fFade);
		m_vertices[i].a = (m_vColor.a * m_fFade);
	}

	return m_vertices;
}

void Particle::Update(float p_pDelta)
{
	//Translate(m_vVelocity*m_pDelta);
	//Rotate(glm::vec3(500,500,500)*m_pDelta);

	for (int i = 0; i< m_lAffectors.size(); i++)
	{
		m_lAffectors.at(i)->Apply(p_pDelta, this);
	}
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
	s_pMaterial->SetTexture("tex", m_pTexture);
	s_pMaterial->SetUniform("color", glm::vec4(0,255,0,255));
	s_pMaterial->Apply();

    // Draw!
	glDrawArrays(GL_TRIANGLES, 0, 6 );
	*/
}
