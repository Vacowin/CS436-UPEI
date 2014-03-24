#include "Particle.h"
#include "Affector.h"

Particle::Particle() : Node()
{
	m_pNext = nullptr;
	m_pPre = nullptr;
	m_pEmitter = nullptr;

	m_vVelocity = glm::vec3(0,0,0);
	m_vColor = glm::vec4(0,0,0,0);
	m_fSize = 0;
	m_fScale = 0;
	m_fFade = 0;
	m_fAge = 0;
	m_fLifeTime = 0;

	// Texture UVs
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
	m_pNext = nullptr;
	m_pPre = nullptr;

	m_vVelocity = glm::vec3(0,0,0);
	m_vColor = glm::vec4(0,0,0,0);
	m_fSize = 0;
	m_fScale = 0;
	m_fFade = 0;
	m_fAge = 0;
	m_fLifeTime = 0;
	m_lAffectors.clear();
}

Vertex* Particle::GetVertices()
{
	glm::mat4 mWorld = GetWorldTransform();

	glm::vec4 posArray[6];

	posArray[0] = glm::vec4(-0.5 * m_fSize * m_fScale, -0.5* m_fSize * m_fScale, 0.0, 1);
	posArray[1] = glm::vec4(-0.5 * m_fSize * m_fScale, 0.5* m_fSize * m_fScale, 0.0, 1);
	posArray[2] = glm::vec4(0.5 * m_fSize * m_fScale, 0.5* m_fSize * m_fScale, 0.0, 1);
	posArray[3] = glm::vec4(0.5 * m_fSize * m_fScale, 0.5* m_fSize * m_fScale, 0.0, 1);
	posArray[4] = glm::vec4(0.5 * m_fSize * m_fScale, -0.5* m_fSize * m_fScale, 0.0, 1);
	posArray[5] = glm::vec4(-0.5 * m_fSize * m_fScale, -0.5* m_fSize * m_fScale, 0.0, 1);

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
		m_vertices[i].a = (m_vColor.a * m_fFade);
	}

	return m_vertices;
}

void Particle::Update(float p_pDelta)
{
	for (int i = 0; i< m_lAffectors.size(); i++)
	{
		m_lAffectors.at(i)->Apply(p_pDelta, this);
	}
}

