#include <glm/glm.hpp>
#include "Triangle.h"

#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

class Intersection {
public:
    glm::vec3 P; // position of the intersection
    glm::vec3 N; // surface normal
    glm::vec3 V; // direction to incoming ray
    Triangle* triangle; // pointer to geometric primitive (and material info)
    float dist; // distance to the source of ray
};
#endif