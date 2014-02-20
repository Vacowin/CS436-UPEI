#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"
#include "QuadTree.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "Octree.h"

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

	Frustum();
	~Frustum(){};

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(vec3 &p, vec3 &l, vec3 &u);
	int pointInFrustum(vec3 &p);
	int sphereInFrustum(const vec3 &p, float raio);
	int QuadTreeInFrustum(QuadTree *p_pQuadTree);
	int OcTreeInFrustum(OcTree *p_pOcTree);

	void Frustum::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);

	wolf::VertexDeclaration* m_pDecl ;
	wolf::VertexBuffer* m_pVB;
	static Material *s_pMaterial;
};


#endif