#include "Emitter.h"
#include "W_BufferManager.h"
#include "AddVelocityAffector.h"
#include "Affector.h"
#include "Scene.h"

Emitter::Emitter(int p_iID,const glm::vec3 &p_vPos):Node(p_iID, p_vPos)
{
	m_pActiveList = nullptr;
	m_pFreeList = nullptr;
	m_pTailActiveList = nullptr;

	m_fDuration = -1;
	m_fLifeTime = 0.0f;
	m_eSpawnMode = CONTINUOUS;
	m_iMaxNumParticles = 1000;
	m_bRandomBirthRate = false;
	m_fToSpawnAccumulator = 0;
	m_fBirthRate = 200.0;

	m_fVelocityAffectorChace = 1.0f;
	m_bRandomVelocity = true;
	m_vVelocity = glm::vec3(0.0f,1.0f,0.0f);
	m_vVelocityMin = -glm::vec3(5.0f, 0.0f, 5.0f);
	m_vVelocityMax = glm::vec3(5.0f, 35.0f, 5.0f);

	m_fFaceAffectorChance = 1.0;
	m_bRandomFade = false;
	m_eFadeMode = FadeOut;

	Init();
}

Emitter::~Emitter()
{
}

void Emitter::Init()
{
	for (int i = 0; i < m_iMaxNumParticles; i++)
	{
		Particle* pParticle = new Particle(i, glm::vec3(0.0f,0.0f,0.0f));
		pParticle->SetEmitter(this);
		
		AddToPool(pParticle);
	}

	CalculateBurstTime();
	
	Vertex * vertices = new Vertex[m_iMaxNumParticles*6];
	
	m_pVertexBuffer = wolf::BufferManager::CreateDynamicVertexBuffer(vertices, sizeof(Vertex) * 6 * m_iMaxNumParticles);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	m_pDecl->SetVertexBuffer(m_pVertexBuffer);
	m_pDecl->End();

	m_pMaterial = wolf::MaterialManager::CreateMaterial("emitter");
	m_pMaterial->SetProgram("data/cube.vsh", "data/cube.fsh");

	m_pMaterial->SetDepthTest(true);
	m_pMaterial->SetDepthFunc(DepthFunc::DF_Always);
	//m_pMaterial->SetBlendEquation(BlendEquation::BE_Add);
	m_pMaterial->SetBlendMode(BlendMode::BM_SrcAlpha, BlendMode::BM_OneMinusSrcAlpha);
	m_pMaterial->SetBlend(true);

	m_pTexture = wolf::TextureManager::CreateTexture("data/glow3.tga");
}

bool ChanceOfAffector(float p_fRate)
{
	// random (0,1]
	float random = ((double) rand() / (RAND_MAX+1));
	return (p_fRate - random>=0?true:false);
}

void Emitter::AddToPool(Particle *p)
{
	p->SetPre(nullptr);
	p->SetNext(m_pFreeList);
	m_pFreeList = p;
}

void Emitter::AddToActive(Particle *p)
{
	p->SetPre(nullptr);
	p->SetNext(m_pActiveList);
	if (m_pActiveList)
		m_pActiveList->SetPre(p);
	m_pActiveList = p;

	if (!p->GetNext())
		m_pTailActiveList = p;
}

void Emitter::RemoveFromActive(Particle *p)
{
	if (m_pActiveList == p)
		m_pActiveList = p->GetNext();
	else
		p->GetPre()->SetNext(p->GetNext());

	if (m_pTailActiveList == p)
		m_pTailActiveList = p->GetPre();
	else
		p->GetNext()->SetPre(p->GetPre());

	p->SetNext(nullptr);
	p->SetPre(nullptr);
}

Particle* Emitter::GetFreeParticle()
{
	Particle *pRet = m_pFreeList;
	if (pRet)
	{
		m_pFreeList = m_pFreeList->GetNext();
		if (m_pFreeList)
			m_pFreeList->SetPre(nullptr);

		pRet->SetNext(nullptr);
		pRet->SetPre(nullptr);
		return pRet;
	}
	else 
		return nullptr;
}

void Emitter::KillParticle(Particle* p)
{
	RemoveFromActive(p);
	AddToPool(p);
}

void Emitter::CalculateBurstTime()
{
	if (m_bRandomBurstTime)
		m_fBurstTime = 1; // more
	else
		m_fBurstTime = 1;
}

