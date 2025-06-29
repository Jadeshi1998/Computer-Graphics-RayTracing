/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of SceneObject, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Sphere.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray. 
*/
float Sphere::intersect(glm::vec3 p0, glm::vec3 dir) {
	//Transform
	p0 = inverseTransform * glm::vec4(p0, 1.0);
	dir = inverseTransform * glm::vec4(dir, 0.0);
	float tScale = glm::length(dir);
	dir = glm::normalize(dir);

	glm::vec3 vdif = p0 - center;   //Vector s (see Slide 28)
	float b = glm::dot(dir, vdif);
	float len = glm::length(vdif);
	float c = len*len - radius*radius;
	float delta = b*b - c;

	if(delta <= 0) return -1;    


	float t1 = -b - sqrt(delta);
	float t2 = -b + sqrt(delta);
	t1 /= tScale;
	t2 /= tScale;

	if (t1 < 0) {
		if (t2 > 0) {
			return t2;
		}
		else {
			return -1;
		}
	}
	else {
		return t1;
	}

}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Sphere::normal(glm::vec3 p) {
	p = inverseTransform * glm::vec4(p, 1.0);
	glm::vec3 n = p - center;
	n = normalTransform * glm::vec4(n, 0.0);
	n = glm::normalize(n);
	return n;
}
