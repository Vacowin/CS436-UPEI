#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"
#include "QuadTree.h"

class Frustum
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

public:

	static enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];


	vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
	float nearD, farD, ratio, angle,tang;
	float nw,nh,fw,fh;

	Frustum(){};
	~Frustum(){};

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(vec3 &p, vec3 &l, vec3 &u);
	int pointInFrustum(vec3 &p);
	int sphereInFrustum(const vec3 &p, float raio);
	int QuadTreeInFrustum(QuadTree *p_pQuadTree);
};


#endif