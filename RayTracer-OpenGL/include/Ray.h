#include <glm/glm.hpp>

#ifndef __RAY_H__
#define __RAY_H__
class Ray {
public:
    glm::vec3 p0; // basepoint
    glm::vec3 dir; // direction
    Ray(glm::vec3 p0, glm::vec3 dir):p0(p0), dir(dir){}
};
#endif