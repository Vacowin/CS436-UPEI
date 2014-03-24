#pragma once
#ifndef EMITTER_H
#define EMITTER_H

#include "Node.h"
#include "Particle.h"
#include "W_VertexBuffer.h"
#include "W_Texture.h"
#include "W_VertexDeclaration.h"
#include "FadeAffector.h"

class Emitter : public Node
{
	enum SpawnMode
	{
		CONTINUOUS,
		BURST
	};
	enum EmitterType
	{
		POINT,
		BOX,
		SPHERE
	};
public:
	Emitter(std::string p_sPath, const glm::vec3 &p_vPos);
	~Emitter();

	void LoadXML(const std::string& p_sXMLPath);
	void Update(float p_fDelta);
	void Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);

	void KillParticle(Particle* p);
private:
	void Init();
	void UpdateSpawning(float p_fDelta);

	void AddToPool(Particle *p);
	void AddToActive(Particle *p);
	void RemoveFromActive(Particle *p);
	Particle* GetFreeParticle();
	void SpawnParticle();
	void CalculateBurstTime();

	float RandomRange(float fMin, float fMax) { return fMin + (float)rand()/((float)RAND_MAX/(fMax-fMin)); }

	std::string m_sEmitterName; 
	std::string m_sTextureName;
	bool m_bActive;
	float m_fDuration;
	float m_fLifeTime;
	int m_iMaxNumParticles;
	SpawnMode m_eSpawnMode;

	float m_fVelocityAffectorChace;
	bool m_bRandomVelocity;
	glm::vec3 m_vVelocity;
	glm::vec3 m_vVelocityMin;
	glm::vec3 m_vVelocityMax;

	float m_fColorAffectorChace;
	bool m_bRandomColor;
	glm::vec4 m_vColor;
	glm::vec4 m_vColorStart;
	glm::vec4 m_vColorEnd;

	float m_fFaceAffectorChance;
	FadeMode m_eFadeMode;
	bool m_bRandomFadeSpawn;
	float m_fFade;
	float m_fFadeMin;
	float m_fFadeMax;

	float m_fScaleAffectorChance;
	float m_fScaleStart;
	float m_fScaleEnd;
	bool m_bRandomSize;
	float m_fSize;
	float m_fSizeMin;
	float m_fSizeMax;

	bool m_fRandomSpawnLifeTime;
	float m_fSpawnLifeTime;
	float m_fSpawnLifeTimeMin;
	float m_fSpawnLifeTimeMax;

	float m_fToSpawnAccumulator;
	bool m_bRandomBirthRate;
	float m_fBirthRate;
	float m_fBirthRateMin;
	float m_fBirthRateMax;

	bool m_bRandomBurstTime;
	float m_fInitBurstTime;
	float m_fBurstTime;
	float m_fBurstTimeMin;
	float m_fBurstTimeMax;
	bool m_bRandomBurstSpawns;
	int m_iBurstNumSpawns;
	int m_iBurstSpawnMin;
	int m_iBurstSpawnMax;
	bool m_bRepeatBurst;

	Particle* m_pFreeList;
	Particle* m_pActiveList;
	Particle* m_pTailActiveList;

	wolf::VertexDeclaration* m_pDecl;
	wolf::VertexBuffer* m_pVertexBuffer;
	wolf::Material *m_pMaterial;
	wolf::Texture* m_pTexture;
};

#endif

