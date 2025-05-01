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

	float t1 = ( - b - sqrt(delta)) / (2 * a);
	float t2 = ( - b + sqrt(delta)) / (2 * a);

	//cut height hit>yc+h(outside) hit<yc+h(under)
	float t = -1;

	if (t1 > 0.001) {
		float y1 = p0.y + t1 * dir.y;
		if (y1 >= center.y && y1 <= center.y + height) {
			t = t1;
		}
	}
	else {
		if (t2 > 0.001) {
			float y2 = p0.y + t2 * dir.y;
			if ((t < 0 || t2 < t) && y2 >= center.y && y2 <= center.y + height) {
				t = t2;
			}
		}
	}

	// if the closest point of intersection is above the cylinder and next within
	if (fabs(dir.y) > 0.01) {
		float capt1 = (center.y + height - p0.y) / dir.y; // top cap
		glm::vec3 cap = p0 + capt1 * dir;
		if (capt1 > 0.001 &&
			glm::length(glm::vec2(cap.x - center.x, cap.z - center.z)) <= radius) {
			if (t < 0 || capt1 < t) t = capt1;
		}
	}

	// Bottom cap
	if (fabs(dir.y) > 0.01) {
		float capb = (center.y - p0.y) / dir.y;
		glm::vec3 cap = p0 + capb * dir;
		if (capb > 0.001 &&
			glm::length(glm::vec2(cap.x - center.x, cap.z - center.z)) <= radius) {
			if (t < 0 || capb < t) t = capb;
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
