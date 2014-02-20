#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

using namespace glm;

class Plane  
{

public:

	vec3 normal,point;
	float d;


	Plane( vec3 &v1,  vec3 &v2,  vec3 &v3);
	Plane(){};
	~Plane(){};

	void set3Points( vec3 &v1,  vec3 &v2,  vec3 &v3);
	void setNormalAndPoint(vec3 &normal, vec3 &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(const vec3 &p);

	void print();

};


#endif