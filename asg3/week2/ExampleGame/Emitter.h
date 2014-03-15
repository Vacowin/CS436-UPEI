#pragma once
#ifndef EMITTER_H
#define EMITTER_H

#include "Node.h"
#include "Particle.h"
#include "W_VertexBuffer.h"

class Emitter : public Node
{
	enum SpawnMode
	{
		CONTINUOUS,
		BURST
	};
public:
	Emitter(int p_iID,const glm::vec3 &p_vPos);
	~Emitter();

	void Update(float p_fDelta);
	void Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);

private:
	void Init();
	void UpdateSpawning(float p_fDelta);

	void AddToPool(Particle *p);
	void AddToActive(Particle *p);
	void RemoveFromActive(Particle *p);
	Particle* GetFreeParticle();
	void SpawnParticle();
	void ParticleKilled(Particle* p);
	void CalculateBurstTime();

	float RandomRange(float fMin, float fMax) { return fMin + (float)rand()/((float)RAND_MAX/(fMax-fMin)); }

	bool m_bActive;
	float m_fDuration;
	float m_fLifeTime;
	int m_iMaxNumParticles;
	SpawnMode m_eSpawnMode;

	bool m_bRandomVelocity;
	glm::vec3 m_vVelocity;
	glm::vec3 m_vVelocityMin;
	glm::vec3 m_vVelocityMax;

	float m_fToSpawnAccumulator;
	bool m_bRandomBirthRate;
	float m_fBirthRate;
	float m_fBirthRateMin;
	float m_fBirthRateMax;

	bool m_bRandomBurstTime;
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
};

#endif

