#pragma once
#ifndef EMITTER_H
#define EMITTER_H

#include "Node.h"

class Emitter : public Node
{
public:
	Emitter(int p_iID,const glm::vec3 &p_vPos):Node(p_iID, p_vPos){}
	~Emitter();

	void Update(float p_fDelta);
	void Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);
};

#endif

