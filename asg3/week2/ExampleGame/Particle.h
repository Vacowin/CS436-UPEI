#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Node.h"
#include "W_VertexDeclaration.h"

class Particle : public Node
{
public:
	Particle(int p_iID, const glm::vec3 &p_vPos);
	~Particle();

	void Update(float m_pDelta);
	void Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);

	Vertex* GetVertices();
	void SetVelocity(const glm::vec3& vel) { m_vVelocity = glm::vec3(vel);}

	Particle* GetNext() { return m_pNext;}
	void SetNext(Particle* p) { m_pNext = p;}
	Particle* GetPre() { return m_pPre;}
	void SetPre(Particle* p) { m_pPre = p;}

private:

	Vertex m_vertices[6];

	glm::vec3 m_vVelocity;
	float m_fSize;

	wolf::VertexDeclaration* m_pDecl ;
	static Material *s_pMaterial;

	Particle* m_pNext;
	Particle* m_pPre;
};

#endif
