/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The plane class
*  This is a subclass of SceneObject, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_PLANE
#define H_PLANE
#include <glm/glm.hpp>
#include "SceneObject.h"
#include <math.h>

/*
 * Defines a simple Plane use 1 point a and a normal vector n.
 * with the specified radius
 */
class Plane :public SceneObject {
private:
	glm::vec3 point; //mid point on the plane
	glm::vec3 normalVec; //normal vector of the plane
    float width;
    float height;

public:
	Plane() {}; //Default constructor creates a unit plane
    Plane(glm::vec3 pt, glm::vec3 norm, float w, float h) : point(pt), normalVec(norm), width(w), height(h) {};

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);
};
#endif //!H_PLANE