#pragma once
#ifndef ADDVELOCITYAFFECTOR_H
#define ADDVELOCITYAFFECTOR_H
#include "Affector.h"

class AddVelocityAffector : public Affector
{
public:
	AddVelocityAffector();
	~AddVelocityAffector();

	virtual void Apply(float p_fDelta, Particle* p);
};
#endif

