#pragma once
#ifndef SCALEAFFECTOR_H
#define SCALEAFFECTOR_H
#include "Affector.h"

class ScaleAffector : public Affector
{
public:
	ScaleAffector(float start, float end);
	~ScaleAffector();

	virtual void Apply(float p_fDelta, Particle* p);
private:
	float m_fScaleStart;
	float m_fScaleEnd;
};
#endif
