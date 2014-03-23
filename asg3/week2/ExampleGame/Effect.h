#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "Node.h"
#include "Emitter.h"
#include <vector>

class Effect : public Node
{
public:
	Effect(std::string p_sPath, const glm::vec3 &p_vPos);
	~Effect();

	void Update(float p_fDelta);
	void Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);

private:
	std::vector<Emitter*> m_emitterList;
	std::string m_sName;
};

#endif
