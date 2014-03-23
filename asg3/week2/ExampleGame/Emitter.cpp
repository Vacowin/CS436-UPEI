#include "Emitter.h"
#include "W_BufferManager.h"
#include "AddVelocityAffector.h"
#include "Affector.h"
#include "ScaleAffector.h"
#include "tinyxml\tinyxml.h"

Emitter::Emitter(std::string p_sPath, const glm::vec3 &p_vPos):Node(0, p_vPos)
{
	m_pActiveList = nullptr;
	m_pFreeList = nullptr;
	m_pTailActiveList = nullptr;

	/*
	m_fDuration = -1;
	m_fLifeTime = 0.0f;
	m_eSpawnMode = CONTINUOUS;
	m_iMaxNumParticles = 500;
	m_bRandomBirthRate = false;
	m_fToSpawnAccumulator = 0;
	m_fBirthRate = 150.0;

	m_fVelocityAffectorChace = 1.0f;
	m_bRandomVelocity = true;
	m_vVelocity = glm::vec3(0.0f,1.0f,0.0f);
	m_vVelocityMin = glm::vec3(-3.0f, 3.0f, -3.0f);
	m_vVelocityMax = glm::vec3(3.0f, 20.0f, 3.0f);

	m_fFaceAffectorChance = 1.0;
	m_eFadeMode = FadeOut;

	m_fScaleAffectorChance = 1.0f;
	m_fScaleStart = 5.0f;
	m_fScaleEnd = 10.0f;
	*/

	m_bRandomBirthRate = false;
	m_bRandomBurstSpawns = false;
	m_bRandomBurstTime = false;
	m_bRandomColor = false;
	m_bRandomFadeSpawn = false;
	m_bRandomSize = false;
	m_bRandomVelocity = false;

	m_sEmitterName ="";
	m_sTextureName ="";
	m_bActive = true;
	m_fDuration = 0;
	m_fLifeTime = 0;
	m_iMaxNumParticles = 0;
	m_eSpawnMode = CONTINUOUS;
	m_fVelocityAffectorChace = 0;
	m_vVelocity = glm::vec3(0,0,0);
	m_vVelocityMin = glm::vec3(0,0,0);
	m_vVelocityMax= glm::vec3(0,0,0);
	m_fColorAffectorChace = 0;
	m_vColor = glm::vec4(0,0,0,0);
	m_vColorStart = glm::vec4(0,0,0,0);
	m_vColorEnd = glm::vec4(0,0,0,0);
	m_fFaceAffectorChance = 0;
	m_eFadeMode = FadeIn;
	m_fFade = 0;
	m_fFadeMin = 0;
	m_fFadeMax = 0;
	m_fScaleAffectorChance = 0;
	m_fScaleStart = 0;
	m_fScaleEnd = 0;
	m_bRandomSize = 0;
	m_fSize = 0;
	m_fSizeMin = 0;
	m_fSizeMax = 0;
	m_fSpawnLifeTime = 0;
	m_fSpawnLifeTimeMin = 0;
	m_fSpawnLifeTimeMax = 0;
	m_fToSpawnAccumulator = 0;
	m_fBirthRate = 0;
	m_fBirthRateMin = 0;
	m_fBirthRateMax = 0;
	m_fBurstTime = 0;
	m_fBurstTimeMin = 0;
	m_fBurstTimeMax = 0;
	m_iBurstNumSpawns= 0;
	m_iBurstSpawnMin = 0;
	m_iBurstSpawnMax = 0;
	m_bRepeatBurst = false; 


	LoadXML(p_sPath);
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

	m_pMaterial = wolf::MaterialManager::CreateMaterial(m_sEmitterName);
	m_pMaterial->SetProgram("data/shader/cube.vsh", "data/shader/cube.fsh");

	m_pMaterial->SetDepthTest(true);
	m_pMaterial->SetDepthFunc(DepthFunc::DF_Always);
	//m_pMaterial->SetBlendEquation(BlendEquation::BE_Add);
	m_pMaterial->SetBlendMode(BlendMode::BM_SrcAlpha, BlendMode::BM_OneMinusSrcAlpha);
	m_pMaterial->SetBlend(true);

	m_pTexture = wolf::TextureManager::CreateTexture("data/texture/"+m_sTextureName);
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

	p->Reset();
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
		// Spawn color
		if (m_bRandomColor)
		{
			m_vColor.r = RandomRange(m_vColorStart.r, m_vColorEnd.r);
			m_vColor.g = RandomRange(m_vColorStart.g, m_vColorEnd.g);
			m_vColor.b = RandomRange(m_vColorStart.b, m_vColorEnd.b);
			m_vColor.a = 1.0f;
		}
		pParticle->SetColor(m_vColor);

		// Spawn size
		if (m_bRandomSize)
		{
			m_fSize = RandomRange(m_fSizeMin, m_fSizeMax);
		}
		pParticle->SetSize(m_fSize);

		// Spawn fade
		if (m_bRandomFadeSpawn)
		{
			m_fFade = RandomRange(m_fFadeMin, m_fFadeMax);
		}
		pParticle->SetFade(m_fFade);

		// Spawn life time
		if (m_fRandomSpawnLifeTime)
		{
			m_fSpawnLifeTime = RandomRange(m_fSpawnLifeTimeMin, m_fSpawnLifeTimeMax);
		}
		pParticle->SetLifeTime(m_fSpawnLifeTime);

		// Affector update time span of this particle
		pParticle->AddAffector(new Affector());

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
			pParticle->AddAffector(new AddVelocityAffector());
		}

		// Fade Affector
		if (ChanceOfAffector(m_fFaceAffectorChance))
		{
			pParticle->AddAffector(new FadeAffector(m_eFadeMode));
		}

		// Scale Affector
		if (ChanceOfAffector(m_fScaleAffectorChance))
		{
			pParticle->AddAffector(new ScaleAffector(m_fScaleStart, m_fScaleEnd));
		}
		// box, sphere emitter
		pParticle->SetTranslation(GetWorldTranslation());

		AddToActive(pParticle);
	}
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
	while(pCurrent)
	{
		pCurrent->Update(p_fDelta);
		pCurrent = pCurrent->GetNext();
		//if (m_pActiveList == m_pTailActiveList)
			//break;
	}
}

