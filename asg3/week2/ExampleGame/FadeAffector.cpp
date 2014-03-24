#include "FadeAffector.h"


FadeAffector::FadeAffector(FadeMode mode)
{
	m_eMode = mode;
}


FadeAffector::~FadeAffector(void)
{
}

void FadeAffector::Apply(float p_fDelta, Particle* p)
{
	if(m_eMode == FadeIn)
	{
		p->SetFade(p->GetAge());
	}
	else if(m_eMode == FadeOut)
	{
		p->SetFade(0.95f - p->GetAge());
	}
}
