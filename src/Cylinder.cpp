/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of SceneObject, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

/**
* cylinder's intersection method.  The input is a ray.
*/

float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir) {
	float a = dir.x * dir.x + dir.z * dir.z;
	float b = 2 * ((p0.x - center.x) * dir.x + (p0.z - center.z) * dir.z);
	float c = (p0.x - center.x) * (p0.x - center.x) + (p0.z - center.z) * (p0.z - center.z) - radius * radius;
	float delta = b * b - 4 * a * c;
	if (delta < 0.001) return -1.0;    //includes zero and negative values

	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);

	//cut height hit>yc+h(outside) hit<yc+h(under)
	float t = -1;
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

	// with cap : if the closest point of intersection is above the cylinder and next within
     if (intersect1.y >= top) {
		float capt1 = (top - p0.y) / dir.y; // top cap
		if (capt1 > 0 && intersect2.y <= top && intersect2.y >= bottom) { 
			t = capt1; 
		}
	}

	return t;
}





glm::vec3 Cylinder::normal(glm::vec3 p) {


	if (fabs(p.y - center.y) < 0.001)
		return glm::vec3(0, -1, 0); // bottom cap
	else if (fabs(p.y - (center.y + height)) < 0.001)
		return glm::vec3(0, 1, 0); // top cap
	else {
		// side surface normal
		glm::vec3 n = glm::vec3(p.x - center.x, 0, p.z - center.z);
		return glm::normalize(n);
	}
}
