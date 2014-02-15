#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class AABB
{
public:
	AABB(glm::vec3 p_vMin, glm::vec3 p_vCorner);
	glm::vec3 vCorner[8];

	glm::vec3 GetCenter();
	glm::vec3 GetHalfExtent();

	glm::vec3 AABB::getVertexP(glm::vec3 normal);
	glm::vec3 AABB::getVertexN(glm::vec3 normal); 
private:
	glm::vec3 m_vMin;
	glm::vec3 m_vMax;

	float x,y,z;
};

#endif

