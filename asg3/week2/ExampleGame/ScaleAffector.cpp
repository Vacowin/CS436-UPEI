#include "ScaleAffector.h"


ScaleAffector::ScaleAffector(float start, float end)
{
	m_fScaleStart = start;
	m_fScaleEnd = end;
}


ScaleAffector::~ScaleAffector(void)
{
}

void ScaleAffector::Apply(float p_fDelta, Particle* p)
{
	p->SetScale(m_fScaleStart + (m_fScaleEnd - m_fScaleStart)*p->GetAge());
}