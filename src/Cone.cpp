/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of SceneObject, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

/**
* Cone's intersection method.  The input is a ray.
*/

float Cone::intersect(glm::vec3 p0, glm::vec3 dir) {
	float tanTheta = tan(angle);
	float tan2 = tanTheta * tanTheta;
	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float px = p0.x;
	float py = p0.y;
	float pz = p0.z;
	//local x,y,z
	float X = p0.x - center.x;
	float Y = height + center.y - p0.y  ;
	float Z = p0.z - center.z;

	float a = dx * dx + dz * dz - dy * dy * tan2;
	float b = 2* X * dx + 2* Z * dz + 2 * tan2 * Y * dy;
	float c = X * X+ Z * Z - tan2 * Y * Y;



	float delta = b * b - 4 * a * c;
	if (delta < 0.) return -1.0;    //includes zero and negative values

	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);

	//cut height hit>yc+h(outside) hit<yc+h(under)
	float t = -1;
	//change y dir to -y reverse 
	glm::vec3 intersect1 = p0 + t1 * dir;
	glm::vec3 intersect2 = p0 + t2 * dir;
	float bottom = center.y;
	float top = center.y + height;
	if (t1 > 0.001) {
		if (intersect1.y >= bottom && intersect1.y <= top) {
			t = t1;
		}
	}
	else {
		if (t2 > 0.001) {
			if ((t < 0 || t2 < t) && intersect2.y >= bottom && intersect2.y <= top) {
				t = t2;
			}
		}
	}

	return t;
}



glm::vec3 Cone::normal(glm::vec3 p) {

	// new radius = radius / height
	glm::vec3 n = glm::vec3(p.x - center.x,
		radius / height * sqrt((p.x - center.x) * (p.x - center.x) +(p.z - center.z) * (p.z - center.z)),
		                    p.z - center.z);

	return glm::normalize(n);
}

