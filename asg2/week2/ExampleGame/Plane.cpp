
#include "Plane.h"


Plane::Plane( vec3 &v1,  vec3 &v2,  vec3 &v3)
{
	set3Points(v1,v2,v3);
}

void Plane::set3Points( vec3 &v1,  vec3 &v2,  vec3 &v3)
{
	vec3 aux1, aux2;

	aux1 = glm::vec3(v1 - v2);
	aux2 = glm::vec3(v3 - v2);

	normal = glm::cross(aux2, aux1);
	normal = glm::normalize(normal);
	
	point = glm::vec3(v2);
	d = -(glm::dot(normal,point));
}

void Plane::setNormalAndPoint(vec3 &p_normal, vec3 &p_point) {

	normal = vec3(p_normal);
	normal = glm::normalize(normal);
	d = -(glm::dot(normal,p_point));
}

void Plane::setCoefficients(float a, float b, float c, float d) {
	
	// set the normal vector
	normal = vec3(a,b,c);
	//compute the lenght of the vector
	float l = normal.length();
	// normalize the vector
	normal = vec3(a/l,b/l,c/l);
	// and divide d by th length as well
	this->d = d/l;
}


	

float Plane::distance(const vec3 &p) {

	return (d + glm::dot(normal,p));
}


