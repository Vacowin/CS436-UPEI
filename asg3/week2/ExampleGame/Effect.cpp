#include "Effect.h"


Effect::Effect(int p_iID, const glm::vec3 &p_vPos) : Node(p_iID, p_vPos)
{
}

Effect::~Effect(void)
{
}

void Effect::Update(float p_fDelta)
{
	std::vector<Emitter*>::iterator it;
	for(it = m_emitterList.begin(); it != m_emitterList.end(); ++it) 
	{
		static_cast<Emitter*>(*it)->Update(p_fDelta);
	}
}

void Effect::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	std::vector<Emitter*>::iterator it;
	for(it = m_emitterList.begin(); it != m_emitterList.end(); ++it) 
	{
		static_cast<Emitter*>(*it)->Render(p_mView, p_mProj);
	}
}
