/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The plane class
*  This is a subclass of SceneObject, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Plane.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray.
* Use the poing a be a central and have size weidth*height
*/


float Plane::intersect(glm::vec3 p0, glm::vec3 dir) {
	//p0+td=p,(p-a).n=0 >>  t=(a-p0).n/d.n
	float t = glm::dot(point - p0, normalVec) / glm::dot(dir, normalVec);
	if (t < 0) return -1.0;

	glm::vec3 p = p0 + t * dir;
	glm::vec3 u = glm::normalize(glm::cross(normalVec, glm::vec3(0, 1, 0)));
	glm::vec3 v = glm::normalize(glm::cross(normalVec, u));
	// u and v is this plane
	//point is mid point on the plane, local is the ray p's distence to point
	glm::vec3 local = p - point;
	//p to u on plan
	float uDist = glm::dot(local, u);
	//p to v on plan
	float vDist = glm::dot(local, v);
	//Outside bounds
	if (fabs(uDist) > width / 2 || fabs(vDist) > height / 2) {
		return -1; 
	}

	return t;

}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Plane::normal(glm::vec3 p) {
	return normalVec;
}


