#include "Frustum.h"
#include <math.h>

#define ANG2RAD 3.141592653589/180.0


void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD)
{
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (float)tan(angle* ANG2RAD * 0.5) ;
	nh = nearD * tang;
	nw = nh * ratio; 
	fh = farD  * tang;
	fw = fh * ratio;
}


void Frustum::setCamDef(vec3 &p, vec3 &l, vec3 &u) 
{
	vec3 dir,nc,fc,X,Y,Z;

	Z = p - l;
	Z = glm::normalize(Z);

	//X = u * Z;
	X = glm::cross(u,Z);
	X = glm::normalize(X);

	//Y = Z * X;
	Y = glm::cross(Z,X);

	nc = p - Z * nearD;
	fc = p - Z * farD;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	pl[TOP].set3Points(ntr,ntl,ftl);
	pl[BOTTOM].set3Points(nbl,nbr,fbr);
	pl[LEFT].set3Points(ntl,nbl,fbl);
	pl[RIGHT].set3Points(nbr,ntr,fbr);
	pl[NEARP].set3Points(ntl,ntr,nbr);
	pl[FARP].set3Points(ftr,ftl,fbl);
}


int Frustum::pointInFrustum(vec3 &p) 
{
	int result = INSIDE;
	for(int i=0; i < 6; i++) {

		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return(result);
}


int Frustum::sphereInFrustum(const vec3 &p, float raio) {

	int result = INSIDE;
	float distance;

	for(int i=0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -raio)
			return OUTSIDE;
		else if (distance < raio)
			result =  INTERSECT;
	}
	return(result);

}

int Frustum::QuadTreeInFrustum(QuadTree *p_pQuadTree)
{
	AABB *aabb = p_pQuadTree->GetAABB();

	int result = INSIDE;
	for(int i=0; i < 6; i++) {

		if (pl[i].distance(aabb->getVertexP(pl[i].normal)) < 0)
			return OUTSIDE;
		else if (pl[i].distance(aabb->getVertexN(pl[i].normal)) < 0)
			result =  INTERSECT;
	}
	return(result);

}