#include "AABB.h"

AABB::AABB(glm::vec3 p_vMin, glm::vec3 p_vCorner)
{
	vCorner[0] = glm::vec3(p_vMin.x, p_vMin.y, p_vMin.z);
	vCorner[1] = glm::vec3(p_vMin.x + p_vCorner.x, p_vMin.y, p_vMin.z);
	vCorner[2] = glm::vec3(p_vMin.x, p_vMin.y, p_vMin.z + p_vCorner.z);
	vCorner[3] = glm::vec3(p_vMin.x + p_vCorner.x, p_vMin.y , p_vMin.z + p_vCorner.z);

	vCorner[4] = glm::vec3(p_vMin.x, p_vMin.y + p_vCorner.y, p_vMin.z);
	vCorner[5] = glm::vec3(p_vMin.x + p_vCorner.x, p_vMin.y + p_vCorner.y, p_vMin.z);
	vCorner[6] = glm::vec3(p_vMin.x, p_vMin.y + p_vCorner.y, p_vMin.z + p_vCorner.z);
	vCorner[7] = glm::vec3(p_vMin.x + p_vCorner.x, p_vMin.y + p_vCorner.y, p_vMin.z + p_vCorner.z);

	m_vMin = vCorner[0];
	m_vMax = vCorner[7];

	x = p_vCorner.x;
	y = p_vCorner.y;
	z = p_vCorner.z;
}

glm::vec3 AABB::GetCenter()
{
	return (m_vMax + m_vMin)*0.5f;
}

glm::vec3 AABB::GetHalfExtent()
{
	return (m_vMax - m_vMin)*0.5f;
}

glm::vec3 AABB::getVertexP(glm::vec3 normal) 
{

	glm::vec3 res = glm::vec3(vCorner[0]);

	if (normal.x > 0)
		res.x += x;

	if (normal.y > 0)
		res.y += y;

	if (normal.z > 0)
		res.z += z;

	return(res);
}



glm::vec3 AABB::getVertexN(glm::vec3 normal) 
{
	glm::vec3 res = glm::vec3(vCorner[0]);

	if (normal.x < 0)
		res.x += x;

	if (normal.y < 0)
		res.y += y;

	if (normal.z < 0)
		res.z += z;

	return(res);
}