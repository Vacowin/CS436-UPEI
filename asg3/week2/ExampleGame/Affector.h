#pragma once
#ifndef AFFECTOR_H
#define AFFECTOR_H

#include "Particle.h"
 
class Affector
{
public:
	Affector(void);
	~Affector(void);

	virtual void Apply(float p_fDelta, Particle* p) ;
};
#endif