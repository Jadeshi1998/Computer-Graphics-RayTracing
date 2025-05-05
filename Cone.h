
/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of SceneObject, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_CONE
#define H_CONE
#include <glm/glm.hpp>
#include "SceneObject.h"
#include <math.h>


/**
 * Defines a simple cylinder located at 'center'
 * with the specified radius
 */
class Cone : public SceneObject {
private:
	glm::vec3 center = glm::vec3(0);
	float radius = 1;
	float height = 1;
	float angle;

public:
	Cone():angle(0) {};  //Default constructor creates a unit sphere

	Cone(glm::vec3 c, float r, float h ) :
		center(c), radius(r), height(h) {
		angle = atan(r / h);
	}

	float intersect(glm::vec3 p0, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);
};

#endif //H_CONE