void Emitter::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	m_pVertexBuffer->Bind();

	// update vertex buffer
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
	
	// Billboard
	glm::mat4 mWorld = glm::mat4(glm::transpose(glm::mat3(p_mView)));

    m_pMaterial->SetUniform("world", mWorld);
	m_pMaterial->SetUniform("projection", p_mProj);
	m_pMaterial->SetUniform("view", p_mView);
	m_pMaterial->SetTexture("tex", m_pTexture);
	m_pMaterial->Apply();
	m_pDecl->Bind();
	// Draw all particles
	glDrawArrays(GL_TRIANGLES, 0, iNumVertices );
}

void Emitter::LoadXML(const std::string& p_sXMLPath)
{
	TiXmlDocument doc(p_sXMLPath.c_str());
	if (doc.LoadFile() == false)
	{
		return;
	}

	TiXmlNode* pRoot = doc.FirstChild("Emitter");
	if (pRoot == NULL)
	{
		return;
	}

	TiXmlElement* pElement = pRoot->ToElement();
	m_sEmitterName = pElement->Attribute("name");
	m_sTextureName = pElement->Attribute("texture");
	pElement->QueryIntAttribute("num_particles",&m_iMaxNumParticles);
	pElement->QueryFloatAttribute("duration",&m_fDuration);
	std::string type = pElement->Attribute("type");
	std::string mode = pElement->Attribute("mode");

	// Continuous mode
	if (mode.compare("continuous") == 0)
	{
		m_eSpawnMode = CONTINUOUS;
		m_bRandomBirthRate = type.compare("random")==0?true:false;
		if (m_bRandomBirthRate)
		{
			pElement->QueryFloatAttribute("birthRateMin",&m_fBirthRateMin);
			pElement->QueryFloatAttribute("birthRateMax",&m_fBirthRateMax);
		}
		else
		{
			pElement->QueryFloatAttribute("birthRate",&m_fBirthRate);
		}
	}
	// Burst mode
	else if (mode.compare("burst") == 0)
	{
		m_eSpawnMode = BURST;
		m_bRandomBurstSpawns = type.compare("random")==0?true:false;
		if (m_bRandomBurstSpawns)
		{
			pElement->QueryIntAttribute("burstSpawnMin",&m_iBurstSpawnMin);
			pElement->QueryIntAttribute("burstSpawnMax",&m_iBurstSpawnMax);
		}
		else
		{
			pElement->QueryIntAttribute("burstSpawnNum",&m_iBurstNumSpawns);
		}
		// Burst Repeat
		pElement->QueryBoolAttribute("burstRepeat", &m_bRepeatBurst);
		if (m_bRepeatBurst)
		{
			std::string repeatType = pElement->Attribute("burstRepeatType");
			m_bRandomBurstTime = type.compare("random")==0?true:false;
			if (m_bRandomBurstTime)
			{
				pElement->QueryFloatAttribute("burstRepeatTimeMin",&m_fBurstTimeMin);
				pElement->QueryFloatAttribute("burstRepeatTimeMax",&m_fBurstTimeMax);
			}
			else
			{
				pElement->QueryFloatAttribute("burstRepeatTime",&m_fBurstTime);
			}
		}
	}

	// Traverse nodes
	TiXmlNode* pChildNode = pRoot->FirstChild();
	while (pChildNode != NULL )
	{
		const char* szNodeName = pChildNode->Value();
		// Spawn properties
		if (strcmp(szNodeName, "SpawnProperty") == 0)
		{
			pElement = pChildNode->ToElement();
			std::string sName = pElement->Attribute("name");
			std::string sType = pElement->Attribute("type");
			// Velocity
			if (sName.compare("velocity") == 0)
			{
				m_bRandomVelocity = sType.compare("random") == 0?true:false;
				if (m_bRandomVelocity)
				{
					pElement->QueryFloatAttribute("minX", &m_vVelocityMin.x);
					pElement->QueryFloatAttribute("minY", &m_vVelocityMin.y);
					pElement->QueryFloatAttribute("minZ", &m_vVelocityMin.z);

					pElement->QueryFloatAttribute("maxX", &m_vVelocityMax.x);
					pElement->QueryFloatAttribute("maxY", &m_vVelocityMax.y);
					pElement->QueryFloatAttribute("maxZ", &m_vVelocityMax.z);
				}
				else
				{
					pElement->QueryFloatAttribute("x", &m_vVelocity.x);
					pElement->QueryFloatAttribute("y", &m_vVelocity.y);
					pElement->QueryFloatAttribute("z", &m_vVelocity.z);
				}
			}
			// Color
			else if (sName.compare("color") == 0)
			{
				m_bRandomColor = sType.compare("random") == 0?true:false;
				if (m_bRandomColor)
				{
					float startR,startG,startB,endR,endG,endB;
					pElement->QueryFloatAttribute("startR", &startR);
					pElement->QueryFloatAttribute("startG", &startG);
					pElement->QueryFloatAttribute("startB", &startB);

					pElement->QueryFloatAttribute("endR", &endR);
					pElement->QueryFloatAttribute("endG", &endG);
					pElement->QueryFloatAttribute("endB", &endB);

					m_vColorStart = glm::vec4(startR, startG, startB, 1.0f);
					m_vColorEnd = glm::vec4(endR, endG, endB, 1.0f);
				}
				else
				{
					float fR,fG,fB;
					pElement->QueryFloatAttribute("r", &fR);
					pElement->QueryFloatAttribute("g", &fG);
					pElement->QueryFloatAttribute("b", &fB);
					m_vColor = glm::vec4(fR, fG, fB, 1.0f);
				}
			}
			// Size
			else if (sName.compare("size") == 0)
			{
				m_bRandomSize = sType.compare("random") == 0?true:false;
				if (m_bRandomSize)
				{
					pElement->QueryFloatAttribute("min", &m_fSizeMin);
					pElement->QueryFloatAttribute("max", &m_fSizeMax);
				}
				else
				{
					pElement->QueryFloatAttribute("value", &m_fSize);
				}
			}
			// Fade
			else if (sName.compare("fade") == 0)
			{
				m_bRandomFadeSpawn = sType.compare("random") == 0?true:false;
				if (m_bRandomFadeSpawn)
				{
					pElement->QueryFloatAttribute("min", &m_fFadeMin);
					pElement->QueryFloatAttribute("max", &m_fFadeMax);
				}
				else
				{
					pElement->QueryFloatAttribute("value", &m_fFade);
				}
			}
			// LifeTime
			else if (sName.compare("lifeTime") == 0)
			{
				m_fRandomSpawnLifeTime = sType.compare("random") == 0?true:false;
				if (m_fRandomSpawnLifeTime)
				{
					pElement->QueryFloatAttribute("min", &m_fSpawnLifeTimeMin);
					pElement->QueryFloatAttribute("max", &m_fSpawnLifeTimeMax);
				}
				else
				{
					pElement->QueryFloatAttribute("value", &m_fSpawnLifeTime);
				}
			}
		}

		// Afectors
		else if (strcmp(szNodeName, "Affector") == 0)
		{
			pElement = pChildNode->ToElement();
			std::string sType = pElement->Attribute("type");

			// Velocity Affector
			if (sType.compare("velocity") == 0)
			{
				pElement->QueryFloatAttribute("chance", &m_fVelocityAffectorChace);
			}
			// Scale Affector
			else if (sType.compare("scale") == 0)
			{
				pElement->QueryFloatAttribute("chance", &m_fScaleAffectorChance);
				pElement->QueryFloatAttribute("start", &m_fScaleStart);
				pElement->QueryFloatAttribute("end", &m_fScaleEnd);
			}
			// Fade Affector
			else if (sType.compare("fade") == 0)
 			{
				pElement->QueryFloatAttribute("chance", &m_fFaceAffectorChance);
				std::string sMode = pElement->Attribute("mode");
				if (sMode.compare("In") == 0)
					m_eFadeMode = FadeIn;
				else if (sMode.compare("Out") == 0)
					m_eFadeMode = FadeOut;
			}
		}

		pChildNode = pChildNode->NextSibling();
	}

}