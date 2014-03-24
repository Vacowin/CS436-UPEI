#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Node.h"

class Emitter;
class Affector;

class Particle : public Node
{
public:
	Particle();
	~Particle();

	void Reset();
	void Update(float m_pDelta);

	Vertex* GetVertices();

	void SetEmitter(Emitter* p_pEmitter) { m_pEmitter = p_pEmitter;}
	Emitter* GetEmitter() { return m_pEmitter;}
	
	void AddAffector(Affector* p_pAffector) { m_lAffectors.push_back(p_pAffector);}

	void SetVelocity(const glm::vec3& vel) { m_vVelocity = glm::vec3(vel);}
	const glm::vec3& GetVelocity() { return m_vVelocity;}
	void SetColor(const glm::vec4& value) { m_vColor = glm::vec4(value);}
	void SetSize(float value) { m_fSize = value;}
	void SetScale(float value) { m_fScale = value;}
	void SetFade(float value) { m_fFade = value;}
	void SetAge(float value) { m_fAge = value;}
	float GetAge() { return m_fAge;}
	void SetLifeTime(float value) { m_fLifeTime = value;} 
	float GetLifeTime() { return m_fLifeTime;}

	Particle* GetNext() { return m_pNext;}
	void SetNext(Particle* p) { m_pNext = p;}
	Particle* GetPre() { return m_pPre;}
	void SetPre(Particle* p) { m_pPre = p;}

private:

	Vertex m_vertices[6];

	glm::vec3 m_vVelocity;
	glm::vec4 m_vColor;
	float m_fSize;
	float m_fScale;
	float m_fFade;
	float m_fAge;
	float m_fLifeTime;

	Emitter* m_pEmitter;

	vector<Affector*> m_lAffectors;

	Particle* m_pNext;
	Particle* m_pPre;
};

#endif