void Emitter::SpawnParticle()
{
	Particle* pParticle = GetFreeParticle();
	if (!pParticle)
	{
		pParticle = m_pTailActiveList;
		RemoveFromActive(pParticle);
	}
	if (pParticle)
	{
		//pParticle->SetTranslation(GetTranslation());

		pParticle->Reset();
		pParticle->GetAffectors().push_back(new Affector());
		// Velocity Affector
		if (ChanceOfAffector(m_fVelocityAffectorChace))
		{
			if (m_bRandomVelocity)
			{
				m_vVelocity.x = RandomRange(m_vVelocityMin.x, m_vVelocityMax.x);
				m_vVelocity.y = RandomRange(m_vVelocityMin.y, m_vVelocityMax.y);
				m_vVelocity.z = RandomRange(m_vVelocityMin.z, m_vVelocityMax.z);
				pParticle->SetVelocity(m_vVelocity);
			}
			else
			{
				pParticle->SetVelocity(m_vVelocity);
			}
			pParticle->GetAffectors().push_back(new AddVelocityAffector());
		}

		if (ChanceOfAffector(m_fFaceAffectorChance))
		{
			if (m_bRandomFade)
			{
				m_eFadeMode = static_cast<FadeMode>(rand()%2);
			}
			pParticle->GetAffectors().push_back(new FadeAffector(m_eFadeMode));
		}

		// box, sphere emitter
		pParticle->SetTranslation(GetWorldTranslation());

		AddToActive(pParticle);
	}
	// more stuff
}

void Emitter::UpdateSpawning(float p_fDelta)
{
	int iNumSpawns;
	if(m_eSpawnMode == CONTINUOUS)
	{
		if(m_bRandomBirthRate)
			m_fBirthRate = 5;//randomRange(m_fBirthRateMin, m_fBirthRateMax);
		
		m_fToSpawnAccumulator += m_fBirthRate * p_fDelta;
		iNumSpawns = (int)m_fToSpawnAccumulator;
		m_fToSpawnAccumulator -= iNumSpawns;
	
		for(int i = 0; i < iNumSpawns; i++)
		{
			SpawnParticle();
		}
	}
	
	else if (m_eSpawnMode == BURST)
	{
		m_fBurstTime -=p_fDelta;
		if (m_fBurstTime <=0)
		{
			if (m_bRandomBurstSpawns)
				iNumSpawns = 10; // range
			else
				iNumSpawns = m_iBurstNumSpawns;

			if (m_bRepeatBurst)
				CalculateBurstTime();
			else
				m_bActive = false;
		}
	}

	for (int i=0;i<iNumSpawns;i++)
		SpawnParticle();
}

void Emitter::Update(float p_fDelta)
{
	m_fLifeTime += p_fDelta;
	if (m_fDuration < 0 || m_fLifeTime < m_fDuration)
		UpdateSpawning(p_fDelta);

	Particle* pCurrent = m_pActiveList;
	Particle* pNext = m_pActiveList;
	while(pCurrent)
	{
		pCurrent->Update(p_fDelta);
		pNext = pCurrent->GetNext();
		pCurrent = pNext;
	}
}

void Emitter::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	/*
	Particle* pCurrent = m_pActiveList;
	Particle* pNext = m_pActiveList;
	while(pCurrent)
	{
		pCurrent->Render(p_mView, p_mProj);
		pNext = pCurrent->GetNext();
		pCurrent = pNext;
	}
	*/

	m_pVertexBuffer->Bind();

	Vertex* pVertices = (Vertex*)m_pVertexBuffer->Lock(); 
	int iNumVertices = 0;
	Particle* pParticle = m_pActiveList;
	while(pParticle)
	{		
		Vertex* vParticleVerts = pParticle->GetVertices();

		for(int i = 0; i < 6; i++)
		{
			Vertex v = vParticleVerts[i];
			pVertices[iNumVertices + i] = vParticleVerts[i];
		}
		
		pParticle = pParticle->GetNext();	
		iNumVertices += 6;
	}
	m_pVertexBuffer->Unlock();
	
	glm::mat4 mWorld = glm::mat4(glm::transpose(glm::mat3(p_mView)));

    m_pMaterial->SetUniform("world", mWorld);
	m_pMaterial->SetUniform("projection", p_mProj);
	m_pMaterial->SetUniform("view", p_mView);
	//m_pMaterial->SetUniform("color", glm::vec4(1.0f,0.7f,0.1f,1.0f));
	m_pMaterial->SetTexture("tex", m_pTexture);
	m_pMaterial->Apply();
	m_pDecl->Bind();
	// Draw all particles
	glDrawArrays(GL_TRIANGLES, 0, iNumVertices );
}
