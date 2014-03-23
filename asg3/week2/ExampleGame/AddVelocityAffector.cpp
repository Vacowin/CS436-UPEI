#include "AddVelocityAffector.h"


AddVelocityAffector::AddVelocityAffector()
{
}


AddVelocityAffector::~AddVelocityAffector()
{
}

void AddVelocityAffector::Apply(float p_fDelta, Particle* p)
{
	glm::vec3 pos;
	if (p->GetID() == 1)
	{
		pos = p->GetWorldTranslation();
	}
	p->Translate(p_fDelta*p->GetVelocity());
	pos =p->GetWorldTranslation();
}