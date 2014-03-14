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

	void Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);

private:

	wolf::VertexDeclaration* m_pDecl ;
	static Material *s_pMaterial;
};

#endif
