#pragma once
#ifndef FADEAFFECTOR_H
#define FADEAFFECTOR_H
#include "Affector.h"

enum FadeMode
	{
		FadeIn = 0,
		FadeOut
	};

class FadeAffector : public Affector
{
public:
	FadeAffector(FadeMode mode);
	~FadeAffector();

	virtual void Apply(float p_fDelta, Particle* p);

private:
	FadeMode m_eMode;
};
#endif

