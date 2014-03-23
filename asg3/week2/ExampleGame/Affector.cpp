#include "Affector.h"
#include "Emitter.h"
#include "Particle.h"

Affector::Affector(void)
{
}


Affector::~Affector(void)
{
}

void Affector::Apply(float p_fDelta, Particle* p)
{
	p->SetAge(p->GetAge() + p_fDelta*(1.0f/p->GetLifeTime()));
	if (p->GetAge() > 1.0f)
		p->GetEmitter()->KillParticle(p);
}