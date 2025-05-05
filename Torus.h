/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Torus class
*  This is a subclass of SceneObject, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_TORUS
#define H_TORUS

#include <glm/glm.hpp>
#include "SceneObject.h"
#include <vector>
#include <cmath>

/**
 * Defines a torus (ring-shaped object) centered at 'center'
 * Major radius R is the distance from the center to the tube center,
 * minor radius r is the radius of the tube.
 */
class Torus : public SceneObject {
private:
    glm::vec3 center = glm::vec3(0);
    float majorRadius = 1.0f;  // R
    float tubeRadius = 0.3f;   // r

public:
    Torus() {}

    Torus(glm::vec3 c, float R, float r)
        : center(c), majorRadius(R), tubeRadius(r) {
    }

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);
};

#endif // H_TORUS
