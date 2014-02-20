#include "Frustum.h"
#include <math.h>

#define ANG2RAD 3.141592653589/180.0

Material *Frustum::s_pMaterial = NULL;

Frustum::Frustum()
{
	m_pDecl = NULL;
	m_pVB = NULL;
}

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

	Vertex cubeVertices[] = {
	// Far Plane
		{ fbl.x, fbl.y, fbl.z, 255, 255, 255, 70 },
		{ ftl.x, ftl.y, ftl.z, 255, 255, 255, 70 },
		{ ftr.x, ftr.y, ftr.z, 255, 255, 255, 70 },
		{ ftr.x, ftr.y, ftr.z, 255, 255, 255, 70 },
		{ fbr.x, fbr.y, fbr.z, 255, 255, 255, 70 },
		{ fbl.x, fbl.y, fbl.z, 255, 255, 255, 70 },

	// Top Plane
		{ ntl.x, ntl.y, ntl.z, 0, 255, 255, 70 },
		{ ftl.x, ftl.y, ftl.z, 0, 255, 255, 70 },
		{ ftr.x, ftr.y, ftr.z, 0, 255, 255, 70 },
		{ ftr.x, ftr.y, ftr.z, 0, 255, 255, 70 },
		{ ntr.x, ntr.y, ntr.z, 0, 255, 255, 70 },
		{ ntl.x, ntl.y, ntl.z, 0, 255, 255, 70 },

	// Bottom Plane
		{ nbl.x, nbl.y, nbl.z, 0, 255, 0, 70 },
		{ fbl.x, fbl.y, fbl.z, 0, 255, 0, 70 },
		{ fbr.x, fbr.y, fbr.z, 0, 255, 0, 70 },
		{ fbr.x, fbr.y, fbr.z, 0, 255, 0, 70 },
		{ nbr.x, nbr.y, nbr.z, 0, 255, 0, 70 },
		{ nbl.x, nbl.y, nbl.z, 0, 255, 0, 70 },

	// Left Plane
		{ nbl.x, nbl.y, nbl.z, 0, 0, 255, 70 },
		{ fbl.x, fbl.y, fbl.z, 0, 0, 255, 70 },
		{ ftl.x, ftl.y, ftl.z, 0, 0, 255, 70 },
		{ ftl.x, ftl.y, ftl.z, 0, 0, 255, 70 },
		{ ntl.x, ntl.y, ntl.z, 0, 0, 255, 70 },
		{ nbl.x, nbl.y, nbl.z, 0, 0, 255, 70 },

	// Right Plane
		{ nbr.x, nbr.y, nbr.z, 255, 255, 0, 70 },
		{ fbr.x, fbr.y, fbr.z, 255, 255, 0, 70 },
		{ ftr.x, ftr.y, ftr.z, 255, 255, 0, 70 },
		{ ftr.x, ftr.y, ftr.z, 255, 255, 0, 70 },
		{ ntr.x, ntr.y, ntr.z, 255, 255, 0, 70 },
		{ nbr.x, nbr.y, nbr.z, 255, 255, 0, 70 },};

	if (m_pVB) 
		wolf::BufferManager::DestroyBuffer(m_pVB);
	m_pVB = wolf::BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex)*5 *3 * 2);

	if (m_pDecl)
		delete m_pDecl;
	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();

	if (!s_pMaterial)
	{
		s_pMaterial = wolf::MaterialManager::CreateMaterial("frustum");
		s_pMaterial->SetProgram("data/cube.vsh", "data/cube.fsh");
		s_pMaterial->SetBlendMode(BlendMode::BM_SrcAlpha, BlendMode::BM_OneMinusSrcAlpha);
		s_pMaterial->SetBlend(true);
	}
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

int Frustum::OcTreeInFrustum(OcTree *p_pOcTree)
{
	AABB *aabb = p_pOcTree->GetAABB();

	int result = INSIDE;
	for(int i=0; i < 6; i++) {

		if (pl[i].distance(aabb->getVertexP(pl[i].normal)) < 0)
			return OUTSIDE;
		else if (pl[i].distance(aabb->getVertexN(pl[i].normal)) < 0)
			result =  INTERSECT;
	}
	return(result);
}

void Frustum::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	glm::mat4 mWorld = glm::mat4(glm::translate(0.0f,0.0f,0.0f));
	//printf("%f,%f,%f  %f,%f,%f  %f,%f,%f  %f,%f,%f\n",ftl.x,ftl.y,ftl.z,ftr.x,ftr.y,ftr.z,fbl.x,fbl.y,fbl.z,fbr.x,fbr.y,fbr.z);
	m_pDecl->Bind();

    s_pMaterial->SetUniform("world", mWorld);
	s_pMaterial->SetUniform("projection", p_mProj);
	s_pMaterial->SetUniform("view", p_mView);

	s_pMaterial->Apply();

    // Draw 5 Planes
	glDrawArrays(GL_TRIANGLES, 0, 5* 6);
